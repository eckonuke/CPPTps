// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include <Engine/SkeletalMesh.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>
#include "EnemyAnim.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Mesh의 위치를 셋팅
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	//Mesh의 회전을 셋팅
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	//Mesh의 외형 셋팅
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Enemy/Model/vampire_a_lusth.vampire_a_lusth'"));
	if (tempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
	}

	//FSM 컴포넌트 추가
	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));
	//Enemy Collision 셋팅
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	//AnimInstance 세팅
	ConstructorHelpers::FClassFinder<UEnemyAnim> tempAnim(TEXT("AnimBlueprint'/Game/Blueprints/ABP_Enemy.ABP_Enemy_C'"));
	if (tempAnim.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
	}

	//auto possess ai 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	//움직이는 방향으로 몸을 자동으로 회전하라는 옵션
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::SetActive(bool bActive) {
	//활성화
	if (bActive) {
		//충돌 활성
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//생성 위치 재설정
		fsm->originPos = GetActorLocation();
	}
	//비활성화
	else {
		//충돌 비활성 설정
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//탄창에 날 다시 넣어줘
		dieDelegate.ExecuteIfBound(this);
	}
	//메쉬를 비활성화
	GetMesh()->SetActive(bActive);
	//안보이게 설정
	GetMesh()->SetVisibility(bActive);
	//Character Movement 활성 / 비활성
	GetCharacterMovement()->SetActive(bActive);
	//FSM 활성 / 비활성
	fsm->SetActive(bActive);
}