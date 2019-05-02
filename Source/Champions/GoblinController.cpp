// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinController.h"
#include "ConstructorHelpers.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Goblin.h"

AGoblinController::AGoblinController(const FObjectInitializer& ObjectInitialiser)
: Super(ObjectInitialiser)
{

    BrainComponent = BehaviourComp = ObjectInitialiser.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviourComp"));

    UE_LOG(LogTemp, Error, TEXT("Initialise components"))

    bWantsPlayerState = true;
}


/*void AGoblinController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    AGoblin* goblin = Cast<AGoblin>(InPawn);
    if (goblin && goblin->GetBehaviourTree())
    {
        if (goblin->GetBehaviourTree()->BlackboardAsset)
        {
            Blackboard->InitializeBlackboard(*goblin->GetBehaviourTree()->BlackboardAsset); // relying on using default blackboard for goblin BT
        }
        BehaviourComp->StartTree(*goblin->GetBehaviourTree());
    }
}

void AGoblinController::OnUnPossess()
{
    Super::OnUnPossess();

    BehaviourComp->StopTree();
}*/

void AGoblinController::CheckAggro()
{

}

void AGoblinController::SetPlayerTarget(APlayerCharacter* player)
{
    /*if (GetWorld())
    {
        for (TActorIterator<APlayerCharacter> PlayerItr(GetWorld()); PlayerItr; ++PlayerItr)
        {
            // for multiplayer, just set the array in blackboard
            APlayerCharacter* player = *PlayerItr;
            UE_LOG(LogTemp, Error, TEXT("%s"), *player->GetName());

            //break;
        }
    }*/
    Blackboard->SetValue<UBlackboardKeyType_Object>("TargetEntity", player);
}