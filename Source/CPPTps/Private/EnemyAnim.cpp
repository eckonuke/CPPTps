// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "Enemy.h"
#include "Tpsplayer.h"

void UEnemyAnim::NativeBeginPlay() {
	//1. 내가 붙어 있는 Pawn을 가져온다
	//TryGetPawnOwner(), GetOwningActor()
	//2. AEnemy로 형변환 한다.
	enemy = Cast<AEnemy>(TryGetPawnOwner());
}

void UEnemyAnim::AnimNotify_DieEnd() {
	//enemy가 가지고 있는 fsm을 이용해서 bDieMove를 true로 설정한다
	enemy->fsm->bDieMove = true;
	//Cast<AEnemy>(TryGetPawnOwner())->fsm->bDieMove = true;
}

void UEnemyAnim::AnimNotify_EnemyAttack() {
	//UE_LOG(LogTemp, Error, TEXT("Attack!!!!!!"));
	//플레이어에게 데미지를 주자
	enemy->fsm->target->ReceiveDamage(1);
}