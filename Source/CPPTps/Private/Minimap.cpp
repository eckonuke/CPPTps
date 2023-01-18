// Fill out your copyright notice in the Description page of Project Settings.


#include "Minimap.h"
#include <Engine/TextureRenderTarget2D.h>
#include <Components/SceneCaptureComponent2D.h>
#include "Tpsplayer.h"
#include <Kismet/GameplayStatics.h>

AMinimap::AMinimap() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> tempTex(TEXT("TextureRenderTarget2D'/Game/Images/RT_Minimap.RT_Minimap'"));
	if (tempTex.Succeeded()) {
		GetCaptureComponent2D()->TextureTarget = tempTex.Object;
	}
}

void AMinimap::BeginPlay() {
	Super::BeginPlay();

	//�÷��̾� ã��
	target = UGameplayStatics::GetActorOfClass(GetWorld(), ATpsplayer::StaticClass());
}

void AMinimap::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	//target ��ġ�� pos ���������� ��������
	FVector pos = target->GetActorLocation();
	//pos�� Z���� 600 ���� ����
	pos.Z = 600;
	//���� ��ġ�� pos������ �����Ѵ�
	SetActorLocation(pos);
}