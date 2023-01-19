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
	//���콺�� ��ư ���� �ö��� ��
	UFUNCTION()
	void OnHover();
	//���콺�� ��ư���� �������
	UFUNCTION()
	void UnHover();
	//������ ��
	UFUNCTION()
	void OnPress();
	
	//Ŭ��������
	UFUNCTION()
	void OnClick();

};
