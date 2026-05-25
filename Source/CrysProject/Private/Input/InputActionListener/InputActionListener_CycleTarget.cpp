// Copyright Soccertitan 2026


#include "Input/InputActionListener/InputActionListener_CycleTarget.h"

#include "CrimTargetingSystemBlueprintFunctionLibrary.h"
#include "CrimTargetingSystemComponent.h"
#include "InputActionValue.h"
#include "TargetingSystem/CrysTargetingContext.h"
#include "TargetingSystem/TargetingSubsystem.h"

UInputActionListener_CycleTarget::UInputActionListener_CycleTarget()
{
	TargetingContext = CreateDefaultSubobject<UCrysTargetingContext>(TEXT("TargetingContext"));
}

void UInputActionListener_CycleTarget::OnInitializeListener()
{
	Super::OnInitializeListener();
	TargetingSystemComponent = UCrimTargetingSystemBlueprintFunctionLibrary::GetCrimTargetingSystemComponent(GetPlayerController());
}

void UInputActionListener_CycleTarget::OnInputActionTriggered(const FInputActionValue& Value)
{
	Super::OnInputActionTriggered(Value);
	
	if (Value.GetMagnitude() > 0.f)
	{
		TargetingContext->SearchDirection = EUINavigation::Right;
	}
	else
	{
		TargetingContext->SearchDirection = EUINavigation::Left;
	}
	TargetingContext->StartingTargetPoint = TargetingSystemComponent->GetTargetPoint();
	
	UTargetingSubsystem* TargetingSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UTargetingSubsystem>();
	FTargetingSourceContext TargetingSourceContext;
	TargetingSourceContext.SourceActor = GetPlayerController()->GetPawn();
	TargetingSourceContext.SourceObject = TargetingContext;
	FTargetingRequestDelegate Delegate;
	Delegate.BindUObject(this, &UInputActionListener_CycleTarget::OnTargetingRequestCompleted);
	FTargetingRequestHandle Handle = UTargetingSubsystem::MakeTargetRequestHandle(TargetingPreset, TargetingSourceContext);
	TargetingSubsystem->ExecuteTargetingRequestWithHandle(Handle, Delegate);
}

void UInputActionListener_CycleTarget::OnTargetingRequestCompleted(FTargetingRequestHandle Handle)
{
	FTargetingDefaultResultsSet* ResultSet = FTargetingDefaultResultsSet::Find(Handle);
	if (ResultSet && ResultSet->TargetResults.IsValidIndex(0))
	{
		FCrimTargetPoint TargetPoint(ResultSet->TargetResults[0].HitResult);
		TargetingSystemComponent->SetTargetPoint(TargetPoint);
	}
}
