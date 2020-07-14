// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"

// Sets default values
AShip::AShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ParticleSystems	= CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	ExplosionFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionFX"));
	DeathExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DeathExplosionSound"));

	ShipMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ParticleSystems->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ExplosionFX->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	DeathExplosionSound->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	MaxVelocity = 250.0f;
	Current_X_Velocity = 0.0f;
	Current_Y_Velocity = 0.0f;
	
	bIsFiring = false;
	WeaponFireRate = 0.25f;
	TimeSinceLastShot = 0.0f;

	PlayerScore = 0.0f;
	
}


// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();
	
	Current_Location = this->GetActorLocation();
	Current_Rotation = this->GetActorRotation();
	
	bHit = false;
	bDead = false;
	
	ExplosionFX->Deactivate();
	DeathExplosionSound->Deactivate();

	Max_Health = 100.0f;
	Current_Health = 100.0f;

	Max_Armor = 100.0f;
	Current_Armor = 100.0f;
	
	OnActorBeginOverlap.AddDynamic(this, &AShip::OnBeginOverlap);
}


// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Current_X_Velocity != 0.0f || Current_Y_Velocity != 0.0f) {
		New_Location = FVector(Current_Location.X + (Current_X_Velocity * DeltaTime),
			Current_Location.Y + (Current_Y_Velocity * DeltaTime), 0);
		this->SetActorLocation(New_Location);
		Current_Location = New_Location;
	}
	if (Current_Y_Velocity > 0.1f) {
		this->SetActorRotation(FRotator(Current_Rotation.Pitch + 45.0f, 
			Current_Rotation.Yaw, Current_Rotation.Roll));
	}
	else if (Current_Y_Velocity < -0.1f) {
		this->SetActorRotation(FRotator(Current_Rotation.Pitch - 45.0f, 
			Current_Rotation.Yaw, Current_Rotation.Roll));
	}
	else {
		this->SetActorRotation(FRotator(Current_Rotation));
	}
	if (this->GetActorLocation().X > Field_Width) 
		Current_Location = FVector(Field_Width - 1, Current_Location.Y, Current_Location.Z);
	if (this->GetActorLocation().X < -Field_Width)
		Current_Location = FVector(-Field_Width + 1, Current_Location.Y, Current_Location.Z);
	if (this->GetActorLocation().Y > Field_Height)
		Current_Location = FVector(Current_Location.X, Field_Height - 1, Current_Location.Z);
	if (this->GetActorLocation().Y < -Field_Height)
		Current_Location = FVector(Current_Location.X, -Field_Height + 1, Current_Location.Z);

}

// Called to bind functionality to input
void AShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AShip::MoveRight);
	PlayerInputComponent->BindAxis(FName("MoveUp"), this, &AShip::MoveUp);
	Pl
}



void AShip::MoveRight(float AxisValue)
{
	Current_X_Velocity = MaxVelocity * AxisValue;
}

void AShip::MoveUp(float AxisValue)
{
	Current_Y_Velocity = MaxVelocity * AxisValue;
}

void AShip::StartFiring()
{
	bIsFiring = true;
}

void AShip::StopFiring()
{
	bIsFiring = false; 
}


void AShip::OnBeginOverlap(AActor* PlayerActor, AActor* OtherActor)
{

}
