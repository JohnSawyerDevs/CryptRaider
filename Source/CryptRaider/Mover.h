// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoverData.h"
#include "Mover.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMover();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetAllActivations(bool IsActivated);
	UFUNCTION(BlueprintCallable)
	void SetActivationOf(FName TagName, bool IsActivated);
	UFUNCTION(BlueprintCallable)
	void ToggleAllActivations();
	UFUNCTION(BlueprintCallable)
	void ToggleActivationOf(FName TagName);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	// UPROPERTY(EditAnywhere,Instanced, Category="Mover Settings")
    // USceneComponent* ComponentTest;

	UPROPERTY(EditAnywhere, Meta = (TitleProperty = "TagName"), Category = "Mover Settings")
	TArray<FMoverData> ComponentsMoverData;
	
	void Trigger(float DeltaTime);
	USceneComponent* GetFirstComponentWithTag(const FName* SocketName);
	FRotator GetRotationSpeed(const FRotator* RotatorA, const float Time);


};

