// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));

	RootComponent = Mesh;

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_CHAIR(TEXT("StaticMesh'/Game/StarterContent/Props/SM_Chair.SM_Chair'"));
	if (SM_CHAIR.Succeeded())
	{
		Mesh->SetStaticMesh(SM_CHAIR.Object);
	}
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Player Controller 에서 바인딩하면 먼저 선점 가능하다.
	// Axis 바인딩
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyPawn::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyPawn::LeftRight);
}

void AMyPawn::UpDown(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("UpDown: %f"), Value);

	if (Value == 0.f)
		return;

	// Parameter: 1) 방향, 2) 크기
	AddMovementInput(GetActorForwardVector(), Value);
}

void AMyPawn::LeftRight(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("LeftRight: %f"), Value);

	if (Value == 0.f)
		return;

	// Parameter: 1) 방향, 2) 크기
	AddMovementInput(GetActorRightVector(), Value);
}

