// Fill out your copyright notice in the Description page of Project Settings.


#include "ABP_Player.h"
#include "Tpsplayer.h"
#include <GameFramework/CharacterMovementComponent.h>

UABP_Player::UABP_Player() {
	//attack montage 가져오자
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("AnimMontage'/Game/Blueprints/AMT_Attack.AMT_Attack'"));
	if (tempMontage.Succeeded()) {
		attackAnimMontage = tempMontage.Object;
	}

}

void UABP_Player::NativeBeginPlay() {
	Super::NativeBeginPlay();
	//1. 소유 폰 가져오기
	APawn* pawn = TryGetPawnOwner();
	//2. ATPSPlayer로 캐스팅
	player = Cast<ATpsplayer>(pawn);

}

void UABP_Player::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);
	//만약에 player 가 nullptr 아니면
	if (player != nullptr) {
		//3. 속도, 앞방향 가져오기
		//FVector velocity = player->GetVelocity();
		//FVector forward = player->GetActorForwardVector();
		//FVector right = player->GetActorRightVector();
		//4. 속도와 앞방향의 내적 값 dirV에 세팅
		dirV = FVector::DotProduct(player->GetActorForwardVector(), player->GetVelocity());
		//5. 속도와 우방향의 내적 값 dirH에 세팅
		dirH = FVector::DotProduct(player->GetActorRightVector(), player->GetVelocity());
		//플레이어가 하늘에서 내려오는지 확인
		bAir = player->GetCharacterMovement()->IsFalling();
	}
}

void UABP_Player::PlayAttackAnim() {
	Montage_Play(attackAnimMontage);
}