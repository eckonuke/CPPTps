// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverUI.generated.h"

/**
 *
 */
UCLASS()
class CPPTPS_API UGameOverUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* BtnRetry;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* BtnIdle;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* BtnHover;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* BtnClick;

public:
	//마우스가 버튼 위에 올라갔을 때
	UFUNCTION()
	void OnHover();
	//마우스가 버튼에서 벗어났을때
	UFUNCTION()
	void UnHover();
	//눌렀을 때
	UFUNCTION()
	void OnPress();
	
	//클릭했을때
	UFUNCTION()
	void OnClick();

};
