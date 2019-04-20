// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinController.h"
#include "ConstructorHelpers.h"

AGoblinController::AGoblinController()
{
    static ConstructorHelpers::FObjectFinder<UBehaviorTree> bt(TEXT("/Game/GoblinBehaviorTree"));
    check(bt.Succeeded());    

    BehaviorTree = bt.Object;
}

void AGoblinController::StartBehaviourTree()
{
    RunBehaviorTree(BehaviorTree);
}

