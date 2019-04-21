// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Aggro.h"
#include "CollisionQueryParams.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Engine/World.h"
#include "ChampionsGame.h"
#include "PlayerCharacter.h"
#include "GoblinController.h"


UBTService_Aggro::UBTService_Aggro(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
    NodeName = "My Service";
    bNotifyBecomeRelevant = true; // to allow OnBecomeRelevant

}

FString UBTService_Aggro::GetStaticDescription() const
{
    return FString("Check if Player is within proximity.");
}

void UBTService_Aggro::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::OnBecomeRelevant(OwnerComp, NodeMemory);

    auto ownerBlackboard = OwnerComp.GetBlackboardComponent();

    FCollisionQueryParams traceParams(SCENE_QUERY_STAT(AggroTrace), true, OwnerComp.GetOwner()); // Name, Complex, IgnoreActor

    auto startLocation = OwnerComp.GetOwner()->GetActorLocation();
    startLocation.Z = 0;
    FHitResult hit(ForceInit);
    auto targetEntity = Cast<AGoblinController>(OwnerComp.GetOwner());
    auto entityLocation = targetEntity->GetPawn()->GetActorLocation();
    entityLocation.Z = 0;

    auto traceDirection = entityLocation - startLocation;

    GetWorld()->LineTraceSingleByChannel(hit, startLocation, traceDirection*500, ENEMY_TRACE, traceParams); // hardcoded length

    if (hit.bBlockingHit)
    {
        UE_LOG(LogTemp, Error, TEXT("Hit something"));
        if (hit.GetActor() == ownerBlackboard->GetValue<UBlackboardKeyType_Object>("TargetEntity"))
        {
            ownerBlackboard->SetValueAsBool("ChasePlayer", true);
            UE_LOG(LogTemp, Error, TEXT("Hit Player"));
        }
    }
}