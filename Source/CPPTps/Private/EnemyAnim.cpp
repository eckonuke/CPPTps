// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "Enemy.h"

void UEnemyAnim::AnimNotify_DieEnd() {
	//1. ���� �پ� �ִ� Pawn�� �����´�
	//TryGetPawnOwner(), GetOwningActor()
	//2. AEnemy�� ����ȯ �Ѵ�.
	AEnemy* enemy = Cast<AEnemy>(TryGetPawnOwner());
	//3. enemy�� ������ �ִ� fsm�� �̿��ؼ� bDieMove�� true�� �����Ѵ�
	enemy->fsm->bDieMove = true;
	//Cast<AEnemy>(TryGetPawnOwner())->fsm->bDieMove = true;
}

void UEnemyAnim::AnimNotify_EnemyAttack() {
	UE_LOG(LogTemp, Error, TEXT("Attack!!!!!!"));
}