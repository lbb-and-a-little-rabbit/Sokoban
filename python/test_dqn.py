import subprocess
import torch
from model import DQN
from utils import mapstr_to_tensor, infer_input_dim_from_map
import threading
import queue

# ===== Config =====
ENV_PATH = "../build/EnvServer.exe"
LEVEL_ID = "0"  #检测第x关训练效果，根据需要修改
MODEL_PATH = "dqn_sokoban.pth"

ACTION_DIM = 4
NUM_EPISODES = 50
MAX_STEPS_PER_EP = 1000

DEVICE = torch.device("cuda" if torch.cuda.is_available() else "cpu")

# ===== Launch Env =====
env = subprocess.Popen(
    [ENV_PATH, LEVEL_ID],
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,
    stderr=subprocess.DEVNULL,
    text=True,
    bufsize=1
)

# ===== Threaded stdout reader =====
q_stdout = queue.Queue()

def reader_thread(pipe, q):
    for line in iter(pipe.readline, ''):
        q.put(line)
    pipe.close()

threading.Thread(target=reader_thread, args=(env.stdout, q_stdout), daemon=True).start()

# ===== Init model =====
try:
    line = q_stdout.get(timeout=5).strip()  # 等 EnvServer 输出初始状态
except queue.Empty:
    raise RuntimeError("EnvServer did not output initial state!")

state_str, _, _ = line.split("\t")
input_dim = infer_input_dim_from_map(state_str)

policy = DQN(input_dim, ACTION_DIM).to(DEVICE)
policy.load_state_dict(torch.load(MODEL_PATH, map_location=DEVICE))
policy.eval()

success = 0
steps_all = []

print("Start testing...")

for ep in range(NUM_EPISODES):
    s = mapstr_to_tensor(state_str, device=DEVICE)  # 每局都用第一次读取的 state_str 初始化
    steps = 0
    done = False

    while not done and steps < MAX_STEPS_PER_EP:
        with torch.no_grad():
            q = policy(s.unsqueeze(0))
            action = int(q.argmax(1))

        # 发动作
        env.stdin.write(f"{action}\n")
        env.stdin.flush()

        try:
            line = q_stdout.get(timeout=2).strip()
        except queue.Empty:
            print("EnvServer timeout, skipping this episode")
            break

        ns_str, _, d_str = line.split("\t")
        done = int(d_str)
        steps += 1
        s = mapstr_to_tensor(ns_str, device=DEVICE)  # 更新下一步状态

    if done:
        success += 1
        steps_all.append(steps)

    print(f"Episode {ep+1:03d}: steps={steps}, success={done}")

rate = success / NUM_EPISODES
avg_steps = sum(steps_all) / len(steps_all) if steps_all else 0

print("\n===== Result =====")
print(f"Success rate: {rate*100:.2f}%")
print(f"Avg steps (success only): {avg_steps:.1f}")

env.terminate()
