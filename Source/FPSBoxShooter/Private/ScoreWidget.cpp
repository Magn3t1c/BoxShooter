// Fill out your copyright notice in the Description page of Project Settings.
#include "ScoreWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetTree.h"

void UScoreWidget::NativeConstruct()
{
    Super::NativeConstruct();
    UE_LOG(LogTemp, Warning, TEXT("ScoreWidget NativeConstruct started"));

    // Create the root canvas panel
    RootCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());
    if (!RootCanvas)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create RootCanvas!"));
        return;
    }

    WidgetTree->RootWidget = RootCanvas;
    UE_LOG(LogTemp, Warning, TEXT("RootCanvas created successfully"));

    // Create the score text block
    ScoreText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
    if (!ScoreText)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create ScoreText!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("ScoreText created successfully"));

    // Set initial text
    ScoreText->SetText(FText::FromString(TEXT("Score: 0")));

    // Add to canvas
    UCanvasPanelSlot* TextSlot = RootCanvas->AddChildToCanvas(ScoreText);
    if (TextSlot)
    {
        TextSlot->SetPosition(FVector2D(50.0f, 50.0f));
        TextSlot->SetSize(FVector2D(300.0f, 50.0f));
        UE_LOG(LogTemp, Warning, TEXT("TextSlot positioned successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create TextSlot!"));
    }

    // Initialize with current score
    UpdateScore(CurrentScore);
    UE_LOG(LogTemp, Warning, TEXT("ScoreWidget NativeConstruct completed"));
}

void UScoreWidget::UpdateScore(int32 NewScore)
{
    CurrentScore = NewScore;
    UE_LOG(LogTemp, Warning, TEXT("UpdateScore called with: %d"), NewScore);

    if (ScoreText)
    {
        FText ScoreDisplayText = FText::FromString(FString::Printf(TEXT("Score: %d"), CurrentScore));
        ScoreText->SetText(ScoreDisplayText);
        UE_LOG(LogTemp, Warning, TEXT("UI Updated: Score set to %d"), CurrentScore);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ScoreText is null! Cannot update score display."));
    }
}