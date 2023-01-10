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
	//Ÿ�� ã�ƿ���
	AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATpsplayer::StaticClass());
	target = Cast<ATpsplayer>(actor);
	//���� ã��
	me = Cast<AEnemy>(GetOwner());
	//anim instance�� ã��
	Eanim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());
	currHealth = maxHealth;
	//���� �ʱ� ��ġ�� ��������
	originPos = me->GetActorLocation();
	//AI Controller�� ��������
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

	////���� ���¸� ��� ����غ���
	//UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	//if (enumPtr != nullptr) {
	//	UE_LOG(LogTemp, Warning, TEXT("%s"), *enumPtr->GetNameStringByIndex((int32)currState));
	//}
}
//������
void UEnemyFSM::UpdateIdle() {
	//�ð��� �帣�� �Ѵ�
	//currTime += GetWorld()->GetDeltaSeconds();
	//2�ʰ� ������
	//if (UpdateTime(idleDelayTime)) {
	//	//������¸� Move�� �Ѵ�
	//	ChangeState(EEnemyState::Move);
	//	currTime = 0;
	//}
	if (IsTraceable()) {
		ChangeState(EEnemyState::Move);
	}
}

//�̵�����
void UEnemyFSM::UpdateMove() {
	//Enemy�� �̵��ؾ��� ������ ���Ѵ�
	FVector direction = target->GetActorLocation() - me->GetActorLocation();
	//ó����ġ, ���� ���� ��ġ�� �Ÿ�
	float distance = FVector::Distance(originPos, me->GetActorLocation());

	//���࿡ distance�� moveRange���� Ŀ���� (������ �� �ִ� �ݰ��� �Ѿ�ٸ�)
	if (distance > moveRange) {
		//���¸� ReturnPos �� ����
		ChangeState(EEnemyState::ReturnPos);
	}
	//���࿡ target - me �Ÿ��� ���ݹ������� ������
	else if (direction.Length() < attackRange) {
		//���¸� Attack���� ����
		ChangeState(EEnemyState::Attack);
	}
	else {//���࿡ ������ ���ٸ� direction�������� �̵��Ѵ�
		//Enemy���� ������ ���� ���ش�
		//me->AddMovementInput(direction.GetSafeNormal());
		//ai �� �̿��ؼ� ���������� �̵��ϰ� �ʹ�
		ai->MoveToLocation(target->GetActorLocation());
	}
}
//���ݻ���
void UEnemyFSM::UpdateAttack() {
	//1. ������ �������
	//UE_LOG(LogTemp, Error, TEXT("Attack!!!!!!"));
	//2. ���¸� AttackDelay�� ��ȯ
	ChangeState(EEnemyState::AttackDelay);
}
//������ ����
void UEnemyFSM::UpdateAttackDelay() {
	if (UpdateTime(attackDelayTime)) {
		//Enemy�� �̵��ؾ��� ������ ���Ѵ�
		FVector direction = target->GetActorLocation() - me->GetActorLocation();
		if (direction.Length() < attackRange) {
			//5. Attack ���·� ��ȯ
			ChangeState(EEnemyState::Attack);
		}
		else {
			//6. �׷��� ������ Idle���·� ����
			ChangeState(EEnemyState::Idle);
		}
	}
}
//�ǰݻ���
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

//��������
void UEnemyFSM::UpdateDie() {
	//���࿡ bDieMove�� false ��� �Լ��� ������
	if(!bDieMove) return;
	//�Ʒ��� �������� ��ġ�� ���Ѵ�
	FVector fall = me->GetActorLocation() + FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	if (fall.Z < -200) {
		me->Destroy(true);
	}
	else {
		me->SetActorLocation(fall);
	}
}

//�ð� ������Ʈ
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

//���º���
void UEnemyFSM::ChangeState(EEnemyState oldState) {
	//���º���  �α׸� �������
	UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	if (enumPtr != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("%s------>%s"),
			*enumPtr->GetNameStringByIndex((int32)currState),
			*enumPtr->GetNameStringByIndex((int32)oldState));
	}
	//���� ���¸� ����
	currState = oldState;
	//anim�� ���� ����
	Eanim->Estate = currState;
	//���� �ð� �ʱ�ȭ
	currTime = 0;
	//���� ���·� �Ѿ�� ���� ���� �������� ������
	ai->StopMovement();
	switch (currState)
	{
	case EEnemyState::Attack:
		//currTime = attackDelayTime;
		break;
	case EEnemyState::Damaged:
	{
		//1. ������ ���� �̴´� (0, 1 ��)
		int32 rand = FMath::RandRange(0, 1);
		//2. Damage0, Damage1 �̶� ���ڿ��� �����.
		FString sectionName = FString::Printf(TEXT("Damage%d"), rand);
		//3. ��Ÿ�ָ� �÷����Ѵ�.
		me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
	}
	break;
	case EEnemyState::Die:
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//Die ��Ÿ�� ����
		me->PlayAnimMontage(damageMontage, 1.0f, FName(TEXT("Die")));
		break;
	}
}

//�÷��̾ ���� �ִ�
bool UEnemyFSM::IsTraceable() {
	FVector direction = target->GetActorLocation() - me->GetActorLocation();
	//Dot Product ����
	float dotValue = FVector::DotProduct(me->GetActorForwardVector(), direction.GetSafeNormal());
	//acos ����
	float degree = UKismetMathLibrary::DegAcos(dotValue);
	if (degree < 30 && direction.Length() < traceRange) {
		// LineTrace �� �浹 ������ ���� ����
		FHitResult hitInfo;
		// �浹 �ɼ� ���� ����
		FCollisionQueryParams params;
		// �ڱ� �ڽ�(�÷��̾�) �� �浹���� ����
		params.AddIgnoredActor(me);
		// Channel ���͸� �̿��� LineTrace �浹 ����(�浹 ����, ���� ��ġ, ���� ��ġ, ���� ä��, �浹 �ɼ�)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, me->GetActorLocation(), target->GetActorLocation(), ECC_Visibility, params);
		if (bHit) {
			if (hitInfo.GetActor()->GetName().Contains(TEXT("Player"))) {
				return true;
			}
		}
	}
	//�׷��� ������ false ��ȯ
	return false;
}

//���� ��ġ�� ���ư���
void UEnemyFSM::UpdateReturnPos() {
	//�������� �̵�����
	EPathFollowingRequestResult::Type result = ai->MoveToLocation(originPos);
	//���࿡ �������� �����ߴٸ�
	if (result == EPathFollowingRequestResult::AlreadyAtGoal) {
		//Idle ���·� ��ȯ
		ChangeState(EEnemyState::Idle);
	}

	////1. �� ----> ó�� ��ġ�� ���ϴ� ������ ���Ѵ�
	//FVector dir = originPos - me->GetActorLocation();
	////2. ���࿡ �� --- ó����ġ�� �Ÿ��� 10���� ������ 
	//if (dir.Length() < 10) {
	//	//3. Idle ���·� ��ȯ
	//	ChangeState(EEnemyState::Idle);
	//}
	//else {
	//	//4. �׷��� ������ ��� �̵�
	//	me->AddMovementInput(dir.GetSafeNormal());
	//}
}