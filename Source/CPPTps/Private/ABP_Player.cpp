// Fill out your copyright notice in the Description page of Project Settings.


#include "ABP_Player.h"
#include "Tpsplayer.h"
#include <GameFramework/CharacterMovementComponent.h>

UABP_Player::UABP_Player() {
	//attack montage ��������
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("AnimMontage'/Game/Blueprints/AMT_Attack.AMT_Attack'"));
	if (tempMontage.Succeeded()) {
		attackAnimMontage = tempMontage.Object;
	}

}

void UABP_Player::NativeBeginPlay() {
	Super::NativeBeginPlay();
	//1. ���� �� ��������
	APawn* pawn = TryGetPawnOwner();
	//2. ATPSPlayer�� ĳ����
	player = Cast<ATpsplayer>(pawn);

}

void UABP_Player::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);
	//���࿡ player �� nullptr �ƴϸ�
	if (player != nullptr) {
		//3. �ӵ�, �չ��� ��������
		//FVector velocity = player->GetVelocity();
		//FVector forward = player->GetActorForwardVector();
		//FVector right = player->GetActorRightVector();
		//4. �ӵ��� �չ����� ���� �� dirV�� ����
		dirV = FVector::DotProduct(player->GetActorForwardVector(), player->GetVelocity());
		//5. �ӵ��� ������� ���� �� dirH�� ����
		dirH = FVector::DotProduct(player->GetActorRightVector(), player->GetVelocity());
		//�÷��̾ �ϴÿ��� ���������� Ȯ��
		bAir = player->GetCharacterMovement()->IsFalling();
	}
}

void UABP_Player::PlayAttackAnim() {
	Montage_Play(attackAnimMontage);
}