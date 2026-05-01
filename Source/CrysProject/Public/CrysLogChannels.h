// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Logging/LogMacros.h"

class UObject;

CRYSPROJECT_API DECLARE_LOG_CATEGORY_EXTERN(LogCrys, Log, All);
CRYSPROJECT_API DECLARE_LOG_CATEGORY_EXTERN(LogCrysAbilitySystem, Log, All);

CRYSPROJECT_API FString GetClientServerContextString(UObject* ContextObject = nullptr);
