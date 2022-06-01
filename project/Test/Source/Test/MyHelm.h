// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyHelm.generated.h"

UCLASS()
class TEST_API AMyHelm : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyHelm();

public:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Helm;
};
