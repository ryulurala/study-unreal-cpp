// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class TEST_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void UpDown(float Value);
	void LeftRight(float Value);
	void Yaw(float Value);

private:
	// class: 전방 선언

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;		// 셀카봉

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;		// 카메라

};
