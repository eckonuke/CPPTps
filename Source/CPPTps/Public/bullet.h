// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "bullet.generated.h"

UCLASS()
class CPPTPS_API Abullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Abullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//
	void Die();

public:
	UPROPERTY(EditAnywhere)
	class USphereComponent* compSphere;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compMesh;
	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* compProjectile;

	float speed = 2000;

	float currentTime = 0;
	float delayTime = 2;

};