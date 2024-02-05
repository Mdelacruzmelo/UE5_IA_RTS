#include "AI/NPC_Base.h"

#include "AI/AIControllerBase.h"


ANPC_Base::ANPC_Base()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AAIControllerBase::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

AAIControllerBase* ANPC_Base::GetAIControllerBase()
{
	if (AIControllerBase) return AIControllerBase;

	if (AAIControllerBase* AIControllerCasted = Cast<AAIControllerBase>(GetController()))
	{
		AIControllerBase = AIControllerCasted;
		return AIControllerBase;
	}

	return nullptr;
}

void ANPC_Base::BeginPlay()
{
	Super::BeginPlay();
}

void ANPC_Base::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AAIControllerBase* AIControllerCasted = Cast<AAIControllerBase>(NewController))
	{
		AIControllerBase = AIControllerCasted;
	}
}
