// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerFire.generated.h"

/**
 * 
 */
UCLASS()
class CPPTPS_API UPlayerFire : public UPlayerBaseComponent
{
	GENERATED_BODY()
public:
	UPlayerFire();

public:
	virtual void BeginPlay() override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;

public:
	
	//ÃÑ¾Ë »ý¼ºÀÚ
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

	bool bFire = false;
	float cameraDelayTime = 0.5f;
	float cameraCurrTime = 0;

public:
	void InputFire();
	//void InputRifle();
	//void InputSniper();
	void ChangeWeapon(bool useSniper);
	void InputZoom(bool sniperA);
};
