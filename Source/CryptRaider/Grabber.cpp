// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "DrawDebugHelpers.h"

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

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent *PhysicsHandle = GetPhysicsHandle();
	if(PhysicsHandle != nullptr && PhysicsHandle->GetGrabbedComponent())
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent *PhysicsHandle = GetPhysicsHandle();

	FHitResult HitResult;
	if(PhysicsHandle != nullptr && WasHitByGrabber(HitResult))
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();
		HitResult.GetActor()->Tags.Add("Grabbed");
		HitResult.GetActor()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		PhysicsHandle->GrabComponentAtLocationWithRotation(HitComponent, NAME_None, HitResult.ImpactPoint, GetComponentRotation());
	}
}

void UGrabber::Release()
{
	UPhysicsHandleComponent *PhysicsHandle = GetPhysicsHandle();

	if(PhysicsHandle != nullptr && PhysicsHandle->GetGrabbedComponent())
	{
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");
		PhysicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();
		PhysicsHandle->ReleaseComponent();
	}
}

void UGrabber::Interact()
{
	UPhysicsHandleComponent *PhysicsHandle = GetPhysicsHandle();
	UE_LOG(LogTemp, Display, TEXT("Called INTERACT"));
	FHitResult HitResult;
	if(PhysicsHandle != nullptr && WasHitByGrabber(HitResult, true))
	{
		UE_LOG(LogTemp, Display, TEXT("Found Hit"));
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		AActor* Actor = HitComponent->GetOwner();
		UMover* Mover = Actor->FindComponentByClass<UMover>();
		if(Mover)
		{
			UE_LOG(LogTemp, Display, TEXT("Activation toggled"));
			UActorComponent* AComponent = Cast<UActorComponent>(HitComponent);
			UE_LOG(LogTemp, Display, TEXT("ON COMPONENT: %s"), *AComponent->ComponentTags[0].ToString());
			Mover->ToggleActivationOf(AComponent->ComponentTags[0]);
		}
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent *Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Physics Handle not found"));
	}
	// else
	// {
	// 	UE_LOG(LogTemp, Display, TEXT("Physics Handle found: %s"), *Result->GetName());
	// }

	return Result;
}

bool UGrabber::WasHitByGrabber(FHitResult& OutHitResult, const bool IsInteract) const
{	
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);	

	ECollisionChannel CollisionChannel = (IsInteract) ? ECC_GameTraceChannel3 : ECC_GameTraceChannel2;

	FCollisionQueryParams Params(NAME_None, false, GetOwner());

	bool HasHit = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CollisionChannel, Sphere, Params);
	if(HasHit && DrawDebug)
	{
			DrawDebugLine(GetWorld(), Start, End, FColor::Red);
			DrawDebugSphere(GetWorld(), OutHitResult.ImpactPoint, 10, 10, FColor::Green, false, 5);
			DrawDebugSphere(GetWorld(), OutHitResult.Location, GrabRadius, GrabRadius, FColor::Blue, false, 5);
	}

	return HasHit;
}
