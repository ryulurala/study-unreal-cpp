// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPatrolPos.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UBTTask_FindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FindPatrolPos();
	
	// Blackboard Key Selector 초기화(중요!)
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	// Task 시작할 때 실행할 함수: BeginPlay()와 비슷
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(Category = Distance, EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
		float SphereRadius;
	
	// Patrol Pos Vector 대입 가능한 BlackboardKey
	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector BlackboardKey;
};
