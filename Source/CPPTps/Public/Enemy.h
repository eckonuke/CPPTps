// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

DECLARE_DELEGATE_OneParam(FEnemyDieDelegate, class AEnemy*);

UCLASS()
class CPPTPS_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	class UEnemyFSM* fsm;
	UPROPERTY(EditAnywhere)
	float health = 5;

	//enemy�� Ȱ��/��Ȳ�� �ϴ� ���
	void SetActive(bool bActive);
	
	//�׾��� �� ȣ��Ǵ� �Լ��� ��� ����
	FEnemyDieDelegate dieDelegate;
};
