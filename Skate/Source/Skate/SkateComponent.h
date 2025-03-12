// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "SkateComponent.generated.h"

/**
 * 
 */
UCLASS()
class SKATE_API USkateComponent : public UArrowComponent
{
	GENERATED_BODY()
	
public:
	USkateComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetMovementInput(FVector2D Input);

	float GetMovementSpeed() const { return MovementSpeed; }

	float GetTurnSpeed() const { return MovementInput.X * TurnMultiplier; }

	bool IsMoving() const { return bIsMoving; }

	// Called by the owner when the player gets hit. Returns the new movement direction
	void ProcessHit(FVector Normal, FVector& OutEscapeDirection);

protected:
	/** The time it takes for the skate to reach full speed with forward input */
	UPROPERTY(EditDefaultsOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	float AccelerationTime = .5f;

	/** The time it takes for the skate to stop with no input */
	UPROPERTY(EditDefaultsOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	float DecelerationTime = 2.f;

	/** The rotation multiplier applied to the input when the player turns */
	UPROPERTY(EditDefaultsOnly, Category = "Settings", meta = (AllowPrivateAccess = "true", ClampMin = "0", ClampMax = "1", UIMin = "0", UIMax = "90"))
	float TurnMultiplier = .03f;

	/** The minimum incident angle for a horizontal hit to redirect the skate. A lower angle will stop the skate. */
	UPROPERTY(EditDefaultsOnly, Category = "Settings", meta = (AllowPrivateAccess = "true", ClampMin = "0", ClampMax = "90", UIMin = "0", UIMax = "90"))
	float MinHitAngle = 30.f;

	/** The maximum incident angle for a horizontal hit to redirect the skate. A higher angle won't change velocity. */
	UPROPERTY(EditDefaultsOnly, Category = "Settings", meta = (AllowPrivateAccess = "true", ClampMin = "0", ClampMax = "90", UIMin = "0", UIMax = "90"))
	float MaxHitAngle = 87.f;

	/** The minimum angle for an obstable hit to be considered vertical */
	UPROPERTY(EditDefaultsOnly, Category = "Settings", meta = (AllowPrivateAccess = "true", ClampMin = "0", ClampMax = "90", UIMin = "0", UIMax = "90"))
	float VerticalHitThreshold = 30.f;

private:
	float MovementSpeed = 0.f;

	FVector2D MovementInput;

	bool bIsMoving = false;
};
