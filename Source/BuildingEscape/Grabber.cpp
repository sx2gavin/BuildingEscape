// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	PlayerController =  GetWorld()->GetFirstPlayerController();
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle)
	{
		// Do something here.
	}
	else
	{
		UE_LOG(LogTemp,
			Error,
			TEXT("ERROR: From Grabber Component on Actor %s :Cannot find Physics Handle Component."),
			*GetOwner()->GetName()
		);
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewPointPosition;
	FRotator PlayerViewPointRotation;

	PlayerController->GetPlayerViewPoint(OUT PlayerViewPointPosition, OUT PlayerViewPointRotation);

	FVector LineReachEnd = PlayerViewPointPosition + PlayerViewPointRotation.Vector() * GrabReachDistance;

	DrawDebugLine(
		GetWorld(),
		PlayerViewPointPosition,
		LineReachEnd,
		FColor::Red,
		false,
		-1.f,
		0,
		3.f
	);

	FHitResult HitResult;

	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		PlayerViewPointPosition,
		LineReachEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams(FName(TEXT("")), false, GetOwner())
	);

	AActor* HitTarget = HitResult.GetActor();
	if (HitTarget)
	{
		// UE_LOG(LogTemp, Warning, TEXT("%s"), *HitTarget->GetName());
	}
}

