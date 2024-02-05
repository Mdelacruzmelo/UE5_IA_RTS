// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/IAPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AI/NPC_Base.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AIAPlayerController::AIAPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void AIAPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AIAPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* Component = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		// Component->BindAction(OnSetDestinationClickAction, ETriggerEvent::Started, this, &AIAPlayerController::OnInputStarted);
		// Component->BindAction(OnSetDestinationClickAction, ETriggerEvent::Triggered, this, &AIAPlayerController::OnSetDestinationTriggered);
		// Component->BindAction(OnSetDestinationClickAction, ETriggerEvent::Completed, this, &AIAPlayerController::OnSetDestinationReleased);
		// Component->BindAction(OnSetDestinationClickAction, ETriggerEvent::Canceled, this, &AIAPlayerController::OnSetDestinationReleased);

		// Tarea RTS
		Component->BindAction(IA_SelectNPC, ETriggerEvent::Started, this, &AIAPlayerController::SelectNPC);
		Component->BindAction(IA_SelectNPC, ETriggerEvent::Completed, this, &AIAPlayerController::SelectNPC);
		Component->BindAction(IA_MoveNPC, ETriggerEvent::Started, this, &AIAPlayerController::SendMoveCommand);
		Component->BindAction(IA_MoveNPC, ETriggerEvent::Completed, this, &AIAPlayerController::SendMoveCommand);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}

// Tarea RTS
void AIAPlayerController::SelectNPC(const FInputActionValue& InputActionValue)
{
	bool Pressed = InputActionValue.Get<bool>();

	if (Pressed)
	{
		FHitResult HitResult;
		if (!GetHitResultUnderCursor(ECC_Camera, true, HitResult))
		{
			NPCArray.Empty();
			return;
		}

		if (ANPC_Base* NPC = Cast<ANPC_Base>(HitResult.GetActor()))
		{
			if (!NPCArray.Contains(NPC))
			{
				UE_LOG(LogTemp, Warning, TEXT("----> NPC Added to array"));
				NPCArray.Add(NPC);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("----> NPC Removed from array"));
				NPCArray.Remove(NPC);
			}
		}
		else
		{
			NPCArray.Empty();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("----> Unpressed"));
	}
}

void AIAPlayerController::SendMoveCommand()
{
	UE_LOG(LogTemp, Warning, TEXT("----> SendMoveCommand"));
	if (NPCArray.Num() > 0)
	{
		FHitResult HitResult;
		if (!GetHitResultUnderCursor(ECC_Camera, true, HitResult)) return;

		UE_LOG(LogTemp, Warning, TEXT("----> ---> GetHitResultUnderCursor"));

		for (ANPC_Base* NPC : NPCArray)
		{
			NPC->GetAIControllerBase()->MoveRequest(HitResult.Location);
		}
	}
}


void AIAPlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void AIAPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}

	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AIAPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator,
		                                               FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}
