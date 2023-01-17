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
	//점프 횟수를 2번한다
	me->JumpMaxCount = 2;
	//움직이는 속력을 walkSpeed으로 변경
	me->GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	//점프하는 속력을 600으로 변경
	me->GetCharacterMovement()->JumpZVelocity = 600;

	//카메라 상하 회전값 min, max 세팅
	APlayerController* playerController = Cast<APlayerController>(me->Controller);
	playerController->PlayerCameraManager->ViewPitchMin = -45;
	playerController->PlayerCameraManager->ViewPitchMax = 45;
}

void UPlayerMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//이동
	MoveAction();
	//회전
	//RotateAction();
}

void UPlayerMove::SetupInputBinding(UInputComponent* PlayerInputComponent) {
	Super::SetupInputBinding(PlayerInputComponent);
	//마우스 좌우
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &UPlayerMove::Turn);
	//마우스 상하
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &UPlayerMove::LookUp);
	//A, D
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &UPlayerMove::InputHorizontal);
	//W, S
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &UPlayerMove::InputVertical);
	//점프
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &UPlayerMove::InputJump);
	//플레이어 뛰어간다
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &UPlayerMove::InputRun);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &UPlayerMove::InputRun);
}

void UPlayerMove::MoveAction() {
	direction = FTransform(me->GetControlRotation()).TransformVector(direction);
	me->AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

void UPlayerMove::RotateAction() {
	//마우스 자와에 따라서 Actor를 회전 시키자
	me->SetActorRotation(FRotator(0, mx, 0));
	//마우스 상하에 따라서 SpringArm을 회전 시키자
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
	//1. 만약에 MaxWalkSpeed 값이 walkSpeed 보다 크다면 (뛰고 있다면)
	if (compMove->MaxWalkSpeed > walkSpeed) {
		//2. MaxWalkSpeed 값을 walkSpeed로 하자
		compMove->MaxWalkSpeed = walkSpeed;
	}
	//3. 그렇지 않으면(걷고 있다면)
	else {
		//4. MaxWalkSpeed 값을 runSpeed로 하자
		me->GetCharacterMovement()->MaxWalkSpeed = runSpeed;
	}
}