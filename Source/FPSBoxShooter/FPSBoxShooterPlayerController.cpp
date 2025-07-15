// Copyright Epic Games, Inc. All Rights Reserved.


#include "FPSBoxShooterPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "FPSBoxShooterCameraManager.h"

AFPSBoxShooterPlayerController::AFPSBoxShooterPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = AFPSBoxShooterCameraManager::StaticClass();
}

void AFPSBoxShooterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}
