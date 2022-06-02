// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

#include "MyData.h"
#include "Engine/DataTable.h"

UMyGameInstance::UMyGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> STAT_DATA(TEXT("DataTable'/Game/Data/StatTable.StatTable'"));

	//if(STAT_DATA.Succeeded())		// Data가 안 들어오면 Crash를 내는 게 더 적합

	MyStats = STAT_DATA.Object;

}

void UMyGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Warning, TEXT("MyGameInstance: %d"), GetStatData(1)->Attack);
}

FMyCharacterData* UMyGameInstance::GetStatData(int Level) const
{
	return MyStats->FindRow<FMyCharacterData>(*FString::FromInt(Level), TEXT(""));
}
