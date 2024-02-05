#include "AI/AIControllerBase.h"

#include "AI/NPC_Base.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"


AAIControllerBase::AAIControllerBase()
{
	PrimaryActorTick.bCanEverTick = true;

	if (ANPC_Base* CastedNPC = Cast<ANPC_Base>(GetPawn()))
	{
		NPC = CastedNPC;
	}
}

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
	if (NPC == nullptr)
	{
		if (ANPC_Base* CastedNPC = Cast<ANPC_Base>(GetPawn()))
		{
			NPC = CastedNPC;
		}
	}
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AAIControllerBase::OnUnPossess()
{
	Super::OnUnPossess();

	// Aqui lo que me interesa es limpiar todos los binds
}

// SE EJECUTA AUTOMATICAMENTE AL TERMINAR EL MoveTo de la Forma 1
void AAIControllerBase::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (Result.IsSuccess())
	{
	}
	else if (Result.IsFailure())
	{
	}
	else if (Result.IsInterrupted())
	{
	}
}

void AAIControllerBase::MoveRequest(FVector3d Location)
{
	if (NPC)
	{
		// Tarea RTS condición para no moverse si ya se está moviendo
		if (bIsMoving)
		{
			UE_LOG(LogTemp, Error, TEXT("-----> El NPC ya está en movimiento"));
			return;
		}

		FAIMoveRequest MoveRequest;
		MoveRequest
			.SetAcceptanceRadius(50.f)
			.SetCanStrafe(true)
			.SetUsePathfinding(true)
			.SetGoalLocation(Location);

		FNavPathSharedPtr NavPathShared;
		FPathFollowingRequestResult RequestID = MoveTo(MoveRequest, &NavPathShared);

		GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ThisClass::MoveFinished);

		bIsMoving = true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("-----> Error, no se pudo obtener NPC_Base"));
	}
}

void AAIControllerBase::MoveFinished(FAIRequestID FaiRequestID, const FPathFollowingResult& PathFollowingResult)
{
	bIsMoving = false;

	GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
	UE_LOG(LogTemp, Warning, TEXT("-----> Hemos terminado de movernos"));
}
