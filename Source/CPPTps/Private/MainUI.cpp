// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"
#include <Components/TextBlock.h>
#include <Components/ProgressBar.h>
#include <Components/CanvasPanelSlot.h>
#include <Components/Image.h>

void UMainUI::NativeConstruct() {
	Super::NativeConstruct();

	//text hp (수치) 가져오자
	currHP = Cast<UTextBlock>(GetWidgetFromName(TEXT("_currHP")));
	//progressbar hp 가져오자
	currHPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("_currHPBar")));

	//rifle 이미지와 sniper 이미지 가져오자
	rifle = Cast<UImage>(GetWidgetFromName(TEXT("_rifle")));
	sniper = Cast<UImage>(GetWidgetFromName(TEXT("_sniper")));
	
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

void UMainUI::UpdateWeapon(bool useSniper) {
	//만약에 스나이퍼 모드라면
	SetZOrderWeapon(useSniper, sniper);
	SetZOrderWeapon(!useSniper, rifle);
}

void UMainUI::SetZOrderWeapon(bool bFocus, class UImage* img) {
	//CanvasPanelSlot을 가져온다
	UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(img->Slot);
	//만약에 bFocus가 true
	if (bFocus) {
		//Zorder를 2로
		slot->SetZOrder(2);
	}
	else {
		//그렇지 않으면 Zorder를 0으로
		slot->SetZOrder(0);
	}
}