// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "Enemy.h"
#include "Tpsplayer.h"

void UEnemyAnim::NativeBeginPlay() {
	//1. ���� �پ� �ִ� Pawn�� �����´�
	//TryGetPawnOwner(), GetOwningActor()
	//2. AEnemy�� ����ȯ �Ѵ�.
	enemy = Cast<AEnemy>(TryGetPawnOwner());
}

void UEnemyAnim::AnimNotify_DieEnd() {
	//enemy�� ������ �ִ� fsm�� �̿��ؼ� bDieMove�� true�� �����Ѵ�
	enemy->fsm->bDieMove = true;
	//Cast<AEnemy>(TryGetPawnOwner())->fsm->bDieMove = true;
}

void UEnemyAnim::AnimNotify_EnemyAttack() {
	//UE_LOG(LogTemp, Error, TEXT("Attack!!!!!!"));
	//�÷��̾�� �������� ����
	enemy->fsm->target->ReceiveDamage(1);
}