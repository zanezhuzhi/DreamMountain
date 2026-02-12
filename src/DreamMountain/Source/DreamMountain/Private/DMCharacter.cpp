// DMCharacter.cpp - 代号梦山基础移动实现
// 第三人称视角，镜头跟随角色背后，类《光遇》
// 体力逻辑：策划文档 01_攀爬与移动.md 4.3、6.1

#include "DMCharacter.h"
#include "DMGameConfig.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"

ADMCharacter::ADMCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 相机默认参数从 DMConfig 初始化，可在蓝图/编辑器中继续调参覆盖
	DefaultCameraArmLength = DMConfig::CAMERA_DEFAULT_ARM_LENGTH;
	DefaultCameraOffset = FVector(
		DMConfig::CAMERA_DEFAULT_OFFSET_X,
		DMConfig::CAMERA_DEFAULT_OFFSET_Y,
		DMConfig::CAMERA_DEFAULT_OFFSET_Z
	);
	DefaultCameraFOV = DMConfig::CAMERA_DEFAULT_FOV;

	ClimbCameraArmLength = DMConfig::CAMERA_CLIMB_ARM_LENGTH;
	ClimbCameraOffset = FVector(
		DMConfig::CAMERA_CLIMB_OFFSET_X,
		DMConfig::CAMERA_CLIMB_OFFSET_Y,
		DMConfig::CAMERA_CLIMB_OFFSET_Z
	);
	ClimbCameraFOV = DMConfig::CAMERA_CLIMB_FOV;
	CameraTransitionSpeed = DMConfig::CAMERA_TRANSITION_SPEED;

	// 0. 确保被 Player0 控制才能接收输入
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// 1. 设置默认行走速度
	GetCharacterMovement()->MaxWalkSpeed = DMConfig::MOVE_SPEED;

	// 体力
	CurrentStamina = DMConfig::STAMINA_MAX;

	// 2. 启用方向随运动旋转 (类《光遇》的丝滑转向感)
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 3. 第三人称镜头：弹簧臂 + 相机 (工程技术规范 1.3)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = DefaultCameraArmLength;
	CameraBoom->bUsePawnControlRotation = true;  // 镜头随鼠标旋转
	CameraBoom->SocketOffset = DefaultCameraOffset;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 8.0f;  // 丝滑跟随

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;  // 由 SpringArm 控制
	FollowCamera->FieldOfView = DefaultCameraFOV;
}

void ADMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Action 绑定
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ADMCharacter::StartRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ADMCharacter::StopRun);
	PlayerInputComponent->BindAction("ToggleClimbCamera", IE_Pressed, this, &ADMCharacter::ToggleClimbCamera);

	// Axis 绑定（基于 Controller Yaw 的方向，按 W 向镜头朝向移动）
	PlayerInputComponent->BindAxis("MoveForward", this, &ADMCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADMCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ADMCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ADMCharacter::LookUp);
}

void ADMCharacter::StartRun()
{
	// 力竭时不允许进入奔跑（策划 4.1 表格：前置条件 体力 > 0）
	if (CurrentStamina <= 0.0f) { return; }
	// 首帧扣除：策划 4.3 规定，进入奔跑时立即扣一次每秒消耗
	CurrentStamina -= DMConfig::STAMINA_COST_RUN;
	CurrentStamina = FMath::Max(0.0f, CurrentStamina);
	bIsRunning = true;
	GetCharacterMovement()->MaxWalkSpeed = DMConfig::RUN_SPEED;
}

void ADMCharacter::StopRun()
{
	bIsRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = DMConfig::MOVE_SPEED;
}

float ADMCharacter::GetMaxStamina() const
{
	return DMConfig::STAMINA_MAX;
}

float ADMCharacter::GetStaminaPercent() const
{
	const float MaxStamina = GetMaxStamina();
	if (MaxStamina <= 0.0f)
	{
		return 0.0f;
	}
	return FMath::Clamp(CurrentStamina / MaxStamina, 0.0f, 1.0f);
}

void ADMCharacter::EnterClimbCamera()
{
	bUseClimbCamera = true;
}

void ADMCharacter::ExitClimbCamera()
{
	bUseClimbCamera = false;
}

void ADMCharacter::ToggleClimbCamera()
{
	bUseClimbCamera = !bUseClimbCamera;
}

void ADMCharacter::UpdateCameraTransition(float DeltaTime)
{
	if (!CameraBoom || !FollowCamera)
	{
		return;
	}

	const float InterpSpeed = FMath::Max(0.01f, CameraTransitionSpeed);
	const float TargetArmLength = bUseClimbCamera ? ClimbCameraArmLength : DefaultCameraArmLength;
	const FVector TargetOffset = bUseClimbCamera ? ClimbCameraOffset : DefaultCameraOffset;
	const float TargetFOV = bUseClimbCamera ? ClimbCameraFOV : DefaultCameraFOV;

	// 按要求：在 Tick 中使用 FInterpTo 进行平滑过渡
	CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, TargetArmLength, DeltaTime, InterpSpeed);
	CameraBoom->SocketOffset.X = FMath::FInterpTo(CameraBoom->SocketOffset.X, TargetOffset.X, DeltaTime, InterpSpeed);
	CameraBoom->SocketOffset.Y = FMath::FInterpTo(CameraBoom->SocketOffset.Y, TargetOffset.Y, DeltaTime, InterpSpeed);
	CameraBoom->SocketOffset.Z = FMath::FInterpTo(CameraBoom->SocketOffset.Z, TargetOffset.Z, DeltaTime, InterpSpeed);
	FollowCamera->FieldOfView = FMath::FInterpTo(FollowCamera->FieldOfView, TargetFOV, DeltaTime, InterpSpeed);
}

void ADMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	const bool bMoving = MoveComp && MoveComp->Velocity.Size() > 0.0f;

	// 力竭惩罚：体力耗尽时强制停止奔跑
	if (bIsRunning && CurrentStamina <= 0.0f)
	{
		StopRun();
	}

	if (bIsRunning && bMoving)
	{
		// 持续消耗：奔跑且移动时按 DeltaTime 扣除
		const float Cost = DMConfig::STAMINA_COST_RUN * DeltaTime;
		CurrentStamina = FMath::Max(0.0f, CurrentStamina - Cost);
	}
	else
	{
		// 恢复机制：静止或行走时每秒恢复
		const float Recover = DMConfig::STAMINA_REGEN * DeltaTime * GetEncumbranceModifier();
		CurrentStamina = FMath::Min(DMConfig::STAMINA_MAX, CurrentStamina + Recover);
	}

	UpdateCameraTransition(DeltaTime);

}

void ADMCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ADMCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ADMCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// 找出角色当前面对的方向
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 获取前向向量并施加移动指令
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ADMCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// 找出角色当前的右侧方向
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 获取右向向量并施加移动指令
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}
