// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GoblinController.generated.h"

/**
 *
 */
UCLASS()
class CHAMPIONS_API AGoblinController : public AAIController
{
	GENERATED_BODY()

    UPROPERTY(Category = BehaviorTree, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UBehaviorTree* BehaviorTree;

public:
    AGoblinController();
    void StartBehaviourTree();
};
