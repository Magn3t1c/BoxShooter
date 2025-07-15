// Fill out your copyright notice in the Description page of Project Settings.
// Private/BoxActor.cpp
#include "BoxActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h" 
#include "Kismet/GameplayStatics.h"

ABoxActor::ABoxActor()
{
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube"));
    if (CubeMesh.Succeeded())
    {
        Mesh->SetStaticMesh(CubeMesh.Object);
    }

    static ConstructorHelpers::FObjectFinder<UMaterial> BaseMaterial(TEXT("/Engine/BasicShapes/BasicShapeMaterial"));
    if (BaseMaterial.Succeeded())
    {
        DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial.Object, this);
        Mesh->SetMaterial(0, DynamicMaterial);
    }

    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetCollisionResponseToAllChannels(ECR_Block);
    Mesh->SetCollisionObjectType(ECC_WorldDynamic);

    // Default values
    Health = 1;
    Score = 10;
}

void ABoxActor::BeginPlay()
{
    Super::BeginPlay();
}

void ABoxActor::InitBox(const FBoxType& TypeData, const FTransform& SpawnTransform)
{
    SetActorTransform(SpawnTransform);

    Health = TypeData.Health;
    Score = TypeData.Score;

    if (DynamicMaterial)
    {
        FVector ColorVec = FVector(TypeData.Color.R, TypeData.Color.G, TypeData.Color.B) / 255.0f;
        DynamicMaterial->SetVectorParameterValue("Color", FLinearColor(ColorVec));
    }
}

void ABoxActor::ApplyDamage(int32 DamageAmount)
{
    Health -= DamageAmount;

    if (Health <= 0)
    {
        Destroy();
    }
}

int32 ABoxActor::GetScoreValue() const
{
    return Score;
}