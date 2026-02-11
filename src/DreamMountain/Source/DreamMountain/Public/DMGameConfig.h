// DMGameConfig.h
// 游戏核心数值配置，所有常量集中在此以便维护
// @see docs/03_系统设计/01_攀爬与移动.md

#pragma once

namespace DMConfig
{
	// --- 移动与体力 (Movement & Stamina) ---
	constexpr float MOVE_SPEED = 350.0f;              // 行走速度 (UE 单位/s)
	constexpr float RUN_SPEED = 550.0f;               // 奔跑速度
	constexpr float STAMINA_MAX = 100.0f;              // 体力上限
	constexpr float STAMINA_COST_RUN_PER_SEC = 15.0f; // 奔跑每秒消耗
	constexpr float STAMINA_RECOVER_PER_SEC = 5.0f;    // 静止/行走时每秒恢复

	// --- 镜头 (Camera) ---
	constexpr float CAMERA_ARM_LENGTH = 300.0f;       // 弹簧臂长度，背后距离
	constexpr float CAMERA_RELATIVE_Z = 80.0f;        // 镜头相对高度

	// --- 攀爬 (Climbing) ---
	// 将来如果有攀爬参数，也都加在这里
}
