// DMGameConfig.h
// 游戏核心数值配置，所有常量集中在此以便维护
// @see docs/03_系统设计/01_攀爬与移动.md

#pragma once

namespace DMConfig
{
	// --- 移动与体力 (Movement & Stamina) ---
	constexpr float MOVE_SPEED = 350.0f;      // 行走速度 (UE 单位/s)
	constexpr float RUN_SPEED = 550.0f;       // 奔跑速度
	constexpr float STAMINA_MAX = 100.0f;     // 体力上限
	constexpr float STAMINA_COST_RUN = 15.0f; // 奔跑每秒消耗（对应 STAMINA_COST_RUN_PER_SEC）
	constexpr float STAMINA_REGEN = 5.0f;     // 静止/行走时每秒恢复

	// --- 镜头 (Camera / P0) ---
	// 默认采用 MMO 风格长距离视角；攀爬态拉近。
	constexpr float CAMERA_DEFAULT_ARM_LENGTH = 700.0f;
	constexpr float CAMERA_DEFAULT_OFFSET_X = 0.0f;
	constexpr float CAMERA_DEFAULT_OFFSET_Y = 0.0f;
	constexpr float CAMERA_DEFAULT_OFFSET_Z = 80.0f;
	constexpr float CAMERA_DEFAULT_FOV = 90.0f;

	constexpr float CAMERA_CLIMB_ARM_LENGTH = 450.0f;
	constexpr float CAMERA_CLIMB_OFFSET_X = 0.0f;
	constexpr float CAMERA_CLIMB_OFFSET_Y = 0.0f;
	constexpr float CAMERA_CLIMB_OFFSET_Z = 100.0f;
	constexpr float CAMERA_CLIMB_FOV = 85.0f;

	constexpr float CAMERA_TRANSITION_SPEED = 6.0f;

	// --- 攀爬 (Climbing) ---
	// 将来如果有攀爬参数，也都加在这里
}
