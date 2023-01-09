// Fill out your copyright notice in the Description page of Project Settings.


#include "bullet.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Engine/StaticMesh.h>
#include <GameFramework/ProjectileMovementComponent.h>

// Sets default values
Abullet::Abullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	compSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	compSphere->SetSphereRadius(13);
	SetRootComponent(compSphere);
	//모든것들과 충돌하게 한다
	compSphere->SetCollisionProfileName(TEXT("BlockAll"));

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	compMesh->SetupAttachment(compSphere);
	compMesh->SetRelativeLocation(FVector(0, 0, -13));
	compMesh->SetRelativeScale3D(FVector(0.26));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (tempMesh.Succeeded()) {
		compMesh->SetStaticMesh(tempMesh.Object);
	}
	//충돌은 sphere에서만 작동하도록
	compMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//Projectile Movement Component 세팅
	compProjectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	//Movement가 움직여야 할 component를 설정
	compProjectile->SetUpdatedComponent(compSphere);
	//초기속력
	compProjectile->InitialSpeed = 2000;
	//최대속력
	compProjectile->MaxSpeed = 2000;
	//반동여부(튕기는여부)
	compProjectile->bShouldBounce = true;
	//얼마나 잘 튕기게 할것인지 (0~1)
	compProjectile->Bounciness = 0.5f;
	
	//생명시간을 2초로 한다
	//InitialLifeSpan = 2;
}

// Called when the game starts or when spawned
void Abullet::BeginPlay()
{
	Super::BeginPlay();
	//Timer 설정하자
	FTimerHandle deathTimer;
	GetWorld()->GetTimerManager().SetTimer(deathTimer, this, &Abullet::Die, delayTime, false);
}

// Called every frame
void Abullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//SetActorLocation(GetActorLocation() + GetActorForwardVector() * speed * DeltaTime);

	////2초가 지나면
	//currentTime += DeltaTime;
	//if (currentTime >= delayTime) {
	//	//나를 파괴하자
	//	Destroy();
	//	currentTime = 0;
	//}
}

void Abullet::Die() {
	Destroy();
}