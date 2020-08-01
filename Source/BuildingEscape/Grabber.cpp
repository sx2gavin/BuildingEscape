// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	PlayerController =  GetWorld()->GetFirstPlayerController();
	SetupPhysicsHandle();
	FindInputComponent();
}

void UGrabber::SetupPhysicsHandle()
{
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

void UGrabber::FindInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Component found on Grabber Owner: %s"), *GetOwner()->GetName());
		InputComponent->BindAction(FName(TEXT("Grab")), IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction(FName(TEXT("Grab")), IE_Released, this, &UGrabber::Release);
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent)
	{
		FVector GrabberReach = GetGrabReachLineEnd();
		PhysicsHandle->SetTargetLocation(GrabberReach);
	}
}

FHitResult UGrabber::GetFirstLineTraceObject() const
{
	FVector PlayerPosition;
	FRotator PlayerRotation;

	PlayerController->GetPlayerViewPoint(OUT PlayerPosition, OUT PlayerRotation);

	FHitResult HitResult;

	FVector LineEnd = PlayerPosition + PlayerRotation.Vector() * GrabReachDistance;

	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		PlayerPosition,
		LineEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams(FName(TEXT("Grab")), false, GetOwner())
	);

	AActor* FirstActor = HitResult.GetActor();

	if (FirstActor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Result: %s"), *FirstActor->GetName());
	}

	return HitResult;
}

FVector UGrabber::GetGrabReachLineEnd() const
{
	FVector PlayerPosition;
	FRotator PlayerRotation;

	PlayerController->GetPlayerViewPoint(OUT PlayerPosition, OUT PlayerRotation);

	FHitResult HitResult;

	FVector LineEnd = PlayerPosition + PlayerRotation.Vector() * GrabReachDistance;
	return LineEnd;
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Action"));
	FHitResult Hit = GetFirstLineTraceObject();
	FVector GrabberReach = GetGrabReachLineEnd();

	if (Hit.GetActor())
	{
		UPrimitiveComponent* GrabComponent = Hit.GetComponent();
		PhysicsHandle->GrabComponentAtLocation(
			GrabComponent,
			NAME_None,
			GrabberReach
		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Release"));
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
}
