// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include "Tpsplayer.h"
#include <GameFramework/Actor.h>
#include "Enemy.h"
#include <Components/CapsuleComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <CollisionQueryParams.h>
#include "EnemyAnim.h"
#include <AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h>
#include <AIModule/Classes/AIController.h>

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("AnimMontage'/Game/Blueprints/AMT_EnemyDamage.AMT_EnemyDamage'"));
	if (tempMontage.Succeeded()) {
		damageMontage = tempMontage.Object;
	}
	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();
	//타겟 찾아오기
	AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATpsplayer::StaticClass());
	target = Cast<ATpsplayer>(actor);
	//나를 찾자
	me = Cast<AEnemy>(GetOwner());
	//anim instance를 찾자
	Eanim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());
	currHealth = maxHealth;
	//나의 초기 위치를 저장하자
	originPos = me->GetActorLocation();
	//AI Controller를 가져오자
	ai = Cast<AAIController>(me->GetController());
	//ai = UAIBlueprintHelperLibrary::GetAIController(me);
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (currState)
	{
	case EEnemyState::Idle:
		UpdateIdle();
		break;
	case EEnemyState::Move:
		UpdateMove();
		break;
	case EEnemyState::Attack:
		UpdateAttack();
		break;
	case EEnemyState::AttackDelay:
		UpdateAttackDelay();
		break;
	case EEnemyState::Damaged:
		UpdateDamaged();
		break;
	case EEnemyState::Die:
		UpdateDie();
		break;
	case EEnemyState::ReturnPos:
		UpdateReturnPos();
		break;
	}

	////현재 상태를 계속 출력해보자
	//UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	//if (enumPtr != nullptr) {
	//	UE_LOG(LogTemp, Warning, TEXT("%s"), *enumPtr->GetNameStringByIndex((int32)currState));
	//}
}
//대기상태
void UEnemyFSM::UpdateIdle() {
	//시간을 흐르게 한다
	//currTime += GetWorld()->GetDeltaSeconds();
	//2초가 지나면
	//if (UpdateTime(idleDelayTime)) {
	//	//현재상태를 Move로 한다
	//	ChangeState(EEnemyState::Move);
	//	currTime = 0;
	//}
	if (IsTraceable()) {
		ChangeState(EEnemyState::Move);
	}
}

//이동상태
void UEnemyFSM::UpdateMove() {
	//Enemy가 이동해야할 방향을 구한다
	FVector direction = target->GetActorLocation() - me->GetActorLocation();
	//처음위치, 나의 현재 위치의 거리
	float distance = FVector::Distance(originPos, me->GetActorLocation());

	//만약에 distance가 moveRange보다 커지면 (움직일 수 있는 반경을 넘어갔다면)
	if (distance > moveRange) {
		//상태를 ReturnPos 로 변경
		ChangeState(EEnemyState::ReturnPos);
	}
	//만약에 target - me 거리가 공격범위보다 작으면
	else if (direction.Length() < attackRange) {
		//상태를 Attack으로 변경
		ChangeState(EEnemyState::Attack);
	}
	else {//만약에 범위에 없다면 direction방향으로 이동한다
		//Enemy에게 방향을 설정 해준다
		//me->AddMovementInput(direction.GetSafeNormal());
		//ai 를 이용해서 목적지까지 이동하고 싶다
		ai->MoveToLocation(target->GetActorLocation());
	}
}
//공격상태
void UEnemyFSM::UpdateAttack() {
	//1. 공격을 출력하자
	//UE_LOG(LogTemp, Error, TEXT("Attack!!!!!!"));
	//2. 상태를 AttackDelay로 전환
	ChangeState(EEnemyState::AttackDelay);
}
//공격후 상태
void UEnemyFSM::UpdateAttackDelay() {
	if (UpdateTime(attackDelayTime)) {
		//Enemy가 이동해야할 방향을 구한다
		FVector direction = target->GetActorLocation() - me->GetActorLocation();
		if (direction.Length() < attackRange) {
			//5. Attack 상태로 전환
			ChangeState(EEnemyState::Attack);
		}
		else {
			//6. 그렇지 않으면 Idle상태로 가자
			ChangeState(EEnemyState::Idle);
		}
	}
}
//피격상태
void UEnemyFSM::UpdateDamaged() {
	if (UpdateTime(damageDelayTime)) {
		ChangeState(EEnemyState::Idle);
	}
}

void UEnemyFSM::ReceiveDamage() {
	currHealth--;
	if (currHealth > 0) {
		ChangeState(EEnemyState::Damaged);
	}
	else {
		ChangeState(EEnemyState::Die);
	}
}

//죽음상태
void UEnemyFSM::UpdateDie() {
	//만약에 bDieMove가 false 라면 함수를 나가라
	if(!bDieMove) return;
	//아래로 내려가는 위치를 구한다
	FVector fall = me->GetActorLocation() + FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	if (fall.Z < -200) {
		me->Destroy(true);
	}
	else {
		me->SetActorLocation(fall);
	}
}

//시간 업데이트
bool UEnemyFSM::UpdateTime(float delayTime) {
	currTime += GetWorld()->GetDeltaSeconds();
	if (currTime > delayTime) {
		currTime = 0;
		return true;
	}
	else {
		return false;
	}
}

//상태변경
void UEnemyFSM::ChangeState(EEnemyState oldState) {
	//상태변경  로그를 출력하자
	UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	if (enumPtr != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("%s------>%s"),
			*enumPtr->GetNameStringByIndex((int32)currState),
			*enumPtr->GetNameStringByIndex((int32)oldState));
	}
	//현재 상태를 갱신
	currState = oldState;
	//anim의 상태 갱신
	Eanim->Estate = currState;
	//현재 시간 초기화
	currTime = 0;
	//다음 상태로 넘어가기 전에 적의 움직임을 멈추자
	ai->StopMovement();
	switch (currState)
	{
	case EEnemyState::Attack:
		//currTime = attackDelayTime;
		break;
	case EEnemyState::Damaged:
	{
		//1. 랜덤한 값을 뽑는다 (0, 1 중)
		int32 rand = FMath::RandRange(0, 1);
		//2. Damage0, Damage1 이란 문자열을 만든다.
		FString sectionName = FString::Printf(TEXT("Damage%d"), rand);
		//3. 몽타주를 플레이한다.
		me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
	}
	break;
	case EEnemyState::Die:
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//Die 몽타주 실행
		me->PlayAnimMontage(damageMontage, 1.0f, FName(TEXT("Die")));
		break;
	}
}

//플레이어를 볼수 있니
bool UEnemyFSM::IsTraceable() {
	FVector direction = target->GetActorLocation() - me->GetActorLocation();
	//Dot Product 사용법
	float dotValue = FVector::DotProduct(me->GetActorForwardVector(), direction.GetSafeNormal());
	//acos 사용법
	float degree = UKismetMathLibrary::DegAcos(dotValue);
	if (degree < 30 && direction.Length() < traceRange) {
		// LineTrace 의 충돌 정보를 담을 변수
		FHitResult hitInfo;
		// 충돌 옵션 설정 변수
		FCollisionQueryParams params;
		// 자기 자신(플레이어) 는 충돌에서 제외
		params.AddIgnoredActor(me);
		// Channel 필터를 이용한 LineTrace 충돌 검출(충돌 정보, 시작 위치, 종료 위치, 검출 채널, 충돌 옵션)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, me->GetActorLocation(), target->GetActorLocation(), ECC_Visibility, params);
		if (bHit) {
			if (hitInfo.GetActor()->GetName().Contains(TEXT("Player"))) {
				return true;
			}
		}
	}
	//그렇지 않으면 false 반환
	return false;
}

//원래 위치로 돌아간다
void UEnemyFSM::UpdateReturnPos() {
	//목적지로 이동하자
	EPathFollowingRequestResult::Type result = ai->MoveToLocation(originPos);
	//만약에 목적지에 도착했다면
	if (result == EPathFollowingRequestResult::AlreadyAtGoal) {
		//Idle 상태로 전환
		ChangeState(EEnemyState::Idle);
	}

	////1. 나 ----> 처음 위치를 향하는 방향을 구한다
	//FVector dir = originPos - me->GetActorLocation();
	////2. 만약에 나 --- 처음위치의 거리가 10보다 작으면 
	//if (dir.Length() < 10) {
	//	//3. Idle 상태로 전환
	//	ChangeState(EEnemyState::Idle);
	//}
	//else {
	//	//4. 그렇지 않으면 계속 이동
	//	me->AddMovementInput(dir.GetSafeNormal());
	//}
}