#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
从项目根目录启动「代号梦山」文档构建（无需在命令行输入中文路径）。
用法: python build_dreamhill_docs.py
"""
import sys
from pathlib import Path

# 在代码中拼接路径，避免终端编码导致找不到脚本
project_root = Path(__file__).resolve().parent
dreamhill_dir = project_root / "项目" / "代号梦山"
build_script = dreamhill_dir / "build_docs.py"

if not build_script.exists():
    print(f"未找到脚本: {build_script}")
    sys.exit(1)

# 传入文档目录，执行 build_docs.py 的 main
sys.argv = [str(build_script), str(dreamhill_dir)]

with open(build_script, encoding="utf-8") as f:
    code = compile(f.read(), str(build_script), "exec")
exec(code)
