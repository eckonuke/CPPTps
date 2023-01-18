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

	//SkeletalMeshComponent 세팅(Rifle)
	compRifle = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Rifle"));
	compRifle->SetupAttachment(GetMesh(), TEXT("WeaponPos"));
	compRifle->SetRelativeLocation(FVector(-17, 10, 0));
	compRifle->SetRelativeRotation(FRotator(0, 90, 0));
	//Skeletal Mesh 세팅
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempRifle(TEXT("SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (tempRifle.Succeeded()) {
		compRifle->SetSkeletalMesh(tempRifle.Object);
	}

	//StaticMesh Componenet 셋팅(Sniper)
	compSniper = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sniper"));
	compSniper->SetupAttachment(GetMesh(), TEXT("WeaponPos"));
	compSniper->SetRelativeLocation(FVector(-42, 7, 1));
	compSniper->SetRelativeRotation(FRotator(0, 90, 0));
	compSniper->SetRelativeScale3D(FVector(0.15f));
	//Static Mesh 세팅
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSniper(TEXT("StaticMesh'/Game/SniperGun/sniper1.sniper1'"));
	if (tempSniper.Succeeded()) {
		compSniper->SetStaticMesh(tempSniper.Object);
	}

	//Mesh 위치와 회전을 세팅
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	//외형(Skeletal Mesh)를 로드해서 세팅하자
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (tempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
	}
	//Spring Arm 생성 및 설정
	compSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	compSpringArm->SetupAttachment(RootComponent);
	compSpringArm->SetRelativeLocation(FVector(0, 70, 90));
	compSpringArm->TargetArmLength = 400;
	//Controller의 회전값을 따라 갈 속성 세팅
	compSpringArm->bUsePawnControlRotation = true;

	//Camera Component 생성 및 설정
	compCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	compCamera->SetupAttachment(compSpringArm);
	//Controller의 회전값을 따라 갈 속성 세팅
	compCamera->bUsePawnControlRotation = true;

	//애니메이션 블루프린트 가져오자
	ConstructorHelpers::FClassFinder<UABP_Player> tempAnim(TEXT("AnimBlueprint'/Game/Blueprints/ABP_Player.ABP_Player_C'"));
	if (tempAnim.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
	}

	compPlayerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMovement"));
	compPlayerFire = CreateDefaultSubobject<UPlayerFire>(TEXT("PlayerFire"));

	//MainUI 클래스 찾아오자
	ConstructorHelpers::FClassFinder<UMainUI> tempUI(TEXT("WidgetBlueprint'/Game/Blueprints/BP_MainUI.BP_MainUI_C'"));
	if (tempUI.Succeeded()) {
		mainUIFactory = tempUI.Class;
	}

	//MiniMap 클래스 찾아오자
	ConstructorHelpers::FClassFinder<AMinimap> tempMini(TEXT("Blueprint'/Game/Blueprints/BP_Minimap.BP_Minimap_C'"));
	if (tempMini.Succeeded()) {
		miniFactory = tempMini.Class;
	}
	////카메라 쉐이크 가져오자
	//ConstructorHelpers::FClassFinder<UCameraShakeBase> tempShake(TEXT("Blueprint'/Game/Blueprints/BP_CameraShake.BP_CameraShake_C'"));
	//if (tempShake.Succeeded()) {
	//	cameraShake = tempShake.Class;
	//}
}

// Called when the game starts or when spawned
void ATpsplayer::BeginPlay()
{
	//찾아온 클래스를 이용해서 MainUI 만들자
	mainUI = CreateWidget<UMainUI>(GetWorld(), mainUIFactory);
	mainUI->AddToViewport();
	Super::BeginPlay();
	//현재 HP 초기화
	currHP = maxHP;
	prevHP = maxHP;

	//Minimap 생성하자
	GetWorld()->SpawnActor<AMinimap>(miniFactory, FVector(GetActorLocation().X, GetActorLocation().Y, 600), FRotator(-90, 0, 0));
}

// Called every frame
void ATpsplayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	prevHP = FMath::Lerp(prevHP, currHP, DeltaTime);
	mainUI->UpdateCurrHP(prevHP, maxHP);
	//만약에 HP UI  갱신해야 한다면
	//if (bUpdateHP) {
	//	ratioHP += (DeltaTime*2);
	//	//목적지에 도달했으면 초기화
	//	if (ratioHP >= 1) {
	//		ratioHP = 1;
	//		bUpdateHP = false;
	//	}

	//	//HP UI를 갱신하자
	//	float hp = FMath::Lerp<float>(prevHP, currHP, ratioHP);
	//	mainUI->UpdateCurrHP(hp, maxHP);
	//}
}

// Called to bind functionality to input
void ATpsplayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//PlayerMove키 등록
	compPlayerMove->SetupInputBinding(PlayerInputComponent);
	compPlayerFire->SetupInputBinding(PlayerInputComponent);
}


void ATpsplayer::ReceiveDamage(float damage) {
	//이전 HP를 저장한다
	prevHP = currHP;
	bUpdateHP = true;
	ratioHP = 0;

	//현재 HP를 damage 만큼 줄여준다
	currHP -= damage;
	//HP UI 갱신
	//mainUI->UpdateCurrHP(currHP, maxHP);

	//만약에 HP가 0이하라면 게임오버(GameOver 출력)
	if (currHP <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("Game Over!!!!"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Curr HP: %f"), currHP);
	}
	//그렇지 않다면 HP를 줄인다
	//현재 HP를 출력
}