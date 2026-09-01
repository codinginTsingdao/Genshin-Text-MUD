# genshinn - 《捕风的异乡人》完整课程设计版

这是在 `Rinn661/genshinn` 原项目基础上的整合版本。

## 最重要的约束：原 UI 保留

本版以 GitHub `main/develop/genshinn/Game.cpp` 为 UI 基准：

- 保留“捕风的异乡人”中文大字标题；
- 保留原来的 ANSI 颜色；
- 保留 `按任意键继续...`；
- 保留旅行者命名界面和欢迎语；
- 保留原来的鹰翔海滩开场剧情；
- 保留游戏中的 1~6 行动菜单：
  1. 四处探索
  2. 查看角色状态
  3. 打开背包
  4. 保存游戏
  5. 返回主菜单
  6. 退出游戏

新增功能只接在原 UI 后面，不再替换原 UI。

## 如何玩

进入原来的 1~6 行动菜单后，选择 `1. 四处探索`，进入文字 MUD 探索模式。

常用命令：

```text
help / 帮助
look / 查看
north / south / east / west
北 / 南 / 东 / 西
talk 安柏
attack 丘丘人
inventory / 背包
use 苹果
equip 骑士护甲
unequip weapon
status / 状态
save / 保存
load / 读取
menu / 返回
quit / 退出
```

其中 `menu` 或 `返回` 只退出探索模式，回到原来的 1~6 行动菜单。

## 主线

完整主线包括：

1. 鹰翔海滩醒来；
2. 在森林小径与安柏对话；
3. 清理丘丘人营地；
4. 进入蒙德城；
5. 与琴、温迪对话；
6. 前往风起地；
7. 击败三处敌人、解除三道封印；
8. 进入风龙废墟；
9. 风魔龙两阶段 Boss 战；
10. 净化风魔龙并进入结局。

## 项目模块

- `Game`：保留原 UI，负责各系统集成；
- `Character`：角色抽象基类；
- `Player`：继承 Character，并持有 Inventory / Equipment；
- `NPC`、`Enemy`、`BossEnemy`：角色派生体系；
- `Inventory`、`Equipment`、`Item`：沿用原仓库已有设计并正式接入游戏；
- `Room`、`World`：16 个房间、4 个区域；
- `CommandParser`：中英文文字命令解析；
- `QuestManager`：主线状态与三道封印；
- `CombatSystem`：普通敌人和风魔龙两阶段回合制战斗；
- `SaveManager`：真实文本存档 / 读档；
- `GameFactory`：物品、敌人和地图初始数据。

## Visual Studio 运行

推荐直接打开 `genshinn.vcxproj`。

工程默认配置：

- C++17
- x64
- Visual Studio 2022 `v143`
- `/utf-8`

然后：

```text
Ctrl + Shift + B   生成
Ctrl + F5          运行
```

`main.cpp` 已设置 Windows 控制台 UTF-8 输入输出，并启用 ANSI 颜色，避免中文乱码和颜色失效。

如果机器没有 v143，可在：

```text
项目属性 -> 配置属性 -> 常规 -> 平台工具集
```

选择本机已安装的 MSVC 工具集。

## CMake

也可使用：

```bash
cmake -S . -B build
cmake --build build
```

## 存档

运行目录下生成：

```text
savegame.txt
```

保存玩家属性、等级经验、摩拉、当前位置、背包、装备、任务阶段、封印数量和重要敌人击败状态。
