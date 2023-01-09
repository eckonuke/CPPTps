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

	//BP_Bullet Ŭ���� ��������
	ConstructorHelpers::FClassFinder<Abullet> tempBullet(TEXT("Blueprint'/Game/Blueprints/BP_Bullet.BP_Bullet_C'"));
	if (tempBullet.Succeeded()) {
		bulletFactory = tempBullet.Class;
	}
	//SniperUI Ŭ������ �����´�
	ConstructorHelpers::FClassFinder<UUserWidget> tempUI(TEXT("WidgetBlueprint'/Game/Blueprints/SniperUI.SniperUI_C'"));
	if (tempUI.Succeeded()) {
		sniperUIFactory = tempUI.Class;
	}
	//�������� UI �����
	sniperUI = CreateWidget<UUserWidget>(GetWorld(), sniperUIFactory);

	//CommonUI Ŭ������ �����´�
	ConstructorHelpers::FClassFinder<UUserWidget> tempCommonUI(TEXT("WidgetBlueprint'/Game/Blueprints/CommonUI.CommonUI_C'"));
	if (tempCommonUI.Succeeded()) {
		commonUIFactory = tempCommonUI.Class;
	}
	//CommonUI �����
	commonUI = CreateWidget<UUserWidget>(GetWorld(), commonUIFactory);

	//���� Effect�� �����´�
	ConstructorHelpers::FObjectFinder<UParticleSystem> tempExplode(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (tempExplode.Succeeded()) {
		explodeFactory = tempExplode.Object;
	}

	//�ִϸ��̼� �������Ʈ ��������
	ConstructorHelpers::FClassFinder<UABP_Player> tempAnim(TEXT("AnimBlueprint'/Game/Blueprints/ABP_Player.ABP_Player_C'"));
	if (tempAnim.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
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
	Super::BeginPlay();

	//ī�޶� ���� ȸ���� min, max ����
	APlayerController* playerController = Cast<APlayerController>(Controller);
	playerController->PlayerCameraManager->ViewPitchMin = -45;
	playerController->PlayerCameraManager->ViewPitchMax = 45;

	//���� Ƚ���� 2���Ѵ�
	JumpMaxCount = 2;
	/*bUseControllerRotationYaw = false;
	//compSpringArm->bUsePawnControlRotation = false;
	//compCamera->bUsePawnControlRotation = false;*/
	//�����̴� �ӷ��� walkSpeed���� ����
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	//�����ϴ� �ӷ��� 600���� ����
	GetCharacterMovement()->JumpZVelocity = 600;
	//�ʱ� �Ѽ���
	ChangeWeapon(false);
	//CommonUI�� �����ش�
	commonUI->AddToViewport();
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

// Called every frame
void ATpsplayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//�̵�
	MoveAction();
	//ȸ��
	//RotateAction();

	//1. ���࿡ bFire�� true���
	if (bFire) {
		//2. ����ð��� �帣�� �ϰ�
		cameraCurrTime += DeltaTime;
		//3. ���࿡ ����ð��� ���ؽð����� ������
		if (cameraCurrTime < cameraDelayTime) {
			//4. ī�޶� �����ϰ� ��ġ��Ű��
			float randY = FMath::RandRange(-1.0, 1.0);
			float randZ = FMath::RandRange(-1.0, 1.0);
			compCamera->SetRelativeLocation(FVector(0, randY, randZ));
		}
		else {
			//5. �׷��� ������ �ʱ�ȭ(����ð�, bFire, ī�޶� ��ġ)
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
	//���콺 �ڿͿ� ���� Actor�� ȸ�� ��Ű��
	SetActorRotation(FRotator(0, mx, 0));
	//���콺 ���Ͽ� ���� SpringArm�� ȸ�� ��Ű��
	compSpringArm->SetRelativeRotation(FRotator(-my, 0, 0));
}

// Called to bind functionality to input
void ATpsplayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//���콺 �¿�
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATpsplayer::Turn);
	//���콺 ����
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATpsplayer::LookUp);
	//A, D
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ATpsplayer::InputHorizontal);
	//W, S
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ATpsplayer::InputVertical);
	//����
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ATpsplayer::InputJump);
	//�Ѿ� �߻�
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATpsplayer::InputFire);
	//��������Ʈ�� ���� ����
	DECLARE_DELEGATE_OneParam(FChangeWeapon, bool);
	PlayerInputComponent->BindAction<FChangeWeapon>(TEXT("Rifle"), IE_Pressed, this, &ATpsplayer::ChangeWeapon, false);
	PlayerInputComponent->BindAction<FChangeWeapon>(TEXT("Sniper"), IE_Pressed, this, &ATpsplayer::ChangeWeapon, true);
	////Rifle 1��Ű
	//PlayerInputComponent->BindAction(TEXT("Rifle"), IE_Pressed, this, &ATpsplayer::InputRifle);
	////Sniper 2��Ű
	//PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this, &ATpsplayer::InputSniper);

	//��������Ʈ�� ����������
	DECLARE_DELEGATE_OneParam(FSniperZoom, bool);
	PlayerInputComponent->BindAction<FSniperZoom>(TEXT("Zoom"), IE_Pressed, this, &ATpsplayer::InputZoom, true);
	PlayerInputComponent->BindAction<FSniperZoom>(TEXT("Zoom"), IE_Released, this, &ATpsplayer::InputZoom, false);
	////�������� Ȯ��
	//PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Pressed, this, &ATpsplayer::InputZoom);
	////�������� ���
	//PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Released, this, &ATpsplayer::InputZoom);

	//�÷��̾� �پ��
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
	//1. ���࿡ MaxWalkSpeed ���� walkSpeed ���� ũ�ٸ� (�ٰ� �ִٸ�)
	if (compMove->MaxWalkSpeed > walkSpeed) {
		//2. MaxWalkSpeed ���� walkSpeed�� ����
		compMove->MaxWalkSpeed = walkSpeed;
	}
	//3. �׷��� ������(�Ȱ� �ִٸ�)
	else {
		//4. MaxWalkSpeed ���� runSpeed�� ����
		GetCharacterMovement()->MaxWalkSpeed = runSpeed;
	}
}
void ATpsplayer::InputFire() {
	//APlayerController* controller = GetWorld()->GetFirstPlayerController();
	////ī�޶� ��鸲�� ������
	//controller->PlayerCameraManager->StopAllCameraShakes();
	////ī�޶� �����
	//controller->PlayerCameraManager->StartCameraShake(cameraShake);

	//���� �i��!!
	bFire = true;

	//�ѽ�� �ִϸ��̼��� ����
	UABP_Player* playerAnim = Cast<UABP_Player>(GetMesh()->GetAnimInstance());
	playerAnim->PlayAttackAnim();
	//���࿡ Rifle�� ����ִ� ���¶�� �Ѿ��� �߻��Ѵ�
	if (compRifle->IsVisible()) {
		//FirePos Transform�� ��������
		FTransform trFire = compRifle->GetSocketTransform(TEXT("FirePos"));
		//�Ѿ˻��� (��ġ, ȸ�� ����)
		GetWorld()->SpawnActor<Abullet>(bulletFactory, trFire);
	}
	else {//Sniper�� ���̴� ����
		//1. Line�� �߻��Ѵ�
		// LineTrace �� ���� ��ġ
		FVector startPos = compCamera->GetComponentLocation();
		// LineTrace �� ���� ��ġ
		//ī�޶���ġ + ī�޶� �չ��� * �Ÿ�
		FVector endPos = startPos + compCamera->GetForwardVector() * 5000;
		// LineTrace �� �浹 ������ ���� ����
		FHitResult hitInfo;
		// �浹 �ɼ� ���� ����
		FCollisionQueryParams params;
		// �ڱ� �ڽ�(�÷��̾�) �� �浹���� ����
		params.AddIgnoredActor(this);
		// Channel ���͸� �̿��� LineTrace �浹 ����(�浹 ����, ���� ��ġ, ���� ��ġ, ���� ä��, �浹 �ɼ�)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

		//2. ���࿡ ������ Line�� �¾Ҵٸ�
		if (bHit) {
			//3. ���� ��ġ�� ���� ȿ���� �����ش�
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explodeFactory, hitInfo.ImpactPoint);
			//4. ���࿡ �����ְ� isSimulatingPhysics�� Ȱ��ȭ �Ǿ� �ִٸ�
			if (hitInfo.GetComponent()->IsSimulatingPhysics()) {
				//��ǥ���� force�� ����Ѵ�
				FVector force = compCamera->GetForwardVector() * hitInfo.GetComponent()->GetMass() * 100000;
				//5. ���������� ���� ���Ѵ�.
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
	//���࿡ useSniper�� false(Rifle�� ���� ����)
	if (useSniper == false && sniperUI->IsInViewport() == true) {
		//common UI�� ���̰� �Ѵ�
		InputZoom(false);
	}
	compSniper->SetVisibility(useSniper);
	compRifle->SetVisibility(!useSniper);
}

void ATpsplayer::InputZoom(bool sniperAim) {
	if (compSniper->IsVisible()) {
		if (sniperAim) {//�������� ������ �Ѵ�
			//����� ���� UI�� Viewport�� ���̰�
			sniperUI->AddToViewport();
			//ī�޶��� Field of View�� 45�� ����
			compCamera->SetFieldOfView(45);
			//�̹� Viewport�� �ִ� CommonUI�� ���������
			commonUI->RemoveFromParent();
		}
		else {//�������� ������ ���Ѵ�
			//�̹� Viewport�� �ִ� SniperUI�� ���������
			sniperUI->RemoveFromParent();
			//ī�޶��� Fiedl of View�� ���� ������ 90���� ����
			compCamera->SetFieldOfView(90);
			//�⺻ commonUI�� Viewport�� �߰�
			commonUI->AddToViewport();
		}
	}
}