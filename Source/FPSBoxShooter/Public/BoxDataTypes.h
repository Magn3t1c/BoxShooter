// Fill out your copyright notice in the Description page of Project Settings.
// Public/BoxDataTypes.h
#pragma once

#include "CoreMinimal.h"
#include "BoxDataTypes.generated.h"

USTRUCT(BlueprintType)
struct FPSBOXSHOOTER_API FBoxType  
{
    GENERATED_BODY()

    UPROPERTY()
    FString Name;

    UPROPERTY()
    FColor Color = FColor::White;

    UPROPERTY()
    int32 Health = 1;

    UPROPERTY()
    int32 Score = 0;
};

USTRUCT(BlueprintType)
struct FPSBOXSHOOTER_API FBoxTransform  
{
    GENERATED_BODY()

    UPROPERTY()
    FVector Location = FVector::ZeroVector;

    UPROPERTY()
    FRotator Rotation = FRotator::ZeroRotator;

    UPROPERTY()
    FVector Scale = FVector(1.0f, 1.0f, 1.0f);
};

USTRUCT(BlueprintType)
struct FPSBOXSHOOTER_API FBoxInstance  
{
    GENERATED_BODY()

    UPROPERTY()
    FString Type;

    UPROPERTY()
    FBoxTransform Transform;
};