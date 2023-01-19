// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverUI.h"
#include <Kismet/GameplayStatics.h>
#include <Components/Button.h>
#include "Tpsplayer.h"

void UGameOverUI::NativeConstruct() {
	Super::NativeConstruct();

	//C++와 블루프린트 분리
	AActor* player = UGameplayStatics::GetActorOfClass(GetWorld(), ATpsplayer::StaticClass());

	if (player != nullptr) {
		//마우스 보이게 하자
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		//게임을 일시정지 하자
		UGameplayStatics::SetGamePaused(GetWorld(), true);

		//버튼 이벤트에 호출되는 함수 등록
		BtnRetry->OnHovered.AddDynamic(this, &UGameOverUI::OnHover);
		BtnRetry->OnUnhovered.AddDynamic(this, &UGameOverUI::UnHover);
		BtnRetry->OnPressed.AddDynamic(this, &UGameOverUI::OnPress);
		BtnRetry->OnClicked.AddDynamic(this, &UGameOverUI::OnClick);

		//BtnIdle anim 실행
		PlayAnimation(BtnIdle, 0, 0);
	}
}

void UGameOverUI::OnHover() {
	//BtnIdle anim 멈춤
	StopAnimation(BtnIdle);
	//BtnHover anim 실행
	PlayAnimation(BtnHover, 0, 1);
}

void UGameOverUI::UnHover() {
	//BtnHover anim 거꾸로 실행
	PlayAnimation(BtnHover, 0, 1, EUMGSequencePlayMode::Reverse);
	//BtnIdle anim 실행
	PlayAnimation(BtnIdle, 0, 0);
}

void UGameOverUI::OnPress() {
	//눌렀을때 실행
	PlayAnimation(BtnClick, 0, 1);
}

void UGameOverUI::OnClick() {
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("TpsMap"));
}