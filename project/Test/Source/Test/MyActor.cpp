// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// New
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));

	// Mesh를 로드
	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_CHAIR(TEXT("StaticMesh'/Game/StarterContent/Props/SM_Chair.SM_Chair'"));
	if (SM_CHAIR.Succeeded())
	{
		Mesh->SetStaticMesh(SM_CHAIR.Object);
	}
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
	//UE_LOG(LogTemp, Warning, TEXT("BeginPlay: %d"), 3);
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Error, TEXT("Tick: %f"), DeltaTime)

	// 인자: FRotator, Quaternion
	// FRotator(Pitch, Yaw, Roll)
	// Pitch(Y), Yaw(Z), Roll(X)
	//AddActorLocalRotation(FRotator(RotateSpeed * DeltaTime, 0.f,  0.f));	// Y축 기준 회전
	//AddActorLocalRotation(FRotator(0.f, RotateSpeed * DeltaTime, 0.f));		// Z축 기준 회전
	AddActorLocalRotation(FRotator(0.f, 0.f, RotateSpeed * DeltaTime));		// X축 기준 회전
}

