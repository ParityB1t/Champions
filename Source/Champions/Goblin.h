// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimBlueprint.h"
#include "Goblin.generated.h"

UCLASS()
class CHAMPIONS_API AGoblin : public ACharacter
{
	GENERATED_BODY()

    UPROPERTY(Category = Gameplay, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
    USkeletalMesh* GoblinMesh;

    UPROPERTY(Category = Gameplay, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UAnimBlueprint* GoblinAnimBlueprint;

    UPROPERTY(Category = Gameplay, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UBehaviorTree* GoblinBehaviorTree;

public:
	// Sets default values for this character's properties
	AGoblin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UBehaviorTree* GetBehaviourTree();
};
