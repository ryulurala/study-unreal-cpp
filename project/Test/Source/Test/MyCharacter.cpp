// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyAnimInstance.h"
#include "DrawDebugHelpers.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = 500.0f;	// 거리
	SpringArm->SetRelativeRotation(FRotator(-35.0f, 0.0f, 0.0f));	// 회전

	// -88.0f 국룰: 땅에서 88.0f 만큼 공중에 떠있는 상태를 바닥에 붙여놓기 위함
	// -90.0f: X방향을 월드의 X방향과 맞추기 위함
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_PARAGON(TEXT("SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'"));
	if (SK_PARAGON.Succeeded())
	{
		// Mesh가 이미 상속받은 Character class에 선언돼있다.
		GetMesh()->SetSkeletalMesh(SK_PARAGON.Object);
	}
}  

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);
		AnimInstance->OnAttackHit.AddUObject(this, &AMyCharacter::AttackCheck);
	}
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 액션을 바인딩한다.(눌렀다 뗐다 정도)
	// Jump는 이미 만들어져있다.
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyCharacter::Attack);

	// Player Controller 에서 바인딩하면 먼저 선점 가능하다.
	// Axis 바인딩
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &AMyCharacter::Yaw);
}

void AMyCharacter::UpDown(float Value)
{
	UpDownValue = Value;

	// Parameter: 1) 방향, 2) 크기
	AddMovementInput(GetActorForwardVector(), Value);
}

void AMyCharacter::LeftRight(float Value)
{
	LeftRightValue = Value;

	// Parameter: 1) 방향, 2) 크기
	AddMovementInput(GetActorRightVector(), Value);
}

void AMyCharacter::Yaw(float Value)
{
	AddControllerYawInput(Value);
}

void AMyCharacter::Attack()
{
	if (IsAttacking)
		return;

	AnimInstance->PlayAttackMontage();

	AnimInstance->JumpToSection(AttackIndex);
	AttackIndex = (AttackIndex + 1) % 3;

	IsAttacking = true;
}

void AMyCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);	// 태그 이름, 디테일한 판정인지, 무시할 액터

	float AttackRange = 100.0f;
	float AttackRadius = 50.0f;

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,		// 충돌한 결과물
		GetActorLocation(),	// 현재 액터의 위치부터
		GetActorLocation() + GetActorForwardVector() * AttackRange,		// 액터의 앞 방향에서 AttachRange까지
		FQuat::Identity,		// 회전 없이
		ECollisionChannel::ECC_GameTraceChannel2,	// "Config/DefaultEngine.ini" 설정 파일 참고
		FCollisionShape::MakeSphere(AttackRadius),	// AttackRadius 만큼의 구체로 
		Params		// 위에 정의한 콜리전 파라미터
	);

	if (bResult && HitResult.Actor.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s"), *HitResult.Actor->GetName());
	}

	// 디버깅
	FVector Direction = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + Direction * 0.5f;	
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Direction).ToQuat();	// 객체의 Z를 공격 방향으로 설정(= Pitch: 90도)
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;	// 충돌 시 초록, 기본은 빨강

	DrawDebugCapsule(
		GetWorld(),
		Center,			// 생성할 위치
		HalfHeight,		// Capsule의 절반 높이
		AttackRadius,	// Capsule의 반지름
		Rotation,		// 회전
		DrawColor,		// 색깔
		false,			// 지속적으로 보여줄 건지
		2.0f			// 2초 동안만 보여줌
	);
}

void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
}
