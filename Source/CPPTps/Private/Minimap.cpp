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

	//플레이어 찾자
	target = UGameplayStatics::GetActorOfClass(GetWorld(), ATpsplayer::StaticClass());
}

void AMinimap::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	//target 위치를 pos 지역변수에 가져오자
	FVector pos = target->GetActorLocation();
	//pos의 Z값을 600 으로 변경
	pos.Z = 600;
	//나의 위치를 pos값으로 설정한다
	SetActorLocation(pos);
}