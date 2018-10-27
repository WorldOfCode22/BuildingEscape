// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"

#define OUT

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

	Owner = GetOwner();
}

void UOpenDoor::OpenDoor()
{
	if (IsDoorOpen) { return; }

	FRotator NewOwnerRoatation = FRotator(Owner->GetActorRotation());
	NewOwnerRoatation.Yaw += OpenAngel;
	IsDoorOpen = Owner->SetActorRotation(NewOwnerRoatation);
}

void UOpenDoor::CloseDoor()
{
	if (!IsDoorOpen) { return; }

	FRotator NewOwnerRoatation = FRotator(Owner->GetActorRotation());
	NewOwnerRoatation.Yaw -= OpenAngel;
	if (Owner->SetActorRotation(NewOwnerRoatation)) { IsDoorOpen = false; };
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOfActorsOnPlate() > 30.f) { 
		OpenDoor(); 
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	// close door if current time is greater then LastDoorOpen + delay
	if (GetWorld()->GetTimeSeconds() > LastDoorOpenTime + DoorCloseDelay) { CloseDoor(); }
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	for (const auto* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}

