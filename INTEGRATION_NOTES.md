# 整合说明

## 原仓库内容的处理

### UI：冻结，不改

以 GitHub `main/develop/genshinn/Game.cpp` 为基准，保留：

- `showTitle()` 中文大字；
- `inputPlayerName()`；
- `showStory()` 原文；
- 主菜单；
- 鹰翔海滩行动界面；
- 原 1~6 菜单；
- ANSI 配色和 `system("cls")` / 按任意键节奏。

后续新增系统不再替换这些 UI。

### 原 Player

保留名字、等级、当前/最大生命、体力、元素充能等接口；扩展为 `Player : public Character`，并继续提供 `getLV/getNHP/getMHP/getTP/getENERGY` 兼容接口。

### 原 Inventory / Equipment / Item

核心结构和接口保留，并正式挂到 Player 上使用。

## 另一位同学角色/战斗代码

保留：

- Character 基类思想；
- Player / Enemy 继承关系；
- attack / defense / agility / hitRate / dodgeRate / alive；
- 回合制战斗流程。

没有保留已确认有问题的实现：

- `hp < 0` 导致死亡状态无法正确设置；
- 技能回合敌人重复攻击；
- 999/888/777 测试伤害；
- 假背包回血；
- 不必要的裸 `new/delete`。

## 新增集成层

- World / Room
- CommandParser
- QuestManager
- GameFactory
- CombatSystem
- SaveManager
- BossEnemy 两阶段战斗

## UI 与 MUD 的结合方式

为了既不改原 UI，又满足“文字 MUD / 命令解析”的课程要求：

```text
原 1~6 行动菜单
      |
      +-- 1. 四处探索
              |
              +-- 进入 MUD 命令模式
                   north / talk / attack / ...
              |
              +-- menu / 返回
                   回到原 1~6 行动菜单
```

因此原同学制作的 UI 仍是游戏的主交互框架，新增命令系统作为“探索”功能的内部实现。
