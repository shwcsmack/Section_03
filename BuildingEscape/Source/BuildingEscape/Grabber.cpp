// Copyright 2017 Shayne Stewart

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"


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

	Owner = GetOwner();

	FString ObjectName = Owner->GetName();
	UE_LOG(LogTemp, Warning, TEXT("Grabber is on %s"), *ObjectName);
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get players viewpoint
	FVector* PlayerLocation = new FVector();
	FRotator* PlayerRotation = new FRotator();
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(*PlayerLocation, *PlayerRotation);
	UE_LOG(LogTemp, Warning, TEXT("%s %s"), *PlayerLocation->ToString(), *PlayerRotation->ToString());
	//Ray-Cast out to reach distance

	//See what we hit
}

