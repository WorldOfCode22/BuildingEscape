// Fill out your copyright notice in the Description page of Project Settings.
#include "PostitionReport.h"
#include "GameFramework/Actor.h"


// Sets default values for this component's properties
UPostitionReport::UPostitionReport()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPostitionReport::BeginPlay()
{
	Super::BeginPlay();
	FString ObjectName = GetOwner()->GetName();
	FString ObjectPostition = GetOwner()->GetActorLocation().ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s Is at %s"), *ObjectName, *ObjectPostition);
	
}


// Called every frame
void UPostitionReport::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

