// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "Enemy.h"

void UEnemyAnim::AnimNotify_DieEnd() {
	//1. 내가 붙어 있는 Pawn을 가져온다
	//TryGetPawnOwner(), GetOwningActor()
	//2. AEnemy로 형변환 한다.
	AEnemy* enemy = Cast<AEnemy>(TryGetPawnOwner());
	//3. enemy가 가지고 있는 fsm을 이용해서 bDieMove를 true로 설정한다
	enemy->fsm->bDieMove = true;
	//Cast<AEnemy>(TryGetPawnOwner())->fsm->bDieMove = true;
}

void UEnemyAnim::AnimNotify_EnemyAttack() {
	UE_LOG(LogTemp, Error, TEXT("Attack!!!!!!"));
}