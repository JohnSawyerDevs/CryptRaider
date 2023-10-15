// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Mover.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UGrabber : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Grab();

	UFUNCTION(BlueprintCallable)
	void Release();

	UFUNCTION(BlueprintCallable)
	void Interact();

private:
	UPROPERTY(EditAnywhere, category="Grabber Settings")
	float MaxGrabDistance = 300;
	UPROPERTY(EditAnywhere, category="Grabber Settings")
	float GrabRadius = 50;
	UPROPERTY(EditAnywhere, category="Grabber Settings")
	float HoldDistance = 200;
	UPROPERTY(EditAnywhere, category="Grabber Settings")
	bool DrawDebug;

	UPhysicsHandleComponent* GetPhysicsHandle() const;

	bool WasHitByGrabber(FHitResult& HitResult, const bool IsInteract = false) const;
};
