// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class CPPTPS_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	// Called when the game starts
	virtual void NativeBeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEnemyState Estate;
	UPROPERTY(EditAnywhere)
	class AEnemy* enemy;
	UFUNCTION(BlueprintCallable)
	void AnimNotify_DieEnd();
	UFUNCTION(BlueprintCallable)
	void AnimNotify_EnemyAttack();
};
