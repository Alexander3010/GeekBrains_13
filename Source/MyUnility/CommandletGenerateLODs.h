// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "CommandletGenerateLODs.generated.h"

/**
 * 
 */
UCLASS()
class MYUNILITY_API UCommandletGenerateLODs : public UCommandlet
{
	GENERATED_BODY()

	virtual int32 Main(const FString& Params) override;

	void ProcessAssets(TArray<FString> RootDirectories);
	void ModifyLod(UObject* AssetInstance);
	void SaveAsset(UObject* AssetInstance);
};
