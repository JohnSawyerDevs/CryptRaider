// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MoverData.generated.h"

USTRUCT(BlueprintType)
struct FMoverData
{
	GENERATED_BODY()

	FORCEINLINE FMoverData();

	FORCEINLINE	void SetComponent(USceneComponent* InComponent);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TagName;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USceneComponent* Component;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsActivated;   
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector OffsetLocation;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FVector OriginalLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRotator OffsetRotation;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FRotator OriginalRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveTime;
};

FORCEINLINE uint32 GetTypeHash(const FMoverData& b)
{
	return FCrc::MemCrc32(&b, sizeof(FMoverData));
}

FORCEINLINE FMoverData::FMoverData()
{
	TagName = "None";
    Component = nullptr;
	IsActivated = false;   
    OffsetLocation = FVector(0, 0, 0);
    OffsetRotation = FRotator(0, 0, 0);
	OriginalLocation = FVector(0, 0, 0);
 	OriginalRotation = FRotator(0, 0, 0);
	MoveTime = 5;
}

FORCEINLINE	void FMoverData::SetComponent(USceneComponent* InComponent)
{
	Component = InComponent;
}

