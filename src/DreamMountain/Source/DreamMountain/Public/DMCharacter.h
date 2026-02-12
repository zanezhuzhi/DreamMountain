// DMCharacter.h - 代号梦山玩家角色
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DMCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * 梦山玩家可控角色。负责地面行走、奔跑、跳跃与攀爬等基础移动逻辑。
 * 第三人称视角，类似《光遇》，镜头跟随角色背后，可自由旋转。
 * @see 策划文档：03_系统设计/01_攀爬与移动.md
 * @see 技术规范：02_技术架构/工程技术规范.md 1.3 镜头系统
 */
UCLASS()
class DREAMMOUNTAIN_API ADMCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ADMCharacter();

	/** 前后移动（W/S 轴） */
	UFUNCTION(BlueprintCallable, Category = "DreamMountain|Movement")
	void MoveForward(float Value);

	/** 左右移动（A/D 轴） */
	UFUNCTION(BlueprintCallable, Category = "DreamMountain|Movement")
	void MoveRight(float Value);

	/** 镜头左右旋转（鼠标 X） */
	void Turn(float Value);

	/** 镜头俯仰（鼠标 Y） */
	void LookUp(float Value);

	/** 按下 Shift 进入奔跑 */
	void StartRun();

	/** 松开 Shift 退出奔跑 */
	void StopRun();

	/** 当前体力（用于 UI） */
	UFUNCTION(BlueprintCallable, Category = "DreamMountain|Stamina")
	float GetCurrentStamina() const { return CurrentStamina; }

	/** 最大体力（用于 UI） */
	UFUNCTION(BlueprintCallable, Category = "DreamMountain|Stamina")
	float GetMaxStamina() const;

	/** 体力百分比 [0,1]（用于 UI） */
	UFUNCTION(BlueprintCallable, Category = "DreamMountain|Stamina")
	float GetStaminaPercent() const;

	/** 切换到攀爬镜头目标（P0 预留接口） */
	UFUNCTION(BlueprintCallable, Category = "DreamMountain|Camera")
	void EnterClimbCamera();

	/** 切回默认镜头目标（P0 预留接口） */
	UFUNCTION(BlueprintCallable, Category = "DreamMountain|Camera")
	void ExitClimbCamera();

	/** 调试：切换攀爬镜头目标（默认绑定 C） */
	UFUNCTION(BlueprintCallable, Category = "DreamMountain|Camera")
	void ToggleClimbCamera();

	/** 弹簧臂：镜头跟随角色背后，类《光遇》 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DreamMountain|Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** 第三人称相机 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DreamMountain|Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	/** 当前体力，策划文档 STAMINA_MAX */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DreamMountain|Stamina")
	float CurrentStamina = 100.0f;

	/** 默认视角：弹簧臂长度（MMO 风格，支持编辑器调参） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamMountain|Camera|Tuning")
	float DefaultCameraArmLength = 700.0f;

	/** 默认视角：镜头偏移（默认 Y=0，不使用 OTS） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamMountain|Camera|Tuning")
	FVector DefaultCameraOffset = FVector(0.0f, 0.0f, 80.0f);

	/** 默认视角 FOV */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamMountain|Camera|Tuning")
	float DefaultCameraFOV = 90.0f;

	/** 攀爬视角：弹簧臂长度（拉近） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamMountain|Camera|Tuning")
	float ClimbCameraArmLength = 450.0f;

	/** 攀爬视角：镜头偏移 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamMountain|Camera|Tuning")
	FVector ClimbCameraOffset = FVector(0.0f, 0.0f, 100.0f);

	/** 攀爬视角 FOV */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamMountain|Camera|Tuning")
	float ClimbCameraFOV = 85.0f;

	/** 相机过渡速度（Tick 中 FInterpTo 使用） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DreamMountain|Camera|Tuning")
	float CameraTransitionSpeed = 6.0f;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** 负重修正系数，影响体力恢复速度；暂返回 1.0，后续可扩展 */
	UFUNCTION(BlueprintCallable, Category = "DreamMountain|Stamina")
	virtual float GetEncumbranceModifier() const { return 1.0f; }

private:
	/** 是否处于奔跑状态 */
	bool bIsRunning = false;

	/** 当前是否处于“攀爬镜头目标态” */
	bool bUseClimbCamera = false;

	void UpdateCameraTransition(float DeltaTime);
};
