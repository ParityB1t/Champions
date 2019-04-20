// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/CollisionProfile.h"

const FName APlayerCharacter::MoveForwardBinding("MoveForward");
const FName APlayerCharacter::MoveRightBinding("MoveRight");
const FName APlayerCharacter::LightAttackBinding("LightAttack");

const FVector APlayerCharacter::ForwardVector(1, 0, 0);
const FVector APlayerCharacter::RightVector(0, 1, 0);

// Sets default values
APlayerCharacter::APlayerCharacter()
: CameraComponent(CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera")))
, CameraBoom(CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom")))
, AttackMoveCurve()
, AttackMoveTimeline()
, bIsAttacking(false)
, bSaveAttack(false)
, bCanMove(true)
, CurrentForwardDirection()
, RotationRate(1080.0f)
, MoveSpeedAttacking(0.01f)
, MoveSpeedScale(1.0f)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create a camera boom...
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-80.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm

	// set CharacterMovement setings
	auto charMovement = GetCharacterMovement();
	charMovement->bUseControllerDesiredRotation = false;
	charMovement->bOrientRotationToMovement = true;
	charMovement->RotationRate = FRotator(0, RotationRate, 0);
	bUseControllerRotationYaw = false; // needed for orienRotationToMovement to work

	// Movement
	MoveSpeedScale = 1.0f;

    // Timeline initialisation (Curve find)
    static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/AttackMoveCurve"));
    check(Curve.Succeeded());
    AttackMoveCurve = Curve.Object;
    nextAttack = AttackCount::FirstAttack;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (CharacterMesh && CharacterAnimBlueprint)
	{
		// for now. Later load from hash
		GetMesh()->SetSkeletalMesh(CharacterMesh);
		GetMesh()->RelativeRotation = FRotator(0, 270.0f, 0);
		GetMesh()->SetAnimInstanceClass(CharacterAnimBlueprint->GeneratedClass);
	}

    if (AttackMoveCurve)
    {        
        // Timeline initialisation, (actualy init)
        AttackMoveTimeline = NewObject<UTimelineComponent>(this, FName("AttackMoveTimeline"));
        AttackMoveTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript; //   ensure gets cleared properly
        this->BlueprintCreatedComponents.Add(AttackMoveTimeline); // gets saved
        AttackMoveTimeline->SetPropertySetObject(this); // timeline act on ths object
        AttackMoveTimeline->SetLooping(false);
        AttackMoveTimeline->SetTimelineLength(0.3f);
        UE_LOG(LogTemp, Error, TEXT("Timeline Length"));
        AttackMoveTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
        AttackMoveTimeline->SetPlaybackPosition(0.0f, false);

        FOnTimelineFloat onAttackMoveTimelineCallBack;
        FOnTimelineEventStatic onAttackMoveTimelineEnd;
        onAttackMoveTimelineCallBack.BindUFunction(this, FName{TEXT("MoveForwardDuringAttack")});
        onAttackMoveTimelineEnd.BindUFunction(this, FName{TEXT("ResetAttackMoveTimeline")});
        AttackMoveTimeline->AddInterpFloat(AttackMoveCurve, onAttackMoveTimelineCallBack);
        AttackMoveTimeline->SetTimelineFinishedFunc(onAttackMoveTimelineEnd);

        AttackMoveTimeline->RegisterComponent();
    }
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::MoveForward(float ForwardValue)
{
    float totalSpeed = bCanMove? ForwardValue*MoveSpeedScale : ForwardValue*MoveSpeedAttacking;
    
    FRotator CurrentRotation(0, 0, GetControlRotation().Yaw);    
	auto ForwardDirection = UKismetMathLibrary::GetForwardVector(CurrentRotation);
	AddMovementInput(ForwardDirection, totalSpeed);
}
void APlayerCharacter::MoveRight(float RightValue)
{   
    float totalSpeed = bCanMove? RightValue*MoveSpeedScale : RightValue*MoveSpeedAttacking;

	FRotator CurrentRotation(0, 0, GetControlRotation().Yaw);
	auto RightDirection = UKismetMathLibrary::GetRightVector(CurrentRotation);
	AddMovementInput(RightDirection, totalSpeed);
}

void APlayerCharacter::Attack()
{
	if (bIsAttacking)
	{
		// started attack already, save this press
		bSaveAttack = true;
	}
	else
	{
		// haven't started attack
		bIsAttacking = true;
        bCanMove = false;
		PlayAttackAnimation();
	}

	/*if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}*/
}

// need to look at anim blueprint and continue implementing animation
void APlayerCharacter::PlayAttackAnimation()
{
    if (AttackMoveTimeline)
        AttackMoveTimeline->Play();

	switch (nextAttack)
	{
	case FirstAttack:
		PlayAnimMontage(AttackOne);
		nextAttack = SecondAttack;
		break;
	case SecondAttack:
		PlayAnimMontage(AttackTwo);
		nextAttack = FirstAttack;
		break;
	default:
		break;
	}
}

void APlayerCharacter::MoveForwardDuringAttack(float interpolatedValue)
{
    UE_LOG(LogTemp, Error, TEXT("Moving forward"));
    AddMovementInput(GetActorForwardVector(), 0.2f);
}

inline void APlayerCharacter::ResetAttackMoveTimeline()
{
    AttackMoveTimeline->SetPlaybackPosition(0.0f, false);
}

void APlayerCharacter::AttackAnimationCallback()
{
	if (bSaveAttack)
	{
		bSaveAttack = false;
		PlayAttackAnimation();
	}
}

void APlayerCharacter::ResetCombo()
{
	nextAttack = AttackCount::FirstAttack;
	bIsAttacking = false;
	bSaveAttack = false;
    bCanMove = true;
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis(MoveForwardBinding, this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(MoveRightBinding, this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAction(LightAttackBinding, IE_Pressed, this, &APlayerCharacter::Attack);
}

