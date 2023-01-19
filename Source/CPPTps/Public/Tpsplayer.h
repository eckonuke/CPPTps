// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Tpsplayer.generated.h"

UCLASS()
class CPPTPS_API ATpsplayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATpsplayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//플레이어 대미지 함수
	void ReceiveDamage(float damage);

	//Spring Arm Component
	UPROPERTY(EditAnywhere, Category = Camera)
	class USpringArmComponent* compSpringArm;
	//Camera Component
	UPROPERTY(EditAnywhere, Category = Camera)
	class UCameraComponent* compCamera;
	//Rifle 총
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* compRifle;
	//스나이퍼 총
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compSniper;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> cameraShake;
	UPROPERTY(EditAnywhere)
	class UPlayerMove* compPlayerMove;
	UPROPERTY(EditAnywhere)
	class UPlayerFire* compPlayerFire;

	//플레이의 기본 UI
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMainUI> mainUIFactory;
	UPROPERTY(EditAnywhere)
	class UMainUI* mainUI;
	//미니맵 블루프린트 클래스
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMinimap> miniFactory;
	//게임오버 블루프린트 클래스
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UGameOverUI> gameOverUIFactory;


	//현재 HP
	UPROPERTY(EditAnywhere)
	float currHP;
	//이전 HP
	UPROPERTY(EditAnywhere)
	float prevHP;
	//최대 HP
	UPROPERTY(EditAnywhere)
	float maxHP = 2;
	//HP UI를 갱신할 수 있니?
	UPROPERTY(EditAnywhere)
	bool bUpdateHP = false;
	//HP UI ratio
	UPROPERTY(EditAnywhere)
	float ratioHP = 0.0f;

};