// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHpWidget.h"

#include "MyStatComponent.h"
#include "Components/ProgressBar.h"

void UMyHpWidget::BindHp(UMyStatComponent* StatComp)
{
	CurrentStatComp = StatComp;
	StatComp->OnHpChanged.AddUObject(this, &UMyHpWidget::UpdateHp);
}

void UMyHpWidget::UpdateHp()
{
	if (CurrentStatComp.IsValid())
		PB_HpBar->SetPercent(CurrentStatComp->GetHpRatio());
}
