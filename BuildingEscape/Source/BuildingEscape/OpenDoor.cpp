// Copyright 2017 Shayne Stewart

#include "OpenDoor.h"
#include "GameFramework/Actor.h"


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

	AActor* Owner = GetOwner();
	FString ObjectName = Owner->GetName();
	float ObjectRotaion = Owner->GetActorRotation().Yaw;
	UE_LOG(LogTemp, Warning, TEXT("%s rotaion is %f"), *ObjectName, ObjectRotaion);

	FRotator NewRotation = FRotator(0.0f, -60.0f, 0.0f);
	Owner->SetActorRotation(NewRotation);
	UE_LOG(LogTemp, Warning, TEXT("Rotated %s"), *ObjectName);

}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

