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
#include "Engine/Engine.h"

ADMCharacter::ADMCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

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
	CameraBoom->TargetArmLength = DMConfig::CAMERA_ARM_LENGTH;
	CameraBoom->bUsePawnControlRotation = true;  // 镜头随鼠标旋转
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, DMConfig::CAMERA_RELATIVE_Z);
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 8.0f;  // 丝滑跟随

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;  // 由 SpringArm 控制
}

void ADMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Action 绑定
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ADMCharacter::StartRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ADMCharacter::StopRun);

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

	// 调试：实时显示体力
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 0.0f, FColor::Cyan,
			FString::Printf(TEXT("体力: %.1f / %.0f"), CurrentStamina, DMConfig::STAMINA_MAX));
	}
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
