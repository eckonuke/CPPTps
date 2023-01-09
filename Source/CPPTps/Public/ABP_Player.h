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
	//앞, 뒤를 판별하는 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float dirV = 0;
	//좌, 우를 판별하는 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float dirH = 0;

	//공중에 있는지 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAir = false; 
};
