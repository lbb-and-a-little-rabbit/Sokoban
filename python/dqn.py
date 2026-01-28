import subprocess
import random
import torch
import torch.optim as optim
import torch.nn.functional as F
from collections import deque

from model import DQN
from utils import mapstr_to_tensor, infer_input_dim_from_map

# ===== Config =====
ENV_PATH = "../build/EnvServer.exe"
LEVEL_ID = "0"          # 固定训练第 x 关，根据需要修改
ACTION_DIM = 4

GAMMA = 0.99
LR = 1e-3
BATCH_SIZE = 64
MEMORY_SIZE = 50000
TARGET_UPDATE = 1000

EPS_START = 1.0
EPS_END = 0.05
EPS_DECAY = 0.9995

MAX_STEPS_PER_EP = 100000

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

# ===== Replay Buffer =====
class ReplayBuffer:
    def __init__(self, cap):
        self.buf = deque(maxlen=cap)

    def push(self, s, a, r, s2, d):
        self.buf.append((s, a, r, s2, d))

    def sample(self, n):
        batch = random.sample(self.buf, n)
        s, a, r, s2, d = zip(*batch)
        return (
            torch.stack(s).to(DEVICE),
            torch.tensor(a, device=DEVICE),
            torch.tensor(r, dtype=torch.float32, device=DEVICE),
            torch.stack(s2).to(DEVICE),
            torch.tensor(d, dtype=torch.float32, device=DEVICE),
        )

    def __len__(self):
        return len(self.buf)

memory = ReplayBuffer(MEMORY_SIZE)

# ===== Read initial state =====
print("Waiting for EnvServer...")
line = env.stdout.readline().strip()
state_str, _, _ = line.split("\t")
input_dim = infer_input_dim_from_map(state_str)
print("Input dim:", input_dim)

policy = DQN(input_dim, ACTION_DIM).to(DEVICE)
target = DQN(input_dim, ACTION_DIM).to(DEVICE)
target.load_state_dict(policy.state_dict())

optimizer = optim.Adam(policy.parameters(), lr=LR)

epsilon = EPS_START
global_step = 0
episode_step = 0

def select_action(s):
    global epsilon
    if random.random() < epsilon:
        return random.randrange(ACTION_DIM)
    with torch.no_grad():
        q = policy(s.unsqueeze(0))
        return int(q.argmax(1))

def train_step():
    if len(memory) < BATCH_SIZE:
        return None

    s, a, r, s2, d = memory.sample(BATCH_SIZE)
    q = policy(s).gather(1, a.unsqueeze(1)).squeeze(1)

    with torch.no_grad():
        q_next = target(s2).max(1)[0]
        y = r + GAMMA * q_next * (1 - d)

    loss = F.mse_loss(q, y)
    optimizer.zero_grad()
    loss.backward()
    torch.nn.utils.clip_grad_norm_(policy.parameters(), 10)
    optimizer.step()
    return loss.item()

print("Start training...")
try:
    while True:
        state_str, reward_str, done_str = line.split("\t")
        s = mapstr_to_tensor(state_str, device=DEVICE)

        action = select_action(s)
        env.stdin.write(f"{action}\n")
        env.stdin.flush()

        next_line = env.stdout.readline()
        if not next_line:
            break
        next_line = next_line.strip()

        ns_str, r_str, d_str = next_line.split("\t")
        r = float(r_str)
        d = int(d_str)

        if d or episode_step >= MAX_STEPS_PER_EP:
            s2 = torch.zeros_like(s)
            memory.push(s.cpu(), action, r, s2.cpu(), 1)
            episode_step = 0

            # 读取 reset 后的新初始状态
            line = env.stdout.readline().strip()
        else:
            s2 = mapstr_to_tensor(ns_str, device=DEVICE)
            memory.push(s.cpu(), action, r, s2.cpu(), 0)
            episode_step += 1
            line = next_line

        loss = train_step()
        epsilon = max(EPS_END, epsilon * EPS_DECAY)

        if global_step % 1000 == 0:
            print(f"[{global_step}] eps={epsilon:.3f} buf={len(memory)} loss={loss}")

        if global_step % TARGET_UPDATE == 0 and global_step > 0:
            target.load_state_dict(policy.state_dict())
            print("Target updated")

        global_step += 1
        line = next_line

except KeyboardInterrupt:
    print("Stopped")

finally:
    torch.save(policy.state_dict(), "dqn_sokoban.pth")
    print("Model saved")
    env.terminate()
