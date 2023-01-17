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

	//CommonUI 클래스를 가져온다
	ConstructorHelpers::FClassFinder<UUserWidget> tempCommonUI(TEXT("WidgetBlueprint'/Game/Blueprints/CommonUI.CommonUI_C'"));
	if (tempCommonUI.Succeeded()) {
		commonUIFactory = tempCommonUI.Class;
	}
	//CommonUI 만들기

	//폭발 Effect를 가져온다
	ConstructorHelpers::FObjectFinder<UParticleSystem> tempExplode(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (tempExplode.Succeeded()) {
		explodeFactory = tempExplode.Object;
	}
}

void UPlayerFire::BeginPlay() {
	Super::BeginPlay();

	//CommonUI를 생성
	commonUI = CreateWidget<UUserWidget>(GetWorld(), commonUIFactory);
	//SniperUI를 생성
	sniperUI = CreateWidget<UUserWidget>(GetWorld(), sniperUIFactory);
	//CommonUI를 보여준다
	commonUI->AddToViewport();
	me->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	ChangeWeapon(true);
}

void UPlayerFire::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//1. 만약에 bFire가 true라면
	if (bFire) {
		//2. 현재시간을 흐르게 하고
		cameraCurrTime += DeltaTime;
		//3. 만약에 현재시간이 기준시간보다 작으면
		if (cameraCurrTime < cameraDelayTime) {
			//4. 카메라를 랜덤하게 위치시키자
			float randY = FMath::RandRange(-1.0, 1.0);
			float randZ = FMath::RandRange(-1.0, 1.0);
			me->compCamera->SetRelativeLocation(FVector(0, randY, randZ));

		}
		else {
			//5. 그렇지 앟으면 초기화(현재시간, bFire, 카메라 위치)
			cameraCurrTime = 0;
			bFire = false;
			me->compCamera->SetRelativeLocation(FVector(0));
		}
	}
}

void UPlayerFire::SetupInputBinding(class UInputComponent* PlayerInputComponent) {
	Super::SetupInputBinding(PlayerInputComponent);
	//총알 발사
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &UPlayerFire::InputFire);
	//델리게이트로 무기 변경
	DECLARE_DELEGATE_OneParam(FChangeWeapon, bool);
	PlayerInputComponent->BindAction<FChangeWeapon>(TEXT("Rifle"), IE_Pressed, this, &UPlayerFire::ChangeWeapon, false);
	PlayerInputComponent->BindAction<FChangeWeapon>(TEXT("Sniper"), IE_Pressed, this, &UPlayerFire::ChangeWeapon, true);
	////Rifle 1번키
	//PlayerInputComponent->BindAction(TEXT("Rifle"), IE_Pressed, this, &ATpsplayer::InputRifle);
	////Sniper 2번키
	//PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this, &ATpsplayer::InputSniper);

	//델리게이트로 스나이퍼줌
	DECLARE_DELEGATE_OneParam(FSniperZoom, bool);
	PlayerInputComponent->BindAction<FSniperZoom>(TEXT("Zoom"), IE_Pressed, this, &UPlayerFire::InputZoom, true);
	PlayerInputComponent->BindAction<FSniperZoom>(TEXT("Zoom"), IE_Released, this, &UPlayerFire::InputZoom, false);
	////스나이퍼 확대
	//PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Pressed, this, &ATpsplayer::InputZoom);
	////스나이퍼 축소
	//PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Released, this, &ATpsplayer::InputZoom);
}

void UPlayerFire::InputFire() {
	//APlayerController* controller = GetWorld()->GetFirstPlayerController();
	////카메라 흔들림을 멈추자
	//controller->PlayerCameraManager->StopAllCameraShakes();
	////카메라를 흔들자
	//controller->PlayerCameraManager->StartCameraShake(cameraShake);

	//총을 쐇다!!
	bFire = true;

	//총쏘는 애니메이션을 하자
	UABP_Player* playerAnim = Cast<UABP_Player>(me->GetMesh()->GetAnimInstance());
	playerAnim->PlayAttackAnim();
	//만약에 Rifle을 들고있는 상태라면 총알을 발사한다
	if (me->compRifle->IsVisible()) {
		//FirePos Transform을 가져오자
		FTransform trFire = me->compRifle->GetSocketTransform(TEXT("FirePos"));
		//총알생성 (위치, 회전 세팅)
		GetWorld()->SpawnActor<Abullet>(bulletFactory, trFire);
	}
	else {//Sniper가 보이는 상태
		//1. Line을 발사한다
		// LineTrace 의 시작 위치
		FVector startPos = me->compCamera->GetComponentLocation();
		// LineTrace 의 종료 위치
		//카메라위치 + 카메라 앞방향 * 거리
		FVector endPos = startPos + me->compCamera->GetForwardVector() * 5000;
		// LineTrace 의 충돌 정보를 담을 변수
		FHitResult hitInfo;
		// 충돌 옵션 설정 변수
		FCollisionQueryParams params;
		// 자기 자신(플레이어) 는 충돌에서 제외
		params.AddIgnoredActor(me);
		// Channel 필터를 이용한 LineTrace 충돌 검출(충돌 정보, 시작 위치, 종료 위치, 검출 채널, 충돌 옵션)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

		//2. 만약에 누군가 Line에 맞았다면
		if (bHit) {
			//3. 맞은 위치에 폭발 효과를 보여준다
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explodeFactory, hitInfo.ImpactPoint);
			//4. 만약에 맞은애가 isSimulatingPhysics가 활성화 되어 있다면
			if (hitInfo.GetComponent()->IsSimulatingPhysics()) {
				//목표물에 force를 계산한다
				FVector force = me->compCamera->GetForwardVector() * hitInfo.GetComponent()->GetMass() * 100000;
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

void UPlayerFire::ChangeWeapon(bool useSniper) {
	//만약에 useSniper가 false(Rifle로 무기 변경)
	if (useSniper == false && sniperUI->IsInViewport() == true) {
		//common UI를 보이게 한다
		InputZoom(false);
	}
	me->compSniper->SetVisibility(useSniper);
	me->compRifle->SetVisibility(!useSniper);
}

void UPlayerFire::InputZoom(bool sniperAim) {
	if (me->compSniper->IsVisible()) {
		if (sniperAim) {//스나이퍼 조준을 한다
			//만들어 놓은 UI를 Viewport에 붙이고
			sniperUI->AddToViewport();
			//카메라의 Field of View를 45로 설정
			me->compCamera->SetFieldOfView(45);
			//이미 Viewport에 있는 CommonUI를 떼어버린다
			commonUI->RemoveFromParent();
		}
		else {//스나이퍼 조준을 안한다
			//이미 Viewport에 있는 SniperUI를 떼어버린다
			sniperUI->RemoveFromParent();
			//카메라의 Fiedl of View를 원래 설정인 90으로 설정
			me->compCamera->SetFieldOfView(90);
			//기본 commonUI를 Viewport에 추가
			commonUI->AddToViewport();
		}
	}
}