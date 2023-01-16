// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

UCLASS()
class CPPTPS_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//시간 변수
	UPROPERTY(EditAnywhere)
	float currTime = 0;
	UPROPERTY(EditAnywhere)
	float createTime = 0;
	//에너미 공장
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemy> enemyFactory;
	UPROPERTY(EditAnywhere)
	float randMin = 1.0f;
	UPROPERTY(EditAnywhere)
	float randMax = 5.0f;
	//적 생성 위치들
	UPROPERTY(EditAnywhere)
	TArray<AActor*> spawnPosArray;
	UPROPERTY(EditAnywhere)
	int32 spawnIndex = 0;
	//Enemy 탄창
	UPROPERTY(EditAnywhere)
	TArray<class AEnemy*> enemyArray;

public:
	void FindSpawnPos();

	UFUNCTION()
		void AddEnemy(class AEnemy* enem);
};