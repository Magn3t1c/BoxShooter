// Fill out your copyright notice in the Description page of Project Settings.
#include "BoxSpawner.h"
#include "BoxActor.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonReader.h"
#include "JsonUtilities.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

ABoxSpawner::ABoxSpawner()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ABoxSpawner::BeginPlay()
{
    Super::BeginPlay();
    FetchJSON();
}

void ABoxSpawner::FetchJSON()
{
    FHttpModule* Http = &FHttpModule::Get();
    if (!Http) return;

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &ABoxSpawner::OnResponseReceived);
    Request->SetURL(JSONUrl);
    Request->SetVerb("GET");
    Request->ProcessRequest();
}

void ABoxSpawner::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to fetch JSON from URL."));
        return;
    }

    FString ResponseString = Response->GetContentAsString();

    TSharedPtr<FJsonObject> RootObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);

    if (FJsonSerializer::Deserialize(Reader, RootObject) && RootObject.IsValid())
    {
        // Parse "types"
        const TArray<TSharedPtr<FJsonValue>>* TypesArray;
        if (RootObject->TryGetArrayField("types", TypesArray))
        {
            for (const TSharedPtr<FJsonValue>& Value : *TypesArray)
            {
                TSharedPtr<FJsonObject> TypeObj = Value->AsObject();
                if (!TypeObj.IsValid()) continue;

                FBoxType Type;
                Type.Name = TypeObj->GetStringField("name");

                const TArray<TSharedPtr<FJsonValue>>* ColorArray;
                if (TypeObj->TryGetArrayField("color", ColorArray) && ColorArray->Num() == 3)
                {
                    Type.Color = FColor(
                        static_cast<uint8>((*ColorArray)[0]->AsNumber()),
                        static_cast<uint8>((*ColorArray)[1]->AsNumber()),
                        static_cast<uint8>((*ColorArray)[2]->AsNumber())
                    );
                }

                Type.Health = TypeObj->GetIntegerField("health");
                Type.Score = TypeObj->GetIntegerField("score");

                TypeMap.Add(Type.Name, Type);
            }
        }

        // Parse "objects"
        const TArray<TSharedPtr<FJsonValue>>* ObjectsArray;
        if (RootObject->TryGetArrayField("objects", ObjectsArray))
        {
            for (const TSharedPtr<FJsonValue>& Value : *ObjectsArray)
            {
                TSharedPtr<FJsonObject> Obj = Value->AsObject();
                if (!Obj.IsValid()) continue;

                FBoxInstance Instance;
                Instance.Type = Obj->GetStringField("type");

                TSharedPtr<FJsonObject> TransformObj = Obj->GetObjectField("transform");

                auto GetVec = [](const TArray<TSharedPtr<FJsonValue>>& Arr) -> FVector
                    {
                        return FVector(Arr[0]->AsNumber(), Arr[1]->AsNumber(), Arr[2]->AsNumber());
                    };

                auto GetRot = [](const TArray<TSharedPtr<FJsonValue>>& Arr) -> FRotator
                    {
                        return FRotator(Arr[0]->AsNumber(), Arr[1]->AsNumber(), Arr[2]->AsNumber());
                    };

                Instance.Transform.Location = GetVec(TransformObj->GetArrayField("location"));
                Instance.Transform.Rotation = GetRot(TransformObj->GetArrayField("rotation"));
                Instance.Transform.Scale = GetVec(TransformObj->GetArrayField("scale"));

                BoxInstances.Add(Instance);
            }
        }

        SpawnBoxes();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("JSON Parse Error."));
    }
}

void ABoxSpawner::SpawnBoxes()
{
    UWorld* World = GetWorld();
    if (!World) return;

    for (const FBoxInstance& Inst : BoxInstances)
    {
        if (!TypeMap.Contains(Inst.Type)) continue;

        const FBoxType& Type = TypeMap[Inst.Type];

        FTransform Transform;
        Transform.SetLocation(Inst.Transform.Location);
        Transform.SetRotation(FQuat(Inst.Transform.Rotation));
        Transform.SetScale3D(Inst.Transform.Scale);

        ABoxActor* Box = World->SpawnActor<ABoxActor>(ABoxActor::StaticClass(), Transform);
        if (Box)
        {
            Box->InitBox(Type, Transform);
        }
    }
}
