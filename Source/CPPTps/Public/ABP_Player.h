// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ABP_Player.generated.h"

/**
 * 
 */
UCLASS()
class CPPTPS_API UABP_Player : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UABP_Player();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeBeginPlay() override;
	void PlayAttackAnim();

public:
	//Player Actor
	UPROPERTY(EditAnywhere)
	class ATpsplayer* player;

	//Montage
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* attackAnimMontage;
	//��, �ڸ� �Ǻ��ϴ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float dirV = 0;
	//��, �츦 �Ǻ��ϴ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float dirH = 0;

	//���߿� �ִ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAir = false; 
};
