// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Mover.h"

UTriggerComponent::UTriggerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    AActor* Actor = GetAcceptableActor();
    if (Actor != nullptr)
    {
        float Pitch = Actor->GetActorRotation().Pitch;
        float Roll = Actor->GetActorRotation().Roll;
        if(abs(Pitch) < LeanignTreshold && abs(Roll) < LeanignTreshold )
        {
            UStaticMeshComponent* SMCStandStatue = GetComponentBySocket("SocketStatue");
            if(SMCStandStatue)
            {
                UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
                if(Component != nullptr)
                {
                    Component->SetSimulatePhysics(false);
                }

                //UE_LOG(LogTemp, Display, TEXT("Actor rotation: %s"), *Actor->GetActorRotation().ToString());
                Actor->AttachToComponent(SMCStandStatue, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), "SocketStatue");
	            UE_LOG(LogTemp, Display, TEXT("####trigger true "));
                TriggerLinkedActors(true);
            }
        }
    }
    else
    {
        TriggerLinkedActors(false);
    }
}

AActor* UTriggerComponent::GetAcceptableActor() const
{    
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);
    //UE_LOG(LogTemp, Display, TEXT("Trigger %s actors hit: %d"), *GetOwner()->GetActorNameOrLabel(), Actors.Num());

    for(AActor* Actor : Actors)
    {
        if(Actor->ActorHasTag(AcceptableActorTag) && !Actor->ActorHasTag("Grabbed") )
        {
            return Actor;
        }
    }

    return nullptr;
}

UStaticMeshComponent* UTriggerComponent::GetComponentBySocket(const FString SocketName)
{    
    TArray<UStaticMeshComponent*> Components; 
    GetOwner()->GetComponents<UStaticMeshComponent>(Components); 
    for(UStaticMeshComponent* StaticMesh : Components) 
    { 
        //const UStaticMeshSocket* socket = StaticMesh->GetSocketByName(FName("SocketName"));
        if(StaticMesh->GetSocketByName(FName(SocketName)) != nullptr)
        {
           return StaticMesh;
        }
    }

    return nullptr;
}

void UTriggerComponent::TriggerLinkedActors(bool ToActivate)
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), TargetActorTag, FoundActors);

    for (AActor* FoundActor : FoundActors)
    {
        UMover* Mover = FoundActor->FindComponentByClass<UMover>();
        if (Mover)
        {
            Mover->SetAllActivations(ToActivate);
        }
    }
}