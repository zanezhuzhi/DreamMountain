# UE5 工程区 — 代号梦山

本目录为完整的 Unreal Engine 5 C++ 项目，可直接用 Epic Games Launcher 或 Unreal Editor 打开。

## 打开项目

1. 安装 [Unreal Engine 5.5](https://www.unrealengine.com/)（或 5.4，需修改 `.uproject` 中的 `EngineAssociation`）
2. 双击 `DreamMountain.uproject`，或从 Epic Launcher 选择「添加」→ 浏览到此目录
3. 首次打开会编译 C++ 模块，等待完成即可

## 当前实现

| 组件 | 说明 |
|------|------|
| **ADMCharacter** | 玩家角色，WASD 地面行走，MaxWalkSpeed=400 |
| **ADMGameModeBase** | 默认 GameMode，使用 DMCharacter 为 Pawn |
| **输入映射** | MoveForward (W/S)、MoveRight (A/D) 已配置 |

## 目录结构

- **Config/** — 引擎与输入配置
- **Content/** — 美术资源（厚涂 3D 素材）
- **Source/DreamMountain/** — C++ 源码

> UE5 临时文件（Intermediate/, Saved/, Binaries/）已通过 `.gitignore` 排除。
