// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimBlueprint.h"
#include "Animation/AnimMontage.h"
#include "Components/TimelineComponent.h"
#include "Math/Vector.h"
#include "Curves/CurveFloat.h"
#include "PlayerCharacter.generated.h"

enum AttackCount
{
	FirstAttack = 0,
	SecondAttack
};

UCLASS(Blueprintable)
class CHAMPIONS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** The camera */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* CameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;
    
    UPROPERTY()
        class UCurveFloat* AttackMoveCurve;
    UPROPERTY()
        class UTimelineComponent* AttackMoveTimeline;

	bool bIsAttacking;
	bool bSaveAttack;
	AttackCount nextAttack;
	bool bCanMove;
    FVector CurrentForwardDirection;

	void PlayAttackAnimation();    

    UFUNCTION()
    void MoveForwardDuringAttack(float interpolatedValue);

    UFUNCTION()
    void ResetAttackMoveTimeline();

public:
	// Sets default values for this character's properties
	APlayerCharacter();

    UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float RotationRate;

    /* Move speed when attack, essentialy just allows small rotation */
    UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float MoveSpeedAttacking;

	/* The speed our ship moves around the level */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float MoveSpeedScale;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		USkeletalMesh* CharacterMesh;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		UAnimBlueprint* CharacterAnimBlueprint;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		UAnimMontage* AttackOne;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		UAnimMontage* AttackTwo;

	// Static names for axis bindings
	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;
	static const FName LightAttackBinding;

	static const FVector RightVector;
	static const FVector ForwardVector;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Moving callbacks */
	void MoveForward(float ForwardValue);

	void MoveRight(float RightValue);

	/* Attacking callback */
	void Attack();

	UFUNCTION(Category = "Attack", BlueprintCallable)
		void AttackAnimationCallback();

	UFUNCTION(Category = "Attack", BlueprintCallable)
		void ResetCombo();
};
