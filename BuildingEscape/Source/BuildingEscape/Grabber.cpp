// Copyright 2017 Shayne Stewart

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Components/PrimitiveComponent.h"



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

	Owner = GetOwner();

	FindPhysicsHandleComponent();
	SetupInputComponent();
	
}

///Look for attached input component
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		//Bind input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No InputHandle found on %s"), *Owner->GetName());
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandle found on %s"), *Owner->GetName());
	}
}


/// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	///if physics is attached move the object were holding
	if (PhysicsHandle->GrabbedComponent) {
		/// Get players viewpoint
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
			PlayerViewPointLocation,
			PlayerViewPointRotation
		);
		///Ray-Cast/Line-Trace out to reach distance
		FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
		
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Get players viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewPointLocation,
		PlayerViewPointRotation
	);
	///Ray-Cast/Line-Trace out to reach distance
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;


	///Setup query params
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, Owner);

	FHitResult LineTraceHit;

	GetWorld()->LineTraceSingleByObjectType(
		LineTraceHit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	///See what we hit
	AActor* ActorHit = LineTraceHit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("We are touching %s"), *ActorHit->GetName());
	}

	return LineTraceHit;
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed"));

	///try to reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();


	//If we hit something then attach a physics handle
	if (ActorHit)
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation()
		);
	}
	}
	

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Released"));

	//release physics handle
	PhysicsHandle->ReleaseComponent();
}

