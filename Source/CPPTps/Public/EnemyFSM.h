// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

//FSM ��������
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
	//�������
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
	//���� ü��
	float currHealth = 0;
	//�ִ� ü��
	float maxHealth = 10;
	//�׾��� �� �������� �ӷ�
	float dieSpeed = 100;
	//�׾����� �������� �ϴ� ����
	bool bDieMove = false;
	//�i�� ���� �ִ� ����
	float traceRange = 1000;
	//�̵��� �� �ִ� �ݰ�
	float moveRange = 2000;
	//ó�� ��ġ�� ��Ƴ��� ����
	FVector originPos;


	//������
	void UpdateIdle();
	//�̵�����
	void UpdateMove();
	//���ݻ���
	void UpdateAttack();
	//���ݴ�����
	void UpdateAttackDelay();
	//�ǰݻ���
	void UpdateDamaged();
	//��������
	void UpdateDie();
	//����
	void UpdateReturnPos();
	//���º���� �ѹ��� ȣ�� �Ǵ� �Լ�
	void ChangeState(EEnemyState oldState);
	//�ð� ������Ʈ
	bool UpdateTime(float delayTime);
	//�ǰݻ��� ��ȯ
	void ReceiveDamage();
	//���� �÷��̾ ���� �ֳ� ����
	bool IsTraceable();
};