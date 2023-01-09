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

// Sets default values
ATpsplayer::ATpsplayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	//BP_Bullet 클래스 가져오자
	ConstructorHelpers::FClassFinder<Abullet> tempBullet(TEXT("Blueprint'/Game/Blueprints/BP_Bullet.BP_Bullet_C'"));
	if (tempBullet.Succeeded()) {
		bulletFactory = tempBullet.Class;
	}
	//SniperUI 클래스를 가져온다
	ConstructorHelpers::FClassFinder<UUserWidget> tempUI(TEXT("WidgetBlueprint'/Game/Blueprints/SniperUI.SniperUI_C'"));
	if (tempUI.Succeeded()) {
		sniperUIFactory = tempUI.Class;
	}
	//스나이퍼 UI 만들기
	sniperUI = CreateWidget<UUserWidget>(GetWorld(), sniperUIFactory);

	//CommonUI 클래스를 가져온다
	ConstructorHelpers::FClassFinder<UUserWidget> tempCommonUI(TEXT("WidgetBlueprint'/Game/Blueprints/CommonUI.CommonUI_C'"));
	if (tempCommonUI.Succeeded()) {
		commonUIFactory = tempCommonUI.Class;
	}
	//CommonUI 만들기
	commonUI = CreateWidget<UUserWidget>(GetWorld(), commonUIFactory);

	//폭발 Effect를 가져온다
	ConstructorHelpers::FObjectFinder<UParticleSystem> tempExplode(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (tempExplode.Succeeded()) {
		explodeFactory = tempExplode.Object;
	}

	//애니메이션 블루프린트 가져오자
	ConstructorHelpers::FClassFinder<UABP_Player> tempAnim(TEXT("AnimBlueprint'/Game/Blueprints/ABP_Player.ABP_Player_C'"));
	if (tempAnim.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
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
	Super::BeginPlay();

	//카메라 상하 회전값 min, max 세팅
	APlayerController* playerController = Cast<APlayerController>(Controller);
	playerController->PlayerCameraManager->ViewPitchMin = -45;
	playerController->PlayerCameraManager->ViewPitchMax = 45;

	//점프 횟수를 2번한다
	JumpMaxCount = 2;
	/*bUseControllerRotationYaw = false;
	//compSpringArm->bUsePawnControlRotation = false;
	//compCamera->bUsePawnControlRotation = false;*/
	//움직이는 속력을 walkSpeed으로 변경
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	//점프하는 속력을 600으로 변경
	GetCharacterMovement()->JumpZVelocity = 600;
	//초기 총설정
	ChangeWeapon(false);
	//CommonUI를 보여준다
	commonUI->AddToViewport();
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

// Called every frame
void ATpsplayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//이동
	MoveAction();
	//회전
	//RotateAction();

	//1. 만약에 bFire가 true라면
	if (bFire) {
		//2. 현재시간을 흐르게 하고
		cameraCurrTime += DeltaTime;
		//3. 만약에 현재시간이 기준시간보다 작으면
		if (cameraCurrTime < cameraDelayTime) {
			//4. 카메라를 랜덤하게 위치시키자
			float randY = FMath::RandRange(-1.0, 1.0);
			float randZ = FMath::RandRange(-1.0, 1.0);
			compCamera->SetRelativeLocation(FVector(0, randY, randZ));
		}
		else {
			//5. 그렇지 앟으면 초기화(현재시간, bFire, 카메라 위치)
			cameraCurrTime = 0;
			bFire = false;
			compCamera->SetRelativeLocation(FVector(0));
		}
	}
}

void ATpsplayer::MoveAction() {
	direction = FTransform(GetControlRotation()).TransformVector(direction);
	AddMovementInput(direction);
	direction = FVector::ZeroVector;
}
void ATpsplayer::RotateAction() {
	//마우스 자와에 따라서 Actor를 회전 시키자
	SetActorRotation(FRotator(0, mx, 0));
	//마우스 상하에 따라서 SpringArm을 회전 시키자
	compSpringArm->SetRelativeRotation(FRotator(-my, 0, 0));
}

// Called to bind functionality to input
void ATpsplayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//마우스 좌우
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATpsplayer::Turn);
	//마우스 상하
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATpsplayer::LookUp);
	//A, D
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ATpsplayer::InputHorizontal);
	//W, S
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ATpsplayer::InputVertical);
	//점프
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ATpsplayer::InputJump);
	//총알 발사
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATpsplayer::InputFire);
	//델리게이트로 무기 변경
	DECLARE_DELEGATE_OneParam(FChangeWeapon, bool);
	PlayerInputComponent->BindAction<FChangeWeapon>(TEXT("Rifle"), IE_Pressed, this, &ATpsplayer::ChangeWeapon, false);
	PlayerInputComponent->BindAction<FChangeWeapon>(TEXT("Sniper"), IE_Pressed, this, &ATpsplayer::ChangeWeapon, true);
	////Rifle 1번키
	//PlayerInputComponent->BindAction(TEXT("Rifle"), IE_Pressed, this, &ATpsplayer::InputRifle);
	////Sniper 2번키
	//PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this, &ATpsplayer::InputSniper);

	//델리게이트로 스나이퍼줌
	DECLARE_DELEGATE_OneParam(FSniperZoom, bool);
	PlayerInputComponent->BindAction<FSniperZoom>(TEXT("Zoom"), IE_Pressed, this, &ATpsplayer::InputZoom, true);
	PlayerInputComponent->BindAction<FSniperZoom>(TEXT("Zoom"), IE_Released, this, &ATpsplayer::InputZoom, false);
	////스나이퍼 확대
	//PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Pressed, this, &ATpsplayer::InputZoom);
	////스나이퍼 축소
	//PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Released, this, &ATpsplayer::InputZoom);

	//플레이어 뛰어간다
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &ATpsplayer::InputRun);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &ATpsplayer::InputRun);
}

void ATpsplayer::Turn(float value) {
	AddControllerYawInput(value);
	//mx += value;
}
void ATpsplayer::LookUp(float value) {
	AddControllerPitchInput(value);
	/*my += value;
	my = FMath::Clamp(my, -45, 45);*/
}
void ATpsplayer::InputHorizontal(float value) {
	direction.Y = value;
}
void ATpsplayer::InputVertical(float value) {
	direction.X = value;
}
void ATpsplayer::InputJump() {
	Jump();
}
void ATpsplayer::InputRun() {
	UCharacterMovementComponent* compMove = GetCharacterMovement();
	//1. 만약에 MaxWalkSpeed 값이 walkSpeed 보다 크다면 (뛰고 있다면)
	if (compMove->MaxWalkSpeed > walkSpeed) {
		//2. MaxWalkSpeed 값을 walkSpeed로 하자
		compMove->MaxWalkSpeed = walkSpeed;
	}
	//3. 그렇지 않으면(걷고 있다면)
	else {
		//4. MaxWalkSpeed 값을 runSpeed로 하자
		GetCharacterMovement()->MaxWalkSpeed = runSpeed;
	}
}
void ATpsplayer::InputFire() {
	//APlayerController* controller = GetWorld()->GetFirstPlayerController();
	////카메라 흔들림을 멈추자
	//controller->PlayerCameraManager->StopAllCameraShakes();
	////카메라를 흔들자
	//controller->PlayerCameraManager->StartCameraShake(cameraShake);

	//총을 쐇다!!
	bFire = true;

	//총쏘는 애니메이션을 하자
	UABP_Player* playerAnim = Cast<UABP_Player>(GetMesh()->GetAnimInstance());
	playerAnim->PlayAttackAnim();
	//만약에 Rifle을 들고있는 상태라면 총알을 발사한다
	if (compRifle->IsVisible()) {
		//FirePos Transform을 가져오자
		FTransform trFire = compRifle->GetSocketTransform(TEXT("FirePos"));
		//총알생성 (위치, 회전 세팅)
		GetWorld()->SpawnActor<Abullet>(bulletFactory, trFire);
	}
	else {//Sniper가 보이는 상태
		//1. Line을 발사한다
		// LineTrace 의 시작 위치
		FVector startPos = compCamera->GetComponentLocation();
		// LineTrace 의 종료 위치
		//카메라위치 + 카메라 앞방향 * 거리
		FVector endPos = startPos + compCamera->GetForwardVector() * 5000;
		// LineTrace 의 충돌 정보를 담을 변수
		FHitResult hitInfo;
		// 충돌 옵션 설정 변수
		FCollisionQueryParams params;
		// 자기 자신(플레이어) 는 충돌에서 제외
		params.AddIgnoredActor(this);
		// Channel 필터를 이용한 LineTrace 충돌 검출(충돌 정보, 시작 위치, 종료 위치, 검출 채널, 충돌 옵션)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

		//2. 만약에 누군가 Line에 맞았다면
		if (bHit) {
			//3. 맞은 위치에 폭발 효과를 보여준다
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explodeFactory, hitInfo.ImpactPoint);
			//4. 만약에 맞은애가 isSimulatingPhysics가 활성화 되어 있다면
			if (hitInfo.GetComponent()->IsSimulatingPhysics()) {
				//목표물에 force를 계산한다
				FVector force = compCamera->GetForwardVector() * hitInfo.GetComponent()->GetMass() * 100000;
				//5. 맞은지점에 힘을 가한다.
				hitInfo.GetComponent()->AddForceAtLocation(force, hitInfo.ImpactPoint);
			}
			//UObject* enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("fsm"));
			AEnemy* enemy = Cast<AEnemy>(hitInfo.GetActor());
			if (enemy) {
				/*auto enemyFSM = Cast<UEnemyFSM>(enemy);
				enemyFSM->OnDamageProcess();*/
				enemy->fsm->ReceiveDamage();
			}
		}
	}
}

//void ATpsplayer::InputRifle() {
//	ChangeWeapon(false);
//}
//void ATpsplayer::InputSniper() {
//	ChangeWeapon(true);
//}

void ATpsplayer::ChangeWeapon(bool useSniper) {
	//만약에 useSniper가 false(Rifle로 무기 변경)
	if (useSniper == false && sniperUI->IsInViewport() == true) {
		//common UI를 보이게 한다
		InputZoom(false);
	}
	compSniper->SetVisibility(useSniper);
	compRifle->SetVisibility(!useSniper);
}

void ATpsplayer::InputZoom(bool sniperAim) {
	if (compSniper->IsVisible()) {
		if (sniperAim) {//스나이퍼 조준을 한다
			//만들어 놓은 UI를 Viewport에 붙이고
			sniperUI->AddToViewport();
			//카메라의 Field of View를 45로 설정
			compCamera->SetFieldOfView(45);
			//이미 Viewport에 있는 CommonUI를 떼어버린다
			commonUI->RemoveFromParent();
		}
		else {//스나이퍼 조준을 안한다
			//이미 Viewport에 있는 SniperUI를 떼어버린다
			sniperUI->RemoveFromParent();
			//카메라의 Fiedl of View를 원래 설정인 90으로 설정
			compCamera->SetFieldOfView(90);
			//기본 commonUI를 Viewport에 추가
			commonUI->AddToViewport();
		}
	}
}