// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Aggro.generated.h"

/**
 *
 */
UCLASS()
class CHAMPIONS_API UBTService_Aggro : public UBTService
{
	GENERATED_BODY()

    virtual FString GetStaticDescription() const override;

public:
    UBTService_Aggro(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:


    virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
