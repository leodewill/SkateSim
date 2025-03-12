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

protected:
	/** The time it takes for the skate to reach full speed with forward input */
	UPROPERTY(EditDefaultsOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	float AccelerationTime = .5f;

	/** The time it takes for the skate to stop with no input */
	UPROPERTY(EditDefaultsOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	float DecelerationTime = 2.f;

	/** The rotation multiplier applied to the input when the player turns */
	UPROPERTY(EditDefaultsOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	float TurnMultiplier = .03f;

private:
	float MovementSpeed = 0.f;

	FVector2D MovementInput;

	bool bIsMoving = false;
};
