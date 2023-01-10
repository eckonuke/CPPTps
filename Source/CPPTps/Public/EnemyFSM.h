// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

//FSM 상태정의
UENUM(BlueprintType)
enum class EEnemyState : uint8 {
	Idle,
	Move,
	Attack,
	AttackDelay,
	Damaged,
	Die,
	ReturnPos
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPPTPS_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	//현재상태
	UPROPERTY(EditAnywhere)
	EEnemyState currState = EEnemyState::Idle;
	UPROPERTY(EditAnywhere)
	class AEnemy* me;
	UPROPERTY(EditAnywhere)
	class ATpsplayer* target;
	UPROPERTY(EditAnywhere)
	class UEnemyAnim* Eanim;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* damageMontage;
	UPROPERTY(EditAnywhere)
	float attackRange = 200.0f;
	UPROPERTY(EditAnywhere)
	float attackDelayTime = 4;
	UPROPERTY(EditAnywhere)
	float damageDelayTime = 2;

	//AI Controller
	UPROPERTY(EditAnywhere)
	class AAIController* ai;

	//delayTime
	float idleDelayTime = 2;
	//currentTime
	float currTime = 0;
	//현재 체력
	float currHealth = 0;
	//최대 체력
	float maxHealth = 10;
	//죽었을 때 내려가는 속력
	float dieSpeed = 100;
	//죽었을때 내려가게 하는 변수
	bool bDieMove = false;
	//쫒아 갈수 있는 범위
	float traceRange = 1000;
	//이동할 수 있는 반경
	float moveRange = 2000;
	//처음 위치를 담아놓을 변수
	FVector originPos;


	//대기상태
	void UpdateIdle();
	//이동상태
	void UpdateMove();
	//공격상태
	void UpdateAttack();
	//공격대기상태
	void UpdateAttackDelay();
	//피격상태
	void UpdateDamaged();
	//죽음상태
	void UpdateDie();
	//리턴
	void UpdateReturnPos();
	//상태변경시 한번만 호출 되는 함수
	void ChangeState(EEnemyState oldState);
	//시간 업데이트
	bool UpdateTime(float delayTime);
	//피격상태 전환
	void ReceiveDamage();
	//적이 플레이어를 볼수 있냐 없냐
	bool IsTraceable();
};