// Copyright Epic Games, Inc. All Rights Reserved.
#include "FPSBoxShooterCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "BoxActor.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AFPSBoxShooterCharacter::AFPSBoxShooterCharacter()
{
    // Setup capsule
    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

    // Create first person mesh
    FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));
    FirstPersonMesh->SetupAttachment(GetMesh());
    FirstPersonMesh->SetOnlyOwnerSee(true);
    FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));
    FirstPersonMesh->bCastDynamicShadow = false;

    // Camera setup
    FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
    FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
    FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
    FirstPersonCameraComponent->bUsePawnControlRotation = true;
    FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
    FirstPersonCameraComponent->bEnableFirstPersonScale = true;
    FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
    FirstPersonCameraComponent->FirstPersonScale = 0.6f;

    // Hide 3rd person mesh
    GetMesh()->SetOwnerNoSee(true);
    GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

    // Movement tuning
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
    GetCharacterMovement()->AirControl = 0.5f;

    // Score init
    PlayerScore = 0;
    ScoreWidget = nullptr;
}

void AFPSBoxShooterCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Create widget from Blueprint class
    if (ScoreWidgetClass)
    {
        ScoreWidget = CreateWidget<UUserWidget>(GetWorld(), ScoreWidgetClass);
        if (ScoreWidget)
        {
            ScoreWidget->AddToViewport();
            UpdateScoreUI();
            UE_LOG(LogTemp, Warning, TEXT("Blueprint Score widget created and added!"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create widget from ScoreWidgetClass!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ScoreWidgetClass not set! Assign WBP_ScoreWidget in Blueprint."));
    }
}

void AFPSBoxShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Jump
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &AFPSBoxShooterCharacter::DoJumpStart);
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &AFPSBoxShooterCharacter::DoJumpEnd);

        // Movement
        EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFPSBoxShooterCharacter::MoveInput);

        // Look (for both gamepad and mouse)
        EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFPSBoxShooterCharacter::LookInput);
        EnhancedInput->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AFPSBoxShooterCharacter::LookInput);

        // Fire
        EnhancedInput->BindAction(ShootAction, ETriggerEvent::Started, this, &AFPSBoxShooterCharacter::FireInput);
    }
    else
    {
        UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find EnhancedInputComponent!"), *GetNameSafe(this));
    }
}

void AFPSBoxShooterCharacter::MoveInput(const FInputActionValue& Value)
{
    FVector2D MoveAxis = Value.Get<FVector2D>();
    DoMove(MoveAxis.X, MoveAxis.Y);
}

void AFPSBoxShooterCharacter::LookInput(const FInputActionValue& Value)
{
    FVector2D LookAxis = Value.Get<FVector2D>();
    DoAim(LookAxis.X, LookAxis.Y);
}

void AFPSBoxShooterCharacter::DoMove(float Right, float Forward)
{
    AddMovementInput(GetActorRightVector(), Right);
    AddMovementInput(GetActorForwardVector(), Forward);
}

void AFPSBoxShooterCharacter::DoAim(float Yaw, float Pitch)
{
    AddControllerYawInput(Yaw);
    AddControllerPitchInput(Pitch);
}

void AFPSBoxShooterCharacter::DoJumpStart()
{
    Jump();
}

void AFPSBoxShooterCharacter::DoJumpEnd()
{
    StopJumping();
}

void AFPSBoxShooterCharacter::FireInput()
{
    FVector Start = FirstPersonCameraComponent->GetComponentLocation();
    FVector End = Start + (FirstPersonCameraComponent->GetForwardVector() * 10000.0f);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        ABoxActor* HitBox = Cast<ABoxActor>(Hit.GetActor());
        if (IsValid(HitBox))
        {
            int32 ScoreToAdd = HitBox->GetScoreValue();
            HitBox->ApplyDamage(1);
            AddScore(ScoreToAdd);
            UE_LOG(LogTemp, Log, TEXT("Hit box! +%d points. Total Score: %d"), ScoreToAdd, PlayerScore);
        }
    }
}

void AFPSBoxShooterCharacter::AddScore(int32 ScoreDelta)
{
    PlayerScore += ScoreDelta;
    UpdateScoreUI();
    UE_LOG(LogTemp, Warning, TEXT("Score updated to: %d"), PlayerScore);
}

void AFPSBoxShooterCharacter::UpdateScoreUI()
{
    if (ScoreWidget)
    {
        // Find the text widget and update it
        UTextBlock* ScoreText = Cast<UTextBlock>(ScoreWidget->GetWidgetFromName(TEXT("ScoreText")));
        if (ScoreText)
        {
            FText ScoreDisplayText = FText::FromString(FString::Printf(TEXT("Score: %d"), PlayerScore));
            ScoreText->SetText(ScoreDisplayText);
            UE_LOG(LogTemp, Warning, TEXT("Score UI updated to: %d"), PlayerScore);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Could not find ScoreText widget by name!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ScoreWidget is null! Cannot update score display."));
    }
}