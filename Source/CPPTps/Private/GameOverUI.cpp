// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverUI.h"
#include <Kismet/GameplayStatics.h>
#include <Components/Button.h>
#include "Tpsplayer.h"

void UGameOverUI::NativeConstruct() {
	Super::NativeConstruct();

	//C++�� �������Ʈ �и�
	AActor* player = UGameplayStatics::GetActorOfClass(GetWorld(), ATpsplayer::StaticClass());

	if (player != nullptr) {
		//���콺 ���̰� ����
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		//������ �Ͻ����� ����
		UGameplayStatics::SetGamePaused(GetWorld(), true);

		//��ư �̺�Ʈ�� ȣ��Ǵ� �Լ� ���
		BtnRetry->OnHovered.AddDynamic(this, &UGameOverUI::OnHover);
		BtnRetry->OnUnhovered.AddDynamic(this, &UGameOverUI::UnHover);
		BtnRetry->OnPressed.AddDynamic(this, &UGameOverUI::OnPress);
		BtnRetry->OnClicked.AddDynamic(this, &UGameOverUI::OnClick);

		//BtnIdle anim ����
		PlayAnimation(BtnIdle, 0, 0);
	}
}

void UGameOverUI::OnHover() {
	//BtnIdle anim ����
	StopAnimation(BtnIdle);
	//BtnHover anim ����
	PlayAnimation(BtnHover, 0, 1);
}

void UGameOverUI::UnHover() {
	//BtnHover anim �Ųٷ� ����
	PlayAnimation(BtnHover, 0, 1, EUMGSequencePlayMode::Reverse);
	//BtnIdle anim ����
	PlayAnimation(BtnIdle, 0, 0);
}

void UGameOverUI::OnPress() {
	//�������� ����
	PlayAnimation(BtnClick, 0, 1);
}

void UGameOverUI::OnClick() {
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("TpsMap"));
}