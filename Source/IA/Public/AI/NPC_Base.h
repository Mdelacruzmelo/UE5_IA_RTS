#pragma once

#include "CoreMinimal.h"
#include "AIControllerBase.h"
#include "GameFramework/Character.h"
#include "NPC_Base.generated.h"

UCLASS()
class IA_API ANPC_Base : public ACharacter
{
	GENERATED_BODY()

public:
	ANPC_Base();

	UPROPERTY(EditInstanceOnly)
	AActor* ReferenceActor;

	UPROPERTY(VisibleAnywhere)
	AAIControllerBase* AIControllerBase;

	UFUNCTION(Blueprintable)
	AAIControllerBase* GetAIControllerBase();

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;
};
