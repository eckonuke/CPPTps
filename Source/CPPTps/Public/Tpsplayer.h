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
	//�Ѿ� ������
	UPROPERTY(EditAnywhere)
	TSubclassOf<class Abullet> bulletFactory;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> sniperUIFactory;
	UPROPERTY(EditAnywhere)
	class UUserWidget* sniperUI;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> commonUIFactory;
	UPROPERTY(EditAnywhere)
	class UUserWidget* commonUI;
	UPROPERTY(EditAnywhere)
	class UParticleSystem* explodeFactory;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> cameraShake;


	//�ȱ� �ӷ�
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float walkSpeed = 400;
	//�ٱ� �ӷ�
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float runSpeed = 800;
	//�̵�����
	FVector direction;

	bool bFire = false;
	float cameraDelayTime = 0.5f;
	float cameraCurrTime = 0;

	//���콺 �̵�����
	float mx = 0;
	float my = 0;

	void MoveAction();
	void RotateAction();

	void Turn(float value);
	void LookUp(float value);
	void InputHorizontal(float value);
	void InputVertical(float value);
	void InputJump();
	void InputRun();
	void InputFire();
	//void InputRifle();
	//void InputSniper();
	void ChangeWeapon(bool useSniper);
	void InputZoom(bool sniperA);
};