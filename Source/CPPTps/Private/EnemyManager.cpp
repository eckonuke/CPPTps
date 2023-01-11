// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AEnemyManager::AEnemyManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//���ʹ� ���� ��������
	ConstructorHelpers::FClassFinder<AEnemy> tempEnemy(TEXT("Blueprint'/Game/Blueprints/BP_Enemy.BP_Enemy_C'"));
	if (tempEnemy.Succeeded()) {
		enemyFactory = tempEnemy.Class;
	}

}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	//ó�� �����ð� ����
	createTime = FMath::RandRange(randMin, randMax);
	FindSpawnPos();
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	currTime += DeltaTime;

	if (currTime > createTime) {
		//��� ��ġ�� ������ ����
		//int32 rand = FMath::RandRange(0, spawnPosArray.Num() - 1);
		//���ʹ̸� �����
		GetWorld()->SpawnActor<AEnemy>(enemyFactory, spawnPosArray[spawnIndex]->GetActorLocation(), spawnPosArray[spawnIndex]->GetActorRotation());
		currTime = 0;
		createTime = FMath::RandRange(randMin, randMax);
		spawnIndex++;
		spawnIndex %= spawnPosArray.Num();//���ǹ� ���� �̷��� ����Ҽ� �ִ�
		//if (spawnIndex == spawnPosArray.Num()) {
		//	spawnIndex = 0;
		//}
	}
}

void AEnemyManager::FindSpawnPos() {
	//���� ���͸� ã�Ƽ� ��������
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), actors);
	for (int32 i = 0; i < actors.Num(); i++) {
		//�˻�(�̸��� SpawnPos�� ���ԵǾ� �ִ���)
		if (actors[i]->GetName().Contains(TEXT("SpawnPos")))
		{
			spawnPosArray.Add(actors[i]);//spawnPosArray�� �߰��Ѵ�
		}
	}
}