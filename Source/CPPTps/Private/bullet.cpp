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
	//���͵�� �浹�ϰ� �Ѵ�
	compSphere->SetCollisionProfileName(TEXT("BlockAll"));

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	compMesh->SetupAttachment(compSphere);
	compMesh->SetRelativeLocation(FVector(0, 0, -13));
	compMesh->SetRelativeScale3D(FVector(0.26));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (tempMesh.Succeeded()) {
		compMesh->SetStaticMesh(tempMesh.Object);
	}
	//�浹�� sphere������ �۵��ϵ���
	compMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//Projectile Movement Component ����
	compProjectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	//Movement�� �������� �� component�� ����
	compProjectile->SetUpdatedComponent(compSphere);
	//�ʱ�ӷ�
	compProjectile->InitialSpeed = 2000;
	//�ִ�ӷ�
	compProjectile->MaxSpeed = 2000;
	//�ݵ�����(ƨ��¿���)
	compProjectile->bShouldBounce = true;
	//�󸶳� �� ƨ��� �Ұ����� (0~1)
	compProjectile->Bounciness = 0.5f;
	
	//����ð��� 2�ʷ� �Ѵ�
	//InitialLifeSpan = 2;
}

// Called when the game starts or when spawned
void Abullet::BeginPlay()
{
	Super::BeginPlay();
	//Timer ��������
	FTimerHandle deathTimer;
	GetWorld()->GetTimerManager().SetTimer(deathTimer, this, &Abullet::Die, delayTime, false);
}

// Called every frame
void Abullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//SetActorLocation(GetActorLocation() + GetActorForwardVector() * speed * DeltaTime);

	////2�ʰ� ������
	//currentTime += DeltaTime;
	//if (currentTime >= delayTime) {
	//	//���� �ı�����
	//	Destroy();
	//	currentTime = 0;
	//}
}

void Abullet::Die() {
	Destroy();
}