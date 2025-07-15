// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreWidget.generated.h"

class UTextBlock;
class UCanvasPanel;

UCLASS()
class FPSBOXSHOOTER_API UScoreWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Score")
    void UpdateScore(int32 NewScore);

protected:
    virtual void NativeConstruct() override;

    UPROPERTY()
    UTextBlock* ScoreText;

    UPROPERTY()
    UCanvasPanel* RootCanvas;

private:
    int32 CurrentScore = 0;
};