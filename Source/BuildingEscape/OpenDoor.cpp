// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"


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

    if (!PressurePlate)
    {
        UE_LOG(LogTemp, Error, TEXT("%s has a OpenDoor component attached to it, but the Pressure plate is null."), *(GetOwner()->GetName()));
    }

    OpenDoorActor =  GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    if (PressurePlate)
    {
        if (PressurePlate->IsOverlappingActor(OpenDoorActor))
        {
            OpenDoor(DeltaTime);
            DoorLastOpenTime = GetWorld()->GetTimeSeconds();
        }
        else
        {
            if (GetWorld()->GetTimeSeconds() >= DoorLastOpenTime + DoorCloseDelay)
            {
                CloseDoor(DeltaTime);
            }
        }
    }
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
    FRotator CurrentRotation = GetOwner()->GetActorRotation();

    CurrentYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, OpenDoorSpeed);
    CurrentRotation.Yaw = CurrentYaw;
    GetOwner()->SetActorRotation(CurrentRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
    FRotator CurrentRotation = GetOwner()->GetActorRotation();

    CurrentYaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, CloseDoorSpeed);

    CurrentRotation.Yaw = CurrentYaw;

    GetOwner()->SetActorRotation(CurrentRotation);
}
