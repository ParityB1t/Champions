// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PlayerCharacter.h"
#include "GoblinController.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class CHAMPIONS_API AGoblinController : public AAIController
{
	GENERATED_UCLASS_BODY()

    UPROPERTY()
	UBehaviorTreeComponent* BehaviourComp;

    /*virtual void OnPossess(APawn*) override;
    virtual void OnUnPossess() override;*/

public:    

    void CheckAggro();

    void SetPlayerTarget(APlayerCharacter*);
};
