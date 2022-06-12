// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CloseTo.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "MyAIController.h"
#include "MyCharacter.h"


UBTDecorator_CloseTo::UBTDecorator_CloseTo()
{
	NodeName = TEXT("Close To");

	// Object Key만 넣을 수 있도록 필터링
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_CloseTo, BlackboardKey), AMyCharacter::StaticClass());
}

void UBTDecorator_CloseTo::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		BlackboardKey.ResolveSelectedKey(*BBAsset);
	}
}

bool UBTDecorator_CloseTo::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr)
		return false;

	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	if (MyBlackboard && BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		auto Target = Cast<AMyCharacter>(MyBlackboard->GetValueAsObject(BlackboardKey.SelectedKeyName));
		if (Target == nullptr)
			return false;

		bResult = Target->GetDistanceTo(CurrentPawn) <= Distance;
	}

	return bResult;
}
