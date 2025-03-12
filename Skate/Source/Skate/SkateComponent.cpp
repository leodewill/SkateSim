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
			MovementSpeed = FMath::Max(MovementSpeed + MovementInput.Y * DeltaTime / DecelerationTime, 0.f);
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

void USkateComponent::ProcessHit(FVector Normal, FVector& OutEscapeDirection)
{
	OutEscapeDirection = GetForwardVector();

	// Checking if the hit wasn't vertical
	double VerticalAngle = FMath::RadiansToDegrees(FMath::Asin(Normal.Z));
	if (FMath::Abs(VerticalAngle) > VerticalHitThreshold)
	{
		OutEscapeDirection = FVector::ZeroVector;
		return;
	}

	// Checking incident angle. Using sin to keep direction
	FVector HorizontalNormal = FVector::VectorPlaneProject(Normal, FVector::UpVector).GetSafeNormal();
	double Incidence = FVector::DotProduct(-HorizontalNormal, GetRightVector());
	double IncidentAngle = FMath::RadiansToDegrees(FMath::Asin(Incidence));
	if (FMath::Abs(IncidentAngle) <= MinHitAngle)
	{
		MovementSpeed = 0.f;
		bIsMoving = false;
	}
	else if (FMath::Abs(IncidentAngle) <= MaxHitAngle)
	{
		OutEscapeDirection = HorizontalNormal.RotateAngleAxis(FMath::Sign(Incidence) * 90.f, FVector::UpVector);
	}
}