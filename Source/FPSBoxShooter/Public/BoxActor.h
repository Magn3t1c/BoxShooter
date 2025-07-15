// Fill out your copyright notice in the Description page of Project Settings.
// Public/BoxActor.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxDataTypes.h"
#include "BoxActor.generated.h"

class UStaticMeshComponent;
class UMaterialInstanceDynamic;

UCLASS()
class FPSBOXSHOOTER_API ABoxActor : public AActor
{
    GENERATED_BODY()

public:
    ABoxActor();

    void InitBox(const FBoxType& TypeData, const FTransform& SpawnTransform);

    // Apply damage - keep it simple like before
    void ApplyDamage(int32 DamageAmount);

    int32 GetScoreValue() const;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    UStaticMeshComponent* Mesh;

    UPROPERTY()
    UMaterialInstanceDynamic* DynamicMaterial;

    int32 Health;
    int32 Score;
};