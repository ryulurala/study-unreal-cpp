// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHelm.h"

// Sets default values
AMyHelm::AMyHelm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	Helm = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HELM"));
		
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_HELM(TEXT("StaticMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Skins/WhiteTiger/Meshes/SM_Greystone_TigerHelm.SM_Greystone_TigerHelm'"));
	if (SM_HELM.Succeeded())
	{
		Helm->SetStaticMesh(SM_HELM.Object);
	}

	Helm->SetCollisionProfileName(TEXT("NoCollision"));
}
