// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API AMyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMyAIController();

	virtual void OnPossess(APawn* InPawn) override;		// 빙의
	virtual void OnUnPossess() override;				// 빙의 해제
	
private:
	UPROPERTY()
	class UBehaviorTree* BehaviorTree;
	
	UPROPERTY()
	class UBlackboardData* BlackboardData;
};
