// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHelm.h"

#include "MyCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
AMyHelm::AMyHelm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Helm = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HELM"));
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));	// Box Component 생성
		
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_HELM(TEXT("StaticMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Skins/WhiteTiger/Meshes/SM_Greystone_TigerHelm.SM_Greystone_TigerHelm'"));
	if (SM_HELM.Succeeded())
	{
		Helm->SetStaticMesh(SM_HELM.Object);
	}

	// 계층구조:
	// - Helm
	//   - Trigger
	Helm->SetupAttachment(RootComponent);
	Trigger->SetupAttachment(Helm);

	Helm->SetCollisionProfileName(TEXT("NoCollision"));
	Trigger->SetCollisionProfileName(TEXT("MyCollectible"));
	Trigger->SetBoxExtent(FVector(30.0f, 30.0f, 30.0f));
}

void AMyHelm::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMyHelm::OnCharacterOverlap);
}

void AMyHelm::OnCharacterOverlap(UPrimitiveComponent* OverlappedComop, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Overlapped"));

	AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
	if (MyCharacter)
	{
		FName HelmSocket(TEXT("Helm"));
		
		AttachToComponent(MyCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, HelmSocket);
	}
}
