// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxDataTypes.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "BoxSpawner.generated.h"

UCLASS()
class FPSBOXSHOOTER_API ABoxSpawner : public AActor
{
    GENERATED_BODY()

public:
    ABoxSpawner();

protected:
    virtual void BeginPlay() override;

private:
    void FetchJSON();
    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
    void SpawnBoxes();

    FString JSONUrl = TEXT("https://raw.githubusercontent.com/CyrusCHAU/Varadise-Technical-Test/refs/heads/main/data.json");

    TMap<FString, FBoxType> TypeMap;
    TArray<FBoxInstance> BoxInstances;
};