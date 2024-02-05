#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerBase.generated.h"


class ANPC_Base;

UCLASS()
class IA_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	AAIControllerBase();

	UPROPERTY(EditInstanceOnly, Category=TareaRTS)
	ANPC_Base* NPC;

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	void MoveFinished(FAIRequestID FaiRequestID, const FPathFollowingResult& PathFollowingResult);

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

public:
	UFUNCTION(Blueprintable)
	void MoveRequest(FVector3d Location);

private:
	bool bIsMoving;
};
