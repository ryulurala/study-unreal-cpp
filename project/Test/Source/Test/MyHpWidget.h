// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyHpWidget.generated.h"

/**
 * 
 */

class UMyStatComponent;

UCLASS()
class TEST_API UMyHpWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindHp(UMyStatComponent* StatComp);

	void UpdateHp();

private:
	// 수명주기가 다른 컴포넌트를 들 때는 혹시 모르니 WeakPtr를 사용
	TWeakObjectPtr<UMyStatComponent> CurrentStatComp;

	UPROPERTY(meta=(BindWidget))	// 이름 바인딩
	class UProgressBar* PB_HpBar;

	// 이름이 달라도 바인딩 가능
	// PB_HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HpBar")))
};
