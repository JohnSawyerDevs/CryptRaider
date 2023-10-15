// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "Mover.h"
#include "TriggerComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this component's properties
	UTriggerComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="Trigger Settings")
	FName AcceptableActorTag;
	UPROPERTY(EditAnywhere, Category="Trigger Settings")
	FName TargetActorTag;
	UPROPERTY(EditAnywhere, Category="Trigger Settings")
	float LeanignTreshold;

	AActor* GetAcceptableActor() const;

	UStaticMeshComponent* GetComponentBySocket(const FString SocketName);
	void TriggerLinkedActors(bool ToActivate);
};
