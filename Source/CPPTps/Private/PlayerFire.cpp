// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFire.h"
#include <Blueprint/UserWidget.h>
#include "bullet.h"
#include <Particles/ParticleSystem.h>
#include <Kismet/GameplayStatics.h>
#include "ABP_Player.h"
#include "Tpsplayer.h"
#include "Enemy.h"
#include <Components/CapsuleComponent.h>
#include <Camera/CameraComponent.h>
#include "EnemyFSM.h"
#include <CollisionQueryParams.h>

UPlayerFire::UPlayerFire() {
	
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

	//CommonUI Ŭ������ �����´�
	ConstructorHelpers::FClassFinder<UUserWidget> tempCommonUI(TEXT("WidgetBlueprint'/Game/Blueprints/CommonUI.CommonUI_C'"));
	if (tempCommonUI.Succeeded()) {
		commonUIFactory = tempCommonUI.Class;
	}
	//CommonUI �����

	//���� Effect�� �����´�
	ConstructorHelpers::FObjectFinder<UParticleSystem> tempExplode(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (tempExplode.Succeeded()) {
		explodeFactory = tempExplode.Object;
	}
}

void UPlayerFire::BeginPlay() {
	Super::BeginPlay();

	//CommonUI�� ����
	commonUI = CreateWidget<UUserWidget>(GetWorld(), commonUIFactory);
	//SniperUI�� ����
	sniperUI = CreateWidget<UUserWidget>(GetWorld(), sniperUIFactory);
	//CommonUI�� �����ش�
	commonUI->AddToViewport();
	me->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	ChangeWeapon(true);
}

void UPlayerFire::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//1. ���࿡ bFire�� true���
	if (bFire) {
		//2. ����ð��� �帣�� �ϰ�
		cameraCurrTime += DeltaTime;
		//3. ���࿡ ����ð��� ���ؽð����� ������
		if (cameraCurrTime < cameraDelayTime) {
			//4. ī�޶� �����ϰ� ��ġ��Ű��
			float randY = FMath::RandRange(-1.0, 1.0);
			float randZ = FMath::RandRange(-1.0, 1.0);
			me->compCamera->SetRelativeLocation(FVector(0, randY, randZ));

		}
		else {
			//5. �׷��� ������ �ʱ�ȭ(����ð�, bFire, ī�޶� ��ġ)
			cameraCurrTime = 0;
			bFire = false;
			me->compCamera->SetRelativeLocation(FVector(0));
		}
	}
}

void UPlayerFire::SetupInputBinding(class UInputComponent* PlayerInputComponent) {
	Super::SetupInputBinding(PlayerInputComponent);
	//�Ѿ� �߻�
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &UPlayerFire::InputFire);
	//��������Ʈ�� ���� ����
	DECLARE_DELEGATE_OneParam(FChangeWeapon, bool);
	PlayerInputComponent->BindAction<FChangeWeapon>(TEXT("Rifle"), IE_Pressed, this, &UPlayerFire::ChangeWeapon, false);
	PlayerInputComponent->BindAction<FChangeWeapon>(TEXT("Sniper"), IE_Pressed, this, &UPlayerFire::ChangeWeapon, true);
	////Rifle 1��Ű
	//PlayerInputComponent->BindAction(TEXT("Rifle"), IE_Pressed, this, &ATpsplayer::InputRifle);
	////Sniper 2��Ű
	//PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this, &ATpsplayer::InputSniper);

	//��������Ʈ�� ����������
	DECLARE_DELEGATE_OneParam(FSniperZoom, bool);
	PlayerInputComponent->BindAction<FSniperZoom>(TEXT("Zoom"), IE_Pressed, this, &UPlayerFire::InputZoom, true);
	PlayerInputComponent->BindAction<FSniperZoom>(TEXT("Zoom"), IE_Released, this, &UPlayerFire::InputZoom, false);
	////�������� Ȯ��
	//PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Pressed, this, &ATpsplayer::InputZoom);
	////�������� ���
	//PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Released, this, &ATpsplayer::InputZoom);
}

void UPlayerFire::InputFire() {
	//APlayerController* controller = GetWorld()->GetFirstPlayerController();
	////ī�޶� ��鸲�� ������
	//controller->PlayerCameraManager->StopAllCameraShakes();
	////ī�޶� �����
	//controller->PlayerCameraManager->StartCameraShake(cameraShake);

	//���� �i��!!
	bFire = true;

	//�ѽ�� �ִϸ��̼��� ����
	UABP_Player* playerAnim = Cast<UABP_Player>(me->GetMesh()->GetAnimInstance());
	playerAnim->PlayAttackAnim();
	//���࿡ Rifle�� ����ִ� ���¶�� �Ѿ��� �߻��Ѵ�
	if (me->compRifle->IsVisible()) {
		//FirePos Transform�� ��������
		FTransform trFire = me->compRifle->GetSocketTransform(TEXT("FirePos"));
		//�Ѿ˻��� (��ġ, ȸ�� ����)
		GetWorld()->SpawnActor<Abullet>(bulletFactory, trFire);
	}
	else {//Sniper�� ���̴� ����
		//1. Line�� �߻��Ѵ�
		// LineTrace �� ���� ��ġ
		FVector startPos = me->compCamera->GetComponentLocation();
		// LineTrace �� ���� ��ġ
		//ī�޶���ġ + ī�޶� �չ��� * �Ÿ�
		FVector endPos = startPos + me->compCamera->GetForwardVector() * 5000;
		// LineTrace �� �浹 ������ ���� ����
		FHitResult hitInfo;
		// �浹 �ɼ� ���� ����
		FCollisionQueryParams params;
		// �ڱ� �ڽ�(�÷��̾�) �� �浹���� ����
		params.AddIgnoredActor(me);
		// Channel ���͸� �̿��� LineTrace �浹 ����(�浹 ����, ���� ��ġ, ���� ��ġ, ���� ä��, �浹 �ɼ�)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

		//2. ���࿡ ������ Line�� �¾Ҵٸ�
		if (bHit) {
			//3. ���� ��ġ�� ���� ȿ���� �����ش�
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explodeFactory, hitInfo.ImpactPoint);
			//4. ���࿡ �����ְ� isSimulatingPhysics�� Ȱ��ȭ �Ǿ� �ִٸ�
			if (hitInfo.GetComponent()->IsSimulatingPhysics()) {
				//��ǥ���� force�� ����Ѵ�
				FVector force = me->compCamera->GetForwardVector() * hitInfo.GetComponent()->GetMass() * 100000;
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

void UPlayerFire::ChangeWeapon(bool useSniper) {
	//���࿡ useSniper�� false(Rifle�� ���� ����)
	if (useSniper == false && sniperUI->IsInViewport() == true) {
		//common UI�� ���̰� �Ѵ�
		InputZoom(false);
	}
	me->compSniper->SetVisibility(useSniper);
	me->compRifle->SetVisibility(!useSniper);
}

void UPlayerFire::InputZoom(bool sniperAim) {
	if (me->compSniper->IsVisible()) {
		if (sniperAim) {//�������� ������ �Ѵ�
			//����� ���� UI�� Viewport�� ���̰�
			sniperUI->AddToViewport();
			//ī�޶��� Field of View�� 45�� ����
			me->compCamera->SetFieldOfView(45);
			//�̹� Viewport�� �ִ� CommonUI�� ���������
			commonUI->RemoveFromParent();
		}
		else {//�������� ������ ���Ѵ�
			//�̹� Viewport�� �ִ� SniperUI�� ���������
			sniperUI->RemoveFromParent();
			//ī�޶��� Fiedl of View�� ���� ������ 90���� ����
			me->compCamera->SetFieldOfView(90);
			//�⺻ commonUI�� Viewport�� �߰�
			commonUI->AddToViewport();
		}
	}
}