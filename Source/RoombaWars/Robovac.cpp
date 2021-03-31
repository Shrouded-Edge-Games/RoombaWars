// Fill out your copyright notice in the Description page of Project Settings.


#include "Robovac.h"
#include "Camera/CameraComponent.h"
#include "GameFramework\SpringArmComponent.h"
UCameraComponent* OurCamera;
USpringArmComponent* OurSpringarm;
// Sets default values
ARobovac::ARobovac()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

    // Create a dummy root component we can attach things to.
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    // Create a camera and a visible object
    OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
    // Create a camera and a visible object
    //TODO: ATTACH CAMERA TO SWINGARM TO ALLOW FOR A REAL CONTROL OVER CAMERA PITCH
    OurSpringarm = CreateDefaultSubobject<USpringArmComponent>(TEXT("OurSpringarm"));
    GameModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
    // Attach our camera and visible object to our root component. Offset and rotate the camera.
    OurCamera->SetupAttachment(RootComponent);
    OurCamera->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
    OurCamera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
    GameModel->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARobovac::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARobovac::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    // Handle growing and shrinking based on our "Grow" action
    {
        float CurrentScale = GameModel->GetComponentScale().X;
        if (bGrowing)
        {
            // Grow to double size over the course of one second
            CurrentScale += DeltaTime;
        }
        else
        {
            // Shrink half as fast as we grow
            CurrentScale -= (DeltaTime * 0.5f);
        }
        // Make sure we never drop below our starting size, or increase past double size.
        CurrentScale = FMath::Clamp(CurrentScale, 1.0f, 2.0f);
        GameModel->SetWorldScale3D(FVector(CurrentScale));
    }

        // Handle movement based on our "MoveX" and "MoveY" axes
    {
        if (!CurrentVelocity.IsZero())
        {
            FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
            SetActorLocation(NewLocation);
        }
    }
}

// Called to bind functionality to input
void ARobovac::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    // Respond when our "Grow" key is pressed or released.
    InputComponent->BindAction("Grow", IE_Pressed, this, &ARobovac::StartGrowing);
    InputComponent->BindAction("Grow", IE_Released, this, &ARobovac::StopGrowing);

    // Respond every frame to the values of our two movement axes, "MoveX" and "MoveY".
    InputComponent->BindAxis("MoveX", this, &ARobovac::Move_XAxis);
    InputComponent->BindAxis("MoveY", this, &ARobovac::Move_YAxis);
    // Respond every frame to the values of our two movement axes, "MoveX" and "MoveY".
    InputComponent->BindAxis("CamX", this, &ARobovac::Cam_XAxis);
    InputComponent->BindAxis("CamY", this, &ARobovac::Cam_YAxis);
}
void ARobovac::Move_XAxis(float AxisValue)
{
    // Move at 100 units per second forward or backward
    CurrentVelocity.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
}

void ARobovac::Move_YAxis(float AxisValue)
{
    // Move at 100 units per second right or left
    CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
}

void ARobovac::Cam_XAxis(float AxisValue)
{
    FRotator initial = this->GetActorRotation();
    this->SetActorRotation(FRotator(initial.Pitch, initial.Yaw + AxisValue, initial.Roll));
}

void ARobovac::Cam_YAxis(float AxisValue)
{
    //Springarm
    //FRotator initial = OurCamera->GetRelativeRotation();
    //OurCamera->SetRelativeRotation(FRotator(initial.Pitch + AxisValue, initial.Yaw, initial.Roll));
}

void ARobovac::StartGrowing()
{
    bGrowing = true;
}

void ARobovac::StopGrowing()
{
    bGrowing = false;
}

