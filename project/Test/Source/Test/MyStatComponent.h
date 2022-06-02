// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEST_API UMyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMyStatComponent();

protected:
	virtual void InitializeComponent() override;	// 컴포넌트 초기화

public:	
	void SetLevel(int32 NewLevel);
	void OnAttacked(float DamageAmount);

	int32 GetLevel() const { return Level; }
	int32 GetHp() const { return Hp; }
	int32 GetAttack() const { return Attack; }

private:
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Level;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Hp;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Attack;

};
