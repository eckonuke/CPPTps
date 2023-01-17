// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMove.h"
#include "Tpsplayer.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Components/InputComponent.h>

UPlayerMove::UPlayerMove() {

}

void UPlayerMove::BeginPlay() {
	Super::BeginPlay();

	me->bUseControllerRotationYaw = true;
	me->compSpringArm->bUsePawnControlRotation = true;
	//���� Ƚ���� 2���Ѵ�
	me->JumpMaxCount = 2;
	//�����̴� �ӷ��� walkSpeed���� ����
	me->GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	//�����ϴ� �ӷ��� 600���� ����
	me->GetCharacterMovement()->JumpZVelocity = 600;

	//ī�޶� ���� ȸ���� min, max ����
	APlayerController* playerController = Cast<APlayerController>(me->Controller);
	playerController->PlayerCameraManager->ViewPitchMin = -45;
	playerController->PlayerCameraManager->ViewPitchMax = 45;
}

void UPlayerMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//�̵�
	MoveAction();
	//ȸ��
	//RotateAction();
}

void UPlayerMove::SetupInputBinding(UInputComponent* PlayerInputComponent) {
	Super::SetupInputBinding(PlayerInputComponent);
	//���콺 �¿�
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &UPlayerMove::Turn);
	//���콺 ����
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &UPlayerMove::LookUp);
	//A, D
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &UPlayerMove::InputHorizontal);
	//W, S
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &UPlayerMove::InputVertical);
	//����
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &UPlayerMove::InputJump);
	//�÷��̾� �پ��
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &UPlayerMove::InputRun);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &UPlayerMove::InputRun);
}

void UPlayerMove::MoveAction() {
	direction = FTransform(me->GetControlRotation()).TransformVector(direction);
	me->AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

void UPlayerMove::RotateAction() {
	//���콺 �ڿͿ� ���� Actor�� ȸ�� ��Ű��
	me->SetActorRotation(FRotator(0, mx, 0));
	//���콺 ���Ͽ� ���� SpringArm�� ȸ�� ��Ű��
	me->compSpringArm->SetRelativeRotation(FRotator(-my, 0, 0));
}

void UPlayerMove::Turn(float value) {
	me->AddControllerYawInput(value);
	//mx += value;
}
void UPlayerMove::LookUp(float value) {
	me->AddControllerPitchInput(value);
	/*my += value;
	my = FMath::Clamp(my, -45, 45);*/
}
void UPlayerMove::InputHorizontal(float value) {
	direction.Y = value;
}
void UPlayerMove::InputVertical(float value) {
	direction.X = value;
}
void UPlayerMove::InputJump() {
	me->Jump();
}
void UPlayerMove::InputRun() {
	UCharacterMovementComponent* compMove = me->GetCharacterMovement();
	//1. ���࿡ MaxWalkSpeed ���� walkSpeed ���� ũ�ٸ� (�ٰ� �ִٸ�)
	if (compMove->MaxWalkSpeed > walkSpeed) {
		//2. MaxWalkSpeed ���� walkSpeed�� ����
		compMove->MaxWalkSpeed = walkSpeed;
	}
	//3. �׷��� ������(�Ȱ� �ִٸ�)
	else {
		//4. MaxWalkSpeed ���� runSpeed�� ����
		me->GetCharacterMovement()->MaxWalkSpeed = runSpeed;
	}
}