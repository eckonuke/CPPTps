// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerMove.generated.h"

/**
 *
 */
UCLASS()
class CPPTPS_API UPlayerMove : public UPlayerBaseComponent
{
	GENERATED_BODY()
public:
	UPlayerMove();


public:
	virtual void BeginPlay() override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;

	void MoveAction();
	void RotateAction();
	void Turn(float value);
	void LookUp(float value);
	void InputHorizontal(float value);
	void InputVertical(float value);
	void InputJump();
	void InputRun();

public:
	//걷기 속력
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
		float walkSpeed = 400;
	//뛰기 속력
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
		float runSpeed = 800;
	//이동방향
	FVector direction;
	//마우스 이동관련
	float mx = 0;
	float my = 0;
};
