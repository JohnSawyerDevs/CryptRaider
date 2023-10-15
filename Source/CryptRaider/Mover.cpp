#include "Mover.h"
#include "math.h"

UMover::UMover()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UMover::BeginPlay()
{
	Super::BeginPlay();

	USceneComponent* SceneComponent;
	for(FMoverData& MoverData : ComponentsMoverData)
	{
		SceneComponent = GetFirstComponentWithTag(&MoverData.TagName);
		if(SceneComponent)
		{
            UE_LOG(LogTemp, Display, TEXT(" adding component %s"), *(SceneComponent->GetName()));
			MoverData.Component = SceneComponent;
			MoverData.OriginalLocation = SceneComponent->GetComponentLocation();
			MoverData.OriginalRotation = SceneComponent->GetComponentRotation();
		}
	}
	
	
}


void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Trigger(DeltaTime);
}	


void UMover::SetAllActivations(bool NewIsActivated)
{	
	if(NewIsActivated)
	{
	UE_LOG(LogTemp, Display, TEXT("####opening "));}
	for(FMoverData& MoverData : ComponentsMoverData)
	{
		if(MoverData.Component)
		{
			MoverData.IsActivated = NewIsActivated;
		}
	}
}

void UMover::SetActivationOf(FName TagName, bool NewIsActivated)
{
	for(FMoverData& MoverData : ComponentsMoverData)
	{
		if(MoverData.Component && MoverData.TagName == TagName)
		{
			MoverData.IsActivated = NewIsActivated;
		}
	}
}

void UMover::ToggleAllActivations()
{
	for(FMoverData& MoverData : ComponentsMoverData)
	{
		if(MoverData.Component)
		{
			MoverData.IsActivated = !MoverData.IsActivated;
		}
	}
}

void UMover::ToggleActivationOf(FName TagName)
{
	UE_LOG(LogTemp, Display, TEXT("TOGGLING..."));
	for(FMoverData& MoverData : ComponentsMoverData)
	{
		if(MoverData.Component && MoverData.TagName == TagName)
		{
			UE_LOG(LogTemp, Display, TEXT("TOGGLED activation for component with tag: %s"), *TagName.ToString());
			MoverData.IsActivated = !MoverData.IsActivated;
		}
	}
}

USceneComponent* UMover::GetFirstComponentWithTag(const FName* SocketName)
{    
	//UE_LOG(LogTemp, Display, TEXT("Find for tag: %s"), **SocketName);
    TArray<UActorComponent*> Components; 

	Components = GetOwner()->GetComponentsByTag(UStaticMeshComponent::StaticClass(), *SocketName);
	for (UActorComponent* it : Components)
	{
		//UE_LOG(LogTemp, Display, TEXT("%s Name : %s"), **SocketName, *it->GetName());
		return Cast<USceneComponent>(it);
	}

    return nullptr;
}	

FRotator UMover::GetRotationSpeed(const FRotator* RotatorA, const float Time)
{
	FRotator RotationSpeed;
	RotationSpeed.Pitch = RotatorA->Pitch / Time;
	RotationSpeed.Yaw = RotatorA->Yaw / Time;
	RotationSpeed.Roll = RotatorA->Roll / Time;
	return RotationSpeed;
}

void UMover::Trigger(float DeltaTime)
{
	for(FMoverData MoverData : ComponentsMoverData)
	{
		if(MoverData.Component)
		{
			FVector CurrentLocation = MoverData.Component->GetComponentLocation();
			FVector TargetLocation = MoverData.OriginalLocation + MoverData.OffsetLocation;
			float MovingSpeed = MoverData.OffsetLocation.Length() / MoverData.MoveTime;

			FRotator CurrentRotation = MoverData.Component->GetComponentRotation();
			FRotator TargetRotation = MoverData.OriginalRotation + MoverData.OffsetRotation;
			FRotator RotationSpeed = GetRotationSpeed(&MoverData.OffsetRotation, MoverData.MoveTime); 

			if(MoverData.IsActivated)
			{
				// UE_LOG(LogTemp, Display, TEXT("IS moving"));
				if(CurrentLocation != TargetLocation)
				{
					// UE_LOG(LogTemp, Display, TEXT("location ORIGINAL: %s"), *OriginalLocation.ToString());
					// UE_LOG(LogTemp, Display, TEXT("location CURRENT: %s"), *CurrentLocation.ToString());
					// UE_LOG(LogTemp, Display, TEXT("location TARGET: %s"), *TargetLocation.ToString());
					// UE_LOG(LogTemp, Display, TEXT("location SPEED: %f"), MovingSpeed);
					FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, MovingSpeed);
					MoverData.Component->SetWorldLocation(NewLocation);
				}

				if(!CurrentRotation.Equals(TargetRotation, 1))
				{
					MoverData.Component->AddLocalRotation(RotationSpeed * DeltaTime);
				}
			}
			else
			{		
				// UE_LOG(LogTemp, Display, TEXT("IS moving back"));
				if(CurrentLocation != MoverData.OriginalLocation)
				{
					// UE_LOG(LogTemp, Display, TEXT("location ORIGINAL: %s"), *OriginalLocation.ToString());
					// UE_LOG(LogTemp, Display, TEXT("location CURRENT: %s"), *CurrentLocation.ToString());
					// UE_LOG(LogTemp, Display, TEXT("location TARGET: %s"), *TargetLocation.ToString());
					// UE_LOG(LogTemp, Display, TEXT("location SPEED: %f"), MovingSpeed);
					FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, MoverData.OriginalLocation, DeltaTime, MovingSpeed);
					MoverData.Component->SetWorldLocation(NewLocation);
				}

				if(!CurrentRotation.Equals(MoverData.OriginalRotation, 2))
				{
					MoverData.Component->AddLocalRotation(RotationSpeed.GetInverse() * DeltaTime);
				}
			}
		}
	}
}