// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CrysPlayerController.h"

#include "CrimEnhancedInputComponent.h"
#include "CrysNativeGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "UINavPCComponent.h"
#include "UINavWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Input/InputActionListenerMap.h"
#include "UI/CrysHUD.h"

class UCrimEnhancedInputComponent;

ACrysPlayerController::ACrysPlayerController()
{
	UINavPCComponent = CreateDefaultSubobject<UUINavPCComponent>(TEXT("UINav PC Component"));
	UINavPCComponent->AutoHideMouse = EAutoHideMouse::Never;

	bReplicates = true;
}

UUINavWidget* ACrysPlayerController::CreateUINavWidgetAndAddToViewport(const TSubclassOf<UUINavWidget> WidgetClass,
	const bool bRestrictMovement, const bool bIsPushed, const bool bRemoveParent, const bool bDestroyParent,
	const int ZOrder)
{
	if (WidgetClass && IsLocalPlayerController())
	{
		UUINavWidget* NewWidget = CreateWidget<UUINavWidget>(this, WidgetClass);
		AddUINavWidgetToViewport(NewWidget, nullptr, bRestrictMovement, bIsPushed, bRemoveParent, bDestroyParent, ZOrder);
		return NewWidget;
	}
	return nullptr;
}

void ACrysPlayerController::AddUINavWidgetToViewport(UUINavWidget* InWidget, UWidget* InWidgetToFocus,
	const bool bRestrictMovement, const bool bIsPushed, const bool bRemoveParent, const bool bDestroyParent,
	const int ZOrder)
{
	if (IsLocalPlayerController() && InWidget)
	{
		if (bIsPushed)
		{
			const bool bNoCurrentWidget = IsValid(UINavPCComponent->GetActiveWidget());
			UINavPCComponent->GoToBuiltWidget(InWidget, bRemoveParent, bDestroyParent, ZOrder);
			if (bNoCurrentWidget)
			{
				UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, InWidgetToFocus);
				bRestrictMovementInUI = bRestrictMovement;
			}
		}
		else
		{
			if (UINavPCComponent->GetActiveWidget())
			{
				UINavPCComponent->GetActiveWidget()->ReturnToParent(true);
			}
			bRestrictMovementInUI = bRestrictMovement;
			UINavPCComponent->GoToBuiltWidget(InWidget, bRemoveParent, bDestroyParent, ZOrder);
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, InWidgetToFocus);
		}
	}
}

void ACrysPlayerController::OnRootWidgetAdded_Implementation()
{
	IUINavPCReceiver::OnRootWidgetAdded_Implementation();
	
	EnhancedInputSubsystem->AddTagToInputMode(Crys::NativeGameplayTag::EnhancedInput_UI);

	if (bRestrictMovementInUI)
	{
		SetIgnoreLookInput(true);
		SetIgnoreMoveInput(true);
	}
	
	OnRootWidgetAddedDelegate.Broadcast();
}

void ACrysPlayerController::OnRootWidgetRemoved_Implementation()
{
	IUINavPCReceiver::OnRootWidgetRemoved_Implementation();
	
	EnhancedInputSubsystem->RemoveTagFromInputMode(Crys::NativeGameplayTag::EnhancedInput_UI);

	SetIgnoreLookInput(false);
	SetIgnoreMoveInput(false);
	
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
	
	OnRootWidgetRemovedDelegate.Broadcast();
}

void ACrysPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	TryInitializeHUD();
}

void ACrysPlayerController::ApplyStartupInputActionMap()
{
	UCrimEnhancedInputComponent* EnhancedInputComponent = Cast<UCrimEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		for (const TObjectPtr<UInputActionListenerMap>& InputActionListenerMap : StartupInputActionListeners)
		{
			if (InputActionListenerMap)
			{
				for (const FInputActionListenerData& Item : InputActionListenerMap->InputActionListenerMap)
				{
					EnhancedInputComponent->SetListener(Item.InputAction, Item.InputActionListener);
				}
			}
		}
	}
}

void ACrysPlayerController::BeginPlay()
{
	Super::BeginPlay();
	TryInitializeHUD();
}

void ACrysPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	ApplyStartupInputActionMap();
	EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	
	for (const auto& Pair : InputMappingContextMap)
	{
		if (Pair.Key)
		{
			EnhancedInputSubsystem->AddMappingContext(Pair.Key, Pair.Value);
		}
	}
}

void ACrysPlayerController::TryInitializeHUD()
{
	if (bHudInitialized == false && PlayerState)
	{
		if (ACrysHUD* CrysHUD = Cast<ACrysHUD>(GetHUD()))
		{
			CrysHUD->CreateHUDWidget();
			bHudInitialized = true;
		}
	}
}
