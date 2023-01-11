// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AEnemyManager::AEnemyManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//에너미 공장 설정하자
	ConstructorHelpers::FClassFinder<AEnemy> tempEnemy(TEXT("Blueprint'/Game/Blueprints/BP_Enemy.BP_Enemy_C'"));
	if (tempEnemy.Succeeded()) {
		enemyFactory = tempEnemy.Class;
	}

}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	//처음 생성시간 설정
	createTime = FMath::RandRange(randMin, randMax);
	FindSpawnPos();
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	currTime += DeltaTime;

	if (currTime > createTime) {
		//어느 위치에 만들지 고른다
		//int32 rand = FMath::RandRange(0, spawnPosArray.Num() - 1);
		//에너미를 만든다
		GetWorld()->SpawnActor<AEnemy>(enemyFactory, spawnPosArray[spawnIndex]->GetActorLocation(), spawnPosArray[spawnIndex]->GetActorRotation());
		currTime = 0;
		createTime = FMath::RandRange(randMin, randMax);
		spawnIndex++;
		spawnIndex %= spawnPosArray.Num();//조건문 없이 이렇게 사용할수 있다
		//if (spawnIndex == spawnPosArray.Num()) {
		//	spawnIndex = 0;
		//}
	}
}

void AEnemyManager::FindSpawnPos() {
	//스폰 액터를 찾아서 가져오자
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), actors);
	for (int32 i = 0; i < actors.Num(); i++) {
		//검색(이름이 SpawnPos가 포함되어 있는지)
		if (actors[i]->GetName().Contains(TEXT("SpawnPos")))
		{
			spawnPosArray.Add(actors[i]);//spawnPosArray에 추가한다
		}
	}
}