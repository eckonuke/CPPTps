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

	//적을 미리 10개 만들자
	for (int32 i = 0; i < 3; i++) {
		//어떤 상황이던 무조건 생성한다는 옵션
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		//적 생성
		AEnemy* enemy = GetWorld()->SpawnActor<AEnemy>(enemyFactory, GetActorLocation(), GetActorRotation(), param);
		//적을 비활성화 하자
		enemy->SetActive(false);
		//생성된 적을 탄창(배열)에 담자
		enemyArray.Add(enemy);
	}
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//만약에 적 탄창 갯수가 0이면 함수를 나가자
	if(enemyArray.Num() == 0) return;

	//시간을 흐르게 한다
	currTime += DeltaTime;

	if (currTime > createTime) {
		//int32 rand = FMath::RandRange(0, spawnPosArray.Num() - 1);
		//랜덤한 yaw값을 구한다
		float angle = FMath::RandRange(0.0f, 359.0f);
		//랜덤한 값을 rotator로 만들어준다
		FRotator newRot = FRotator(0, angle, 0);
		//랜덤한 yaw값을 spawnPos에 설정한다
		spawnPosArray[spawnIndex]->SetActorRotation(newRot);
		//랜덤한 앞방향을 구한다 반경 0~500 이내로
		FVector radius = FMath::RandRange(0.0f, 500.0f) * spawnPosArray[spawnIndex]->GetActorForwardVector();
		//새로운 방향과 현재 spawnPos위치값을 더한다
		FVector newRandPos = radius + spawnPosArray[spawnIndex]->GetActorLocation();
		//어느 위치에 만들지 고른다
		//에너미를 만든다
		//GetWorld()->SpawnActor<AEnemy>(enemyFactory, newRandPos, spawnPosArray[spawnIndex]->GetActorRotation());

		//적 위치 세팅
		enemyArray[0]->SetActorLocation(newRandPos);
		enemyArray[0]->SetActorRotation(newRot);
		//적 탄창에서 0번째 놈 활성화 및 위치 세팅
		enemyArray[0]->SetActive(true);
		//enemyArray 에서 빼자
		enemyArray.RemoveAt(0);

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