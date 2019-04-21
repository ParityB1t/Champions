// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinController.h"
#include "ConstructorHelpers.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "PlayerCharacter.h"

AGoblinController::AGoblinController()
{
    static ConstructorHelpers::FObjectFinder<UBehaviorTree> bt(TEXT("/Game/GoblinBehaviorTree"));
    check(bt.Succeeded());

    BehaviorTree = bt.Object;
    Blackboard->InitializeBlackboard(*(BehaviorTree->BlackboardAsset)); // relying on using default for goblin BT

    bTargetEntityKeyID = Blackboard->GetKeyID("TargetEntity");

    for(TActorIterator<APlayerCharacter> PlayerItr(GetWorld()); PlayerItr; ++PlayerItr)
    {
        // for multiplayer, just set the array in blackboard
        APlayerCharacter* player = *PlayerItr;
        Blackboard->SetValue<UBlackboardKeyType_Object>(bTargetEntityKeyID, player);
        break;
    }
}

void AGoblinController::StartBehaviourTree()
{
    RunBehaviorTree(BehaviorTree);
}

int32 AGoblinController::TargetEntityKeyID()
{
    return bTargetEntityKeyID;
}