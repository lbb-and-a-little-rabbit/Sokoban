# Sokoban(推箱子)

《推箱子》是Thinking Rabbit于1981年发行的一款益智类游戏。
《推箱子》中玩家需要在一个有限的空间内，通过移动搬运工，将木箱推到指定位置。游戏考验玩家的空间规划和策略安排能力，因为一旦操作不当，可能会导致箱子无法移动或通道被堵。玩家只能推动箱子，不能拉动，且每次只能推动一个。


本项目为对推箱子游戏的简易复刻，提供以下功能：
- 多关卡关卡设计
- 存档系统
- AI 自动解答（使用了A*算法，受算力和内存限制，5 秒内无法求解则强制停止AI求解进程）
- 最优步数提示
- 配备了简单的机器学习AI，Q表学习简单关卡成功率极高，但因为状态多且复杂，复杂关卡无法成功；DQN学习因为各关卡地图大小不同，难以统一，故进行单个关卡进行训练，但效果仍然不佳。甚至出现100%通过率后，再次学习后通过率为0的情况。即使通过与最优解相差依旧巨大。代码仅供参考，实际效果不佳。
- 通过EnvServer.cpp建立cpp-python通信协议，实现了同一项目中，c++和python的混用，以发挥各自语言的有点。

---

## 游戏操作
| 操作 | 键位 |
|------|------|
| 向上移动 | W |
| 向下移动 | S |
| 向左移动 | A |
| 向右移动 | D |
| 重新开始 | R |
| 下一关（通关后） | Space |
| 当前盘面最优解步数提示 | H |
| AI 自动求解 | P |
| 查看操作记录 | Tab |
| 游戏中返回菜单 | Esc |

---

## 编译与运行

### 依赖
- C++17 或更高版本
- CMake 3.15+
- MinGW（Windows）或其他支持 C++17 的编译器
- SFML3.x（用于渲染）
- PyTorch（用于RL）

### 使用方法
正常游戏
```bash
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
./build\Sokoban
```


AI训练 --qtable
```bash
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
./build\Sokoban --train
```

AI训练 --DQN
```bash
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
cd python
python dqn.py
```
学习结果检验
```bash
python test_dqn.py
```

## ❤️ 鸣谢

部分关卡设计：
<a href="https://github.com/Lhz0711">
  <img src="https://github.com/Lhz0711.png" width="50" />
</a>
