// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//AActor* Owner = GetOwner();
	//FRotator Rotation = Owner->GetActorRotation();
	//Rotation.Yaw = 90.f;
	//Owner->SetActorRotation(Rotation);

	FRotator CurrentRotation = GetOwner()->GetActorRotation();

	FString Trans = GetOwner()->GetActorTransform().ToString();
	UE_LOG(LogTemp, Log, TEXT("The current transformation is %s"), *Trans);
	UE_LOG(LogTemp, Warning, TEXT("The Yaw value is %f"), GetOwner()->GetActorRotation().Yaw);

	CurrentYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, 2.f);
	CurrentRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(CurrentRotation);

}

