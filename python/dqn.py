import subprocess
import torch
import torch.nn as nn
import torch.optim as optim
import random
from collections import deque
import time

# ======================
# 参数区（你之后主要调这里）
# ======================

ENV_PATH = "../build/EnvServer.exe"   # 改成你的实际路径
STATE_DIM = 32                        # 你 encode 后的维度（必须固定）
ACTION_DIM = 4

GAMMA = 0.99
LR = 1e-3
BATCH_SIZE = 64
MEMORY_SIZE = 50000

EPS_START = 1.0
EPS_END = 0.05
EPS_DECAY = 0.9995

TARGET_UPDATE = 1000

DEVICE = torch.device("cuda" if torch.cuda.is_available() else "cpu")

# ======================
# DQN 网络
# ======================

class DQN(nn.Module):
    def __init__(self, state_dim, action_dim):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(state_dim, 256),
            nn.ReLU(),
            nn.Linear(256, 256),
            nn.ReLU(),
            nn.Linear(256, action_dim)
        )

    def forward(self, x):
        return self.net(x)


# ======================
# 状态解析（你可按 encode 改）
# ======================

def parse_state(state_str: str) -> torch.Tensor:
    """
    示例 state_str:
    P(5,6)|B(2,1);(4,3)|T(1,0);(3,2)

    规则：
    - 只提取数字（含负数）
    - 长度必须固定
    """
    nums = []
    i = 0
    while i < len(state_str):
        if state_str[i] == '-' or state_str[i].isdigit():
            j = i + 1
            while j < len(state_str) and state_str[j].isdigit():
                j += 1
            nums.append(int(state_str[i:j]))
            i = j
        else:
            i += 1

    if len(nums) != STATE_DIM:
        raise ValueError(f"State dim mismatch: got {len(nums)}, expect {STATE_DIM}")

    return torch.tensor(nums, dtype=torch.float32, device=DEVICE)


# ======================
# Replay Buffer
# ======================

class ReplayBuffer:
    def __init__(self, capacity):
        self.buffer = deque(maxlen=capacity)

    def push(self, s, a, r, s2, d):
        self.buffer.append((s, a, r, s2, d))

    def sample(self, batch_size):
        batch = random.sample(self.buffer, batch_size)
        s, a, r, s2, d = zip(*batch)
        return (
            torch.stack(s),
            torch.tensor(a, device=DEVICE),
            torch.tensor(r, device=DEVICE),
            torch.stack(s2),
            torch.tensor(d, device=DEVICE)
        )

    def __len__(self):
        return len(self.buffer)


# ======================
# 启动环境进程
# ======================

env = subprocess.Popen(
    [ENV_PATH],
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,
    text=True,
    bufsize=1
)

# ======================
# 初始化网络
# ======================

policy_net = DQN(STATE_DIM, ACTION_DIM).to(DEVICE)
target_net = DQN(STATE_DIM, ACTION_DIM).to(DEVICE)
target_net.load_state_dict(policy_net.state_dict())
target_net.eval()

optimizer = optim.Adam(policy_net.parameters(), lr=LR)
memory = ReplayBuffer(MEMORY_SIZE)

epsilon = EPS_START
global_step = 0

# ======================
# 训练一步
# ======================

def train_step():
    if len(memory) < BATCH_SIZE:
        return

    s, a, r, s2, d = memory.sample(BATCH_SIZE)

    q = policy_net(s).gather(1, a.unsqueeze(1)).squeeze(1)

    with torch.no_grad():
        max_q_next = target_net(s2).max(1)[0]
        target_q = r + GAMMA * max_q_next * (1 - d)

    loss = nn.functional.mse_loss(q, target_q)

    optimizer.zero_grad()
    loss.backward()
    nn.utils.clip_grad_norm_(policy_net.parameters(), 10.0)
    optimizer.step()


# ======================
# 主循环
# ======================

print("🚀 DQN training started")

try:
    while True:
        # 读取环境输出
        line = env.stdout.readline()
        if not line:
            break

        line = line.strip()
        if not line:
            continue

        # 拆行
        try:
            state_str, reward_str, done_str = line.split()
        except ValueError:
            print("⚠ Bad env output:", line)
            continue

        state = parse_state(state_str)
        reward = float(reward_str)
        done = float(done_str)

        # ε-greedy
        if random.random() < epsilon:
            action = random.randint(0, ACTION_DIM - 1)
        else:
            with torch.no_grad():
                action = policy_net(state).argmax().item()

        # 发送 action
        env.stdin.write(f"{action}\n")
        env.stdin.flush()

        # 读 step 后状态
        next_line = env.stdout.readline().strip()
        next_state_str, next_reward_str, next_done_str = next_line.split()

        next_state = parse_state(next_state_str)
        next_reward = float(next_reward_str)
        next_done = float(next_done_str)

        memory.push(state, action, next_reward, next_state, next_done)

        train_step()

        epsilon = max(EPS_END, epsilon * EPS_DECAY)

        if global_step % TARGET_UPDATE == 0:
            target_net.load_state_dict(policy_net.state_dict())

        if global_step % 1000 == 0:
            print(
                f"[{global_step}] "
                f"epsilon={epsilon:.3f} "
                f"buffer={len(memory)}"
            )

        global_step += 1

except KeyboardInterrupt:
    print("⛔ Training interrupted")

finally:
    env.terminate()
    torch.save(policy_net.state_dict(), "dqn_sokoban.pt")
    print("✅ Model saved to dqn_sokoban.pt")
