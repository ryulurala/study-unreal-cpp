// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);		// GENERATE_BODY()에 Super 키워드 존재

	APawn* MyPawn = TryGetPawnOwner();
	if (IsValid(MyPawn))
	{
		Speed = MyPawn->GetVelocity().Size();
	}
}