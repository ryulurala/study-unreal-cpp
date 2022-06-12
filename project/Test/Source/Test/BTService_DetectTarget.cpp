// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DetectTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "MyAIController.h"
#include "MyCharacter.h"
#include "DrawDebugHelpers.h"


UBTService_DetectTarget::UBTService_DetectTarget()
{
	NodeName = TEXT("Detect Target");
	Interval = 1.0f;

	// Object Key만 넣을 수 있도록 필터링
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_DetectTarget, BlackboardKey), AMyCharacter::StaticClass());
}

void UBTService_DetectTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		BlackboardKey.ResolveSelectedKey(*BBAsset);
	}
}

void UBTService_DetectTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr)
		return;

	UWorld* World = CurrentPawn->GetWorld();
	if (World == nullptr)
		return;

	FVector Center = CurrentPawn->GetActorLocation();
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(NAME_None, false, CurrentPawn);	// IgnoreActor: CurrentPawn

	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		QueryParams
	);

	if (bResult)
	{
		for (auto& OverlapResult : OverlapResults)
		{
			AMyCharacter* MyCharacter = Cast<AMyCharacter>(OverlapResult.GetActor());
			if (MyCharacter && MyCharacter->GetController()->IsPlayerController())
			{
				UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
				if (MyBlackboard && BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
				{
					MyBlackboard->SetValueAsObject(BlackboardKey.SelectedKeyName, MyCharacter);

					DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
				}
			}
		}
	}
	else
	{
		UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
		if (MyBlackboard && BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
		{
			MyBlackboard->SetValueAsObject(BlackboardKey.SelectedKeyName, nullptr);

			DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
		}
	}
}
