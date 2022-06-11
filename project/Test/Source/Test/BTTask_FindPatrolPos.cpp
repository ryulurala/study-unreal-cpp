// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	// Behavior Tree에서 해당 Task를 만들면 나오는 노드 이름
	NodeName = TEXT("FindPatrolPos");

	// Vector Key만 넣을 수 있도록 필터링
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindPatrolPos, BlackboardKey));
}

void UBTTask_FindPatrolPos::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		BlackboardKey.ResolveSelectedKey(*BBAsset);
	}
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get Navi-Mesh
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr)
		return EBTNodeResult::Failed;

	FNavLocation RandomLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, SphereRadius, RandomLocation))
	{
		UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
		if (MyBlackboard && BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
		{
			MyBlackboard->SetValueAsVector(BlackboardKey.SelectedKeyName, RandomLocation.Location);
		}
		
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
