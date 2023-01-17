// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"
#include <Components/TextBlock.h>
#include <Components/ProgressBar.h>

void UMainUI::NativeConstruct() {
	Super::NativeConstruct();

	//text hp (��ġ) ��������
	currHP = Cast<UTextBlock>(GetWidgetFromName(TEXT("_currHP")));
	//progressbar hp ��������
	currHPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("_currHPBar")));
}

void UMainUI::UpdateCurrHP(float curr, float max) {
	//hp�� 0~1 ���̤Ǥ� ǥ��
	float hp = curr / max;
	//��ġ UI ����
	FString strHP = FString::Printf(TEXT("%d%%"), (int32)(hp*100));
	currHP->SetText(FText::FromString(strHP));
	//Progress bar UI ����
	currHPBar->SetPercent(hp);
}