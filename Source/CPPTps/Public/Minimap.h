// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SceneCapture2D.h"
#include "Minimap.generated.h"

/**
 *
 */
UCLASS()
class CPPTPS_API AMinimap : public ASceneCapture2D
{
	GENERATED_BODY()

public:
	AMinimap();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(EditAnywhere)
	class AActor* target;
};
