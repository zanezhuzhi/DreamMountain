# UE5 工程区

此目录用于存放《代号梦山》的 Unreal Engine 5 项目。

- **Config/** — 项目配置
- **Content/** — 存放美术资源（厚涂 3D 素材）
- **Source/** — 存放 C++ 代码（如攀爬逻辑 DreamClimber.cpp）

请在 UE5 中新建项目时，选择此目录作为项目根目录，`.uproject` 文件将放置于此。

> UE5 会生成大量临时文件（Intermediate/, Saved/, Binaries/），已通过 `.gitignore` 排除，避免 Git 状态栏被淹没。
