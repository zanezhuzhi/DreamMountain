# 代号梦山 — 文档站使用说明

通过网页浏览「代号梦山」项目文档（核心概念、企划、技术架构等），并可配合 Git 实现协作者在网页端或本地编辑内容。

## 快速开始（本地预览）

1. **安装依赖**（建议使用项目 venv）：
   ```bash
   pip install -r requirements-docs.txt
   ```

2. **启动本地站点**：
   ```bash
   mkdocs serve
   ```

3. 浏览器打开：**http://127.0.0.1:8000**

## 生成静态站点（用于部署）

```bash
mkdocs build
```

输出在 `site/` 目录，可部署到 Nginx、GitHub Pages、对象存储等任意静态托管。

## 协作者如何参与编辑

- **方式一**：将本仓库推送到 GitHub/GitLab，协作者在网页上编辑对应 `.md` 文件并提交；通过 CI 在推送后自动执行 `mkdocs build` 并发布站点。
- **方式二**：协作者克隆仓库，用 Cursor/VS Code 修改 `docs/` 下的 Markdown，提交或提 PR。

详细方案与权限说明见：[网页发布与协作指南](docs/08_其他/网页发布与协作指南.md)。
