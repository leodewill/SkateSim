// Fill out your copyright notice in the Description page of Project Settings.


#include "SkateComponent.h"

USkateComponent::USkateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	AccelerationTime = .5f;
	DecelerationTime = 2.f;
	TurnMultiplier = .03f;

	MovementSpeed = 0.f;
	MovementInput = FVector2D::ZeroVector;
	bIsMoving = false;
}

void USkateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (bIsMoving)
	{
		if (MovementInput.Y > 0.f)
		{
			MovementSpeed = FMath::Min(MovementSpeed + MovementInput.Y * DeltaTime / AccelerationTime, 1.f);
		}
		else if (MovementInput.Y < 0.f)
		{
			MovementSpeed = FMath::Max(MovementSpeed + DecelerationTime * MovementInput.Y * DeltaTime / DecelerationTime, 0.f);
			if (FMath::IsNearlyZero(MovementSpeed))
			{
				bIsMoving = false;
			}
		}
	}
}

void USkateComponent::SetMovementInput(FVector2D Input)
{
	MovementInput = Input;
	bIsMoving = true;
}
