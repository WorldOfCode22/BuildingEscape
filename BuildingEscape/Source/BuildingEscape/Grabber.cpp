// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "Engine.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
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
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	/// attaching physics component to private member
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error,
			TEXT("%s: Is missing Physics Handler"),
			*GetOwner()->GetName()
		);
	}
}

void UGrabber::SetupInputComponent()
{
	/// Find attached physics handler
	InputHandle = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!InputHandle)
	{
		UE_LOG(LogTemp, Error,
			TEXT("%s: Is missing Input Handler"),
			*GetOwner()->GetName()
		);
	}

	/// Bind key actions to InputHandler
	InputHandle->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	InputHandle->BindAction("Grab", IE_Released, this, &UGrabber::Release);
}

// Returns hit result from ray cast
FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FHitResult ReachLineHit;
	/// checks to see if a physics body object is within players reach (ray-cast)
	GetWorld()->LineTraceSingleByObjectType(
		OUT ReachLineHit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams(FName(TEXT("")), false, GetOwner())
	);
	return ReachLineHit;
}
// called when grab key is pressed
void UGrabber::Grab()
{
	if (!PhysicsHandle) { return; }
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true
		);
	}
}
// called when grab key is realesed
void UGrabber::Release()
{
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

FPlayerViewPoint UGrabber::GetPlayerViewPoint()
{
	FPlayerViewPoint ViewPoint = FPlayerViewPoint();
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	ViewPoint.Location = PlayerViewPointLocation;
	ViewPoint.Rotation = PlayerViewPointRotation;
	return ViewPoint;
}

FVector UGrabber::GetReachLineStart()
{
	FPlayerViewPoint ViewPoint = GetPlayerViewPoint();
	FVector ReachLineStart =
		ViewPoint.Location;
	return ReachLineStart;
}
FVector UGrabber::GetReachLineEnd()
{
	FPlayerViewPoint ViewPoint = GetPlayerViewPoint();
	FVector ReachLineEnd =
		ViewPoint.Location + ViewPoint.Rotation.Vector() * Reach;
	return ReachLineEnd;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

