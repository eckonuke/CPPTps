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

	//�÷��̾� ����� �Լ�
	void ReceiveDamage(float damage);

	//Spring Arm Component
	UPROPERTY(EditAnywhere, Category = Camera)
	class USpringArmComponent* compSpringArm;
	//Camera Component
	UPROPERTY(EditAnywhere, Category = Camera)
	class UCameraComponent* compCamera;
	//Rifle ��
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* compRifle;
	//�������� ��
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compSniper;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> cameraShake;
	UPROPERTY(EditAnywhere)
	class UPlayerMove* compPlayerMove;
	UPROPERTY(EditAnywhere)
	class UPlayerFire* compPlayerFire;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMainUI> mainUIFactory;
	UPROPERTY(EditAnywhere)
	class UMainUI* mainUI;

	//���� HP
	UPROPERTY(EditAnywhere)
	float currHP;
	//�ִ� HP
	UPROPERTY(EditAnywhere)
	float maxHP = 5;

};