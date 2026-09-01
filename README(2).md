# 风与龙之歌

一个使用 **C++17** 编写的单机文字 MUD 游戏。

游戏以蒙德地区为背景，玩家扮演旅行者，从低语森林出发，经过蒙德城、风起地遗迹与风龙废墟，最终完成对风魔龙的净化。

项目采用控制台文字交互，包含地图探索、NPC 对话、任务推进、背包与装备、回合制战斗、敌人掉落以及存档读取等功能。

---

## 游戏简介

玩家通过输入文字命令进行行动，例如：

```text
north
east
talk 安柏
attack 丘丘人
inventory
use 苹果
status
save
```

游戏的主要流程为：

```text
探索地图
   ↓
与 NPC 对话
   ↓
推进主线任务
   ↓
击败区域敌人
   ↓
获得装备和恢复道具
   ↓
解除风之封印
   ↓
进入风龙废墟
   ↓
挑战风魔龙
```

---

## 主要功能

### 地图探索

游戏目前包含四个主要区域：

- 低语森林
- 蒙德城
- 风起地遗迹
- 风龙废墟

玩家可以通过：

```text
north / south / east / west
```

或：

```text
北 / 南 / 东 / 西
```

在房间之间移动。

每个房间可能包含：

- NPC
- 敌人
- 地图出口
- 剧情事件
- 主线任务提示

---

### NPC 与剧情

游戏中的主要 NPC 包括：

- 安柏
- 琴
- 温迪
- 瓦格纳

玩家可以输入：

```text
talk 安柏
```

与当前房间中的 NPC 对话。

部分 NPC 对话会：

- 推进主线任务
- 给出下一步方向提示
- 给予装备或任务物品
- 解锁新的区域

---

## 战斗系统

游戏采用回合制战斗。

战斗中可以选择：

```text
1. 普通攻击
2. 使用物品
3. 逃跑
```

普通敌人允许逃跑，Boss 战无法逃离。

战斗会考虑：

- 攻击力
- 防御力
- 敏捷
- 命中率
- 闪避率
- 随机伤害浮动

基础伤害逻辑大致为：

```text
实际伤害 =
攻击力
- 敌人防御 / 2
+ 随机浮动
```

并保证至少造成 1 点伤害。

---

## 敌人

目前敌人按游戏进度逐渐增强：

```text
丘丘人
  ↓
风史莱姆
  ↓
丘丘人射手
  ↓
深渊法师
  ↓
丘丘暴徒
  ↓
遗迹守卫
  ↓
风魔龙
```

不同敌人拥有不同的：

- HP
- 攻击
- 防御
- 敏捷
- 命中率
- 闪避率
- EXP 奖励
- 摩拉奖励
- 掉落物

例如：

```text
丘丘人
→ 苹果

深渊法师
→ 净化结晶
→ 北地烟熏鸡

丘丘暴徒
→ 甜甜花酿鸡

遗迹守卫
→ 治疗药剂
→ 北地烟熏鸡
```

---

## Boss：风魔龙

风魔龙使用独立的 `BossEnemy` 类。

Boss 战分为两个主要阶段。

### 第一阶段：风之护盾

玩家需要先攻击风之护盾：

```text
第一阶段目标：
击破风之护盾
```

在护盾存在期间，对风魔龙造成的攻击会优先削减护盾值。

护盾被击破后：

```text
>>> 风之护盾破碎！
>>> 风魔龙降落在平台上。
>>> 战斗进入第二阶段。
```

### 第二阶段：本体战

第二阶段可以直接攻击风魔龙本体。

风魔龙会周期性释放：

```text
【风息】
```

风息会造成比普通攻击更高的伤害。

---

## 玩家系统

玩家拥有以下主要属性：

```text
等级
经验 EXP
生命 HP
体力 TP
元素充能
攻击
防御
敏捷
命中率
闪避率
摩拉
```

装备会影响玩家的实际攻击和防御。

---

## 背包与物品

输入：

```text
inventory
```

可以查看当前背包和装备。

### 武器

- 无锋剑
- 西风剑

### 护甲

- 旅行者外衣
- 骑士护甲

### 恢复道具

#### 苹果

```text
恢复少量 HP
```

#### 甜甜花酿鸡

```text
恢复中量 HP
```

#### 治疗药剂

```text
恢复大量 HP
```

#### 北地烟熏鸡

```text
恢复 TP
```

北地烟熏鸡主要通过击败特定敌人获得。

### 任务物品

- 风龙的眼泪
- 净化结晶
- 风之钥匙

---

## 装备系统

输入：

```text
equip 骑士护甲
```

可以装备背包中的武器或护甲。

卸下装备：

```text
unequip weapon
unequip armor
```

装备会直接影响：

```text
实际攻击力
实际防御力
```

---

## 常用命令

| 命令 | 中文 | 功能 |
|---|---|---|
| `help` | 帮助 | 查看命令 |
| `look` | 查看 | 查看当前房间 |
| `north` | 北 | 向北移动 |
| `south` | 南 | 向南移动 |
| `east` | 东 | 向东移动 |
| `west` | 西 | 向西移动 |
| `talk <npc>` | 对话 | 与 NPC 对话 |
| `attack <enemy>` | 攻击 | 与敌人战斗 |
| `inventory` | 背包 | 查看背包 |
| `use <item>` | 使用 | 使用消耗品 |
| `equip <item>` | 装备 | 装备武器或护甲 |
| `unequip weapon` | — | 卸下武器 |
| `unequip armor` | — | 卸下护甲 |
| `status` | 状态 | 查看角色状态 |
| `save` | 保存 | 保存当前游戏 |
| `load` | 读取 | 加载存档 |
| `new` | 新游戏 | 重新开始 |
| `menu` | 菜单 | 返回主菜单 |
| `quit` | 退出 | 退出游戏 |

---

## 存档系统

游戏支持：

```text
save
load
```

存档会记录主要游戏状态，包括：

- 玩家属性
- 当前房间
- 等级与经验
- 摩拉
- 背包物品
- 当前装备
- 主线任务阶段
- 已击败敌人

默认存档文件：

```text
savegame.txt
```

---

## 项目结构

主要代码文件：

```text
genshinn/
│
├─ main.cpp
│
├─ Game.cpp
├─ Game.h
│
├─ Character.cpp
├─ Character.h
│
├─ Player.cpp
├─ Player.h
│
├─ NPC.cpp
├─ NPC.h
│
├─ Enemy.cpp
├─ Enemy.h
│
├─ BossEnemy.cpp
├─ BossEnemy.h
│
├─ CombatSystem.cpp
├─ CombatSystem.h
│
├─ Item.h
│
├─ Inventory.cpp
├─ Inventory.h
│
├─ Equipment.cpp
├─ Equipment.h
│
├─ Room.cpp
├─ Room.h
│
├─ World.cpp
├─ World.h
│
├─ QuestManager.cpp
├─ QuestManager.h
│
├─ GameFactory.cpp
├─ GameFactory.h
│
├─ SaveManager.cpp
├─ SaveManager.h
│
├─ CommandParser.cpp
└─ CommandParser.h
```

不同模块分别负责游戏主循环、角色、战斗、地图、任务、物品和存档。

---

## 开发环境

推荐环境：

```text
Visual Studio 2022
C++17
MSVC v143
Windows 10 / Windows 11
```

项目使用中文控制台输出，建议启用 UTF-8。

Visual Studio 中可以加入：

```text
/utf-8
```

避免中文乱码。

---

## 编译与运行

使用 Visual Studio：

1. 打开 `.sln`
2. 确认 Platform Toolset 为本机已安装版本，例如 `v143`
3. 选择：

```text
x64
Debug
```

4. 编译：

```text
Ctrl + Shift + B
```

5. 不调试运行：

```text
Ctrl + F5
```

如果出现：

```text
LNK1168
无法打开 genshinn.exe 进行写入
```

通常是上一次运行的游戏进程没有关闭。

结束 `genshinn.exe` 后重新编译即可。

---

## 不要提交的文件

Visual Studio 会生成大量缓存和编译文件。

这些文件不需要上传到 GitHub，也不需要放进提交用压缩包：

```text
.vs/
x64/
Debug/
Release/

*.obj
*.pdb
*.ilk
*.exe
*.idb
*.ipch
*.VC.db
```

否则项目压缩包可能从几百 KB 直接变成几百 MB。

推荐 `.gitignore`：

```gitignore
.vs/

Debug/
Release/
x64/
x86/

*.obj
*.pdb
*.ilk
*.exe
*.idb
*.ipch
*.VC.db
```

---

## 技术实现

项目主要使用：

- C++17
- 面向对象程序设计
- 继承与多态
- STL 容器
- `std::vector`
- `std::unordered_map`
- `std::set`
- `std::unique_ptr`
- `std::pair`
- 随机数
- 文件读写

主要类关系包括：

```text
Character
├─ Player
├─ NPC
└─ Enemy
    └─ BossEnemy
```

同时使用 `GameFactory` 集中创建：

```text
物品
敌人
Boss
地图
```

避免在游戏逻辑中重复编写对象初始化代码。

---

## 当前版本目标

目前项目重点保证：

- 可以从头开始新游戏
- 可以完整探索四个区域
- NPC 可以正常推进剧情
- 普通敌人可以正常战斗
- 敌人可以掉落物品
- 玩家可以使用恢复道具
- 玩家可以装备武器和护甲
- 可以保存和读取游戏
- 可以完成风魔龙 Boss 战
- 可以从开场连续游玩到最终结局

---

## 游戏标题

```text
============================================================
                     风 与 龙 之 歌
                  单机文字 MUD · 蒙德篇
============================================================
```

愿风指引你的道路。
