// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"
#include <Components/TextBlock.h>
#include <Components/ProgressBar.h>

void UMainUI::NativeConstruct() {
	Super::NativeConstruct();

	//text hp (수치) 가져오자
	currHP = Cast<UTextBlock>(GetWidgetFromName(TEXT("_currHP")));
	//progressbar hp 가져오자
	currHPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("_currHPBar")));
}

void UMainUI::UpdateCurrHP(float curr, float max) {
	//hp를 0~1 사이ㅗㄹ 표현
	float hp = curr / max;
	//수치 UI 갱신
	FString strHP = FString::Printf(TEXT("%d%%"), (int32)(hp*100));
	currHP->SetText(FText::FromString(strHP));
	//Progress bar UI 갱신
	currHPBar->SetPercent(hp);
}