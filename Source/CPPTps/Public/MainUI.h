// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

/**
 * 
 */
UCLASS()
class CPPTPS_API UMainUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
public:
	//UPROPERTY(meta = {})
	UPROPERTY(EditAnywhere)
	class UProgressBar* currHPBar;
	UPROPERTY(EditAnywhere)
	class UTextBlock* currHP;

public:
	void UpdateCurrHP(float curr, float max); 
};
