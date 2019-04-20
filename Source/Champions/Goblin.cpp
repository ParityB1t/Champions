// Fill out your copyright notice in the Description page of Project Settings.


#include "Goblin.h"
#include "GoblinController.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"

// Sets default values
AGoblin::AGoblin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("/Game/InfinityBladeAdversaries/Enemy/Enemy_Gruntling/SK_Exodus_Gruntling"));
    check(mesh.Succeeded());
    static ConstructorHelpers::FObjectFinder<UAnimBlueprint> anim(TEXT("/Game/GoblinAnim"));
    check(anim.Succeeded());    

    GoblinMesh = mesh.Object;
    GoblinAnimBlueprint = anim.Object;    
}

// Called when the game starts or when spawned
void AGoblin::BeginPlay()
{
	Super::BeginPlay();

    if (GoblinMesh && GoblinAnimBlueprint)
    {
        GetMesh()->SetSkeletalMesh(GoblinMesh);
        GetMesh()->SetAnimInstanceClass(GoblinAnimBlueprint->GeneratedClass);
    }

    AGoblinController* controller = GetWorld()->SpawnActor<AGoblinController>(GetActorLocation(), GetActorRotation());
    controller->Possess(this);
    controller->StartBehaviourTree();
}

// Called every frame
void AGoblin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
