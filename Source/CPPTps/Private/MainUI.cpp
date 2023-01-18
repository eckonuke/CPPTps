// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"
#include <Components/TextBlock.h>
#include <Components/ProgressBar.h>
#include <Components/CanvasPanelSlot.h>
#include <Components/Image.h>

void UMainUI::NativeConstruct() {
	Super::NativeConstruct();

	//text hp (��ġ) ��������
	currHP = Cast<UTextBlock>(GetWidgetFromName(TEXT("_currHP")));
	//progressbar hp ��������
	currHPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("_currHPBar")));

	//rifle �̹����� sniper �̹��� ��������
	rifle = Cast<UImage>(GetWidgetFromName(TEXT("_rifle")));
	sniper = Cast<UImage>(GetWidgetFromName(TEXT("_sniper")));
	
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

void UMainUI::UpdateWeapon(bool useSniper) {
	//���࿡ �������� �����
	SetZOrderWeapon(useSniper, sniper);
	SetZOrderWeapon(!useSniper, rifle);
}

void UMainUI::SetZOrderWeapon(bool bFocus, class UImage* img) {
	//CanvasPanelSlot�� �����´�
	UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(img->Slot);
	//���࿡ bFocus�� true
	if (bFocus) {
		//Zorder�� 2��
		slot->SetZOrder(2);
	}
	else {
		//�׷��� ������ Zorder�� 0����
		slot->SetZOrder(0);
	}
}