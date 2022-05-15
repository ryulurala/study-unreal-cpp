// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);		// GENERATE_BODY()에 Super 키워드 존재

	APawn* MyPawn = TryGetPawnOwner();
	if (IsValid(MyPawn))
	{
		Speed = MyPawn->GetVelocity().Size();

		ACharacter* MyCharacter = Cast<ACharacter>(MyPawn);
		if (MyCharacter)
		{
			IsFalling = MyCharacter->GetMovementComponent()->IsFalling();
		}
	}
}