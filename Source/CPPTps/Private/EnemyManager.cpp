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

	//���� �̸� 10�� ������
	for (int32 i = 0; i < 3; i++) {
		//� ��Ȳ�̴� ������ �����Ѵٴ� �ɼ�
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		//�� ����
		AEnemy* enemy = GetWorld()->SpawnActor<AEnemy>(enemyFactory, GetActorLocation(), GetActorRotation(), param);
		//���� ��Ȱ��ȭ ����
		enemy->SetActive(false);
		//������ ���� źâ(�迭)�� ����
		enemyArray.Add(enemy);
	}
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//���࿡ �� źâ ������ 0�̸� �Լ��� ������
	if(enemyArray.Num() == 0) return;

	//�ð��� �帣�� �Ѵ�
	currTime += DeltaTime;

	if (currTime > createTime) {
		//int32 rand = FMath::RandRange(0, spawnPosArray.Num() - 1);
		//������ yaw���� ���Ѵ�
		float angle = FMath::RandRange(0.0f, 359.0f);
		//������ ���� rotator�� ������ش�
		FRotator newRot = FRotator(0, angle, 0);
		//������ yaw���� spawnPos�� �����Ѵ�
		spawnPosArray[spawnIndex]->SetActorRotation(newRot);
		//������ �չ����� ���Ѵ� �ݰ� 0~500 �̳���
		FVector radius = FMath::RandRange(0.0f, 500.0f) * spawnPosArray[spawnIndex]->GetActorForwardVector();
		//���ο� ����� ���� spawnPos��ġ���� ���Ѵ�
		FVector newRandPos = radius + spawnPosArray[spawnIndex]->GetActorLocation();
		//��� ��ġ�� ������ ����
		//���ʹ̸� �����
		//GetWorld()->SpawnActor<AEnemy>(enemyFactory, newRandPos, spawnPosArray[spawnIndex]->GetActorRotation());

		//�� ��ġ ����
		enemyArray[0]->SetActorLocation(newRandPos);
		enemyArray[0]->SetActorRotation(newRot);
		//�� źâ���� 0��° �� Ȱ��ȭ �� ��ġ ����
		enemyArray[0]->SetActive(true);
		//enemyArray ���� ����
		enemyArray.RemoveAt(0);

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