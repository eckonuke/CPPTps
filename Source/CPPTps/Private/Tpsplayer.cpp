// Fill out your copyright notice in the Description page of Project Settings.


#include "Tpsplayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "bullet.h"
#include <Blueprint/UserWidget.h>
#include <Particles/ParticleSystem.h>
#include <Kismet/GameplayStatics.h>
#include "Enemy.h"
#include "EnemyFSM.h"
#include <Components/CapsuleComponent.h>
#include "ABP_Player.h"
#include "Math/UnrealMathUtility.h"
#include "PlayerMove.h"
#include "PlayerFire.h"
#include "MainUI.h"
#include "Minimap.h"

// Sets default values
ATpsplayer::ATpsplayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SkeletalMeshComponent ����(Rifle)
	compRifle = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Rifle"));
	compRifle->SetupAttachment(GetMesh(), TEXT("WeaponPos"));
	compRifle->SetRelativeLocation(FVector(-17, 10, 0));
	compRifle->SetRelativeRotation(FRotator(0, 90, 0));
	//Skeletal Mesh ����
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempRifle(TEXT("SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (tempRifle.Succeeded()) {
		compRifle->SetSkeletalMesh(tempRifle.Object);
	}

	//StaticMesh Componenet ����(Sniper)
	compSniper = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sniper"));
	compSniper->SetupAttachment(GetMesh(), TEXT("WeaponPos"));
	compSniper->SetRelativeLocation(FVector(-42, 7, 1));
	compSniper->SetRelativeRotation(FRotator(0, 90, 0));
	compSniper->SetRelativeScale3D(FVector(0.15f));
	//Static Mesh ����
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSniper(TEXT("StaticMesh'/Game/SniperGun/sniper1.sniper1'"));
	if (tempSniper.Succeeded()) {
		compSniper->SetStaticMesh(tempSniper.Object);
	}

	//Mesh ��ġ�� ȸ���� ����
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	//����(Skeletal Mesh)�� �ε��ؼ� ��������
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (tempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
	}
	//Spring Arm ���� �� ����
	compSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	compSpringArm->SetupAttachment(RootComponent);
	compSpringArm->SetRelativeLocation(FVector(0, 70, 90));
	compSpringArm->TargetArmLength = 400;
	//Controller�� ȸ������ ���� �� �Ӽ� ����
	compSpringArm->bUsePawnControlRotation = true;

	//Camera Component ���� �� ����
	compCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	compCamera->SetupAttachment(compSpringArm);
	//Controller�� ȸ������ ���� �� �Ӽ� ����
	compCamera->bUsePawnControlRotation = true;

	//�ִϸ��̼� �������Ʈ ��������
	ConstructorHelpers::FClassFinder<UABP_Player> tempAnim(TEXT("AnimBlueprint'/Game/Blueprints/ABP_Player.ABP_Player_C'"));
	if (tempAnim.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
	}

	compPlayerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMovement"));
	compPlayerFire = CreateDefaultSubobject<UPlayerFire>(TEXT("PlayerFire"));

	//MainUI Ŭ���� ã�ƿ���
	ConstructorHelpers::FClassFinder<UMainUI> tempUI(TEXT("WidgetBlueprint'/Game/Blueprints/BP_MainUI.BP_MainUI_C'"));
	if (tempUI.Succeeded()) {
		mainUIFactory = tempUI.Class;
	}

	//MiniMap Ŭ���� ã�ƿ���
	ConstructorHelpers::FClassFinder<AMinimap> tempMini(TEXT("Blueprint'/Game/Blueprints/BP_Minimap.BP_Minimap_C'"));
	if (tempMini.Succeeded()) {
		miniFactory = tempMini.Class;
	}
	////ī�޶� ����ũ ��������
	//ConstructorHelpers::FClassFinder<UCameraShakeBase> tempShake(TEXT("Blueprint'/Game/Blueprints/BP_CameraShake.BP_CameraShake_C'"));
	//if (tempShake.Succeeded()) {
	//	cameraShake = tempShake.Class;
	//}
}

// Called when the game starts or when spawned
void ATpsplayer::BeginPlay()
{
	//ã�ƿ� Ŭ������ �̿��ؼ� MainUI ������
	mainUI = CreateWidget<UMainUI>(GetWorld(), mainUIFactory);
	mainUI->AddToViewport();
	Super::BeginPlay();
	//���� HP �ʱ�ȭ
	currHP = maxHP;
	prevHP = maxHP;

	//Minimap ��������
	GetWorld()->SpawnActor<AMinimap>(miniFactory, FVector(GetActorLocation().X, GetActorLocation().Y, 600), FRotator(-90, 0, 0));
}

// Called every frame
void ATpsplayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	prevHP = FMath::Lerp(prevHP, currHP, DeltaTime);
	mainUI->UpdateCurrHP(prevHP, maxHP);
	//���࿡ HP UI  �����ؾ� �Ѵٸ�
	//if (bUpdateHP) {
	//	ratioHP += (DeltaTime*2);
	//	//�������� ���������� �ʱ�ȭ
	//	if (ratioHP >= 1) {
	//		ratioHP = 1;
	//		bUpdateHP = false;
	//	}

	//	//HP UI�� ��������
	//	float hp = FMath::Lerp<float>(prevHP, currHP, ratioHP);
	//	mainUI->UpdateCurrHP(hp, maxHP);
	//}
}

// Called to bind functionality to input
void ATpsplayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//PlayerMoveŰ ���
	compPlayerMove->SetupInputBinding(PlayerInputComponent);
	compPlayerFire->SetupInputBinding(PlayerInputComponent);
}


void ATpsplayer::ReceiveDamage(float damage) {
	//���� HP�� �����Ѵ�
	prevHP = currHP;
	bUpdateHP = true;
	ratioHP = 0;

	//���� HP�� damage ��ŭ �ٿ��ش�
	currHP -= damage;
	//HP UI ����
	//mainUI->UpdateCurrHP(currHP, maxHP);

	//���࿡ HP�� 0���϶�� ���ӿ���(GameOver ���)
	if (currHP <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("Game Over!!!!"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Curr HP: %f"), currHP);
	}
	//�׷��� �ʴٸ� HP�� ���δ�
	//���� HP�� ���
}