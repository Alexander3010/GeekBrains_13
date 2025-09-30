// Fill out your copyright notice in the Description page of Project Settings.


#include "LODsCommandlet.h"
#include "EditorStaticMeshLibrary.h"
#include "Engine/StaticMesh.h"
#include "StaticMeshDescription.h"
#include "StaticMeshAttributes.h"
#include "MeshDescription.h"
#include "AssetRegistry/AssetRegistryModule.h"

int32 ULODsCommandlet::Main(const FString& Params)
{
	TArray<FString> Tokens;
	TArray<FString> Switches;

	ParseCommandLine(*Params, Tokens, Switches);

	if (Switches.Contains(TEXT("GenerateLod")))
	{
		if(Tokens.Num() > 0)
		{
			ProcessAssets(Tokens);
		}
	}
	
	return 0;
}

void ULODsCommandlet::ProcessAssets(TArray<FString> RootDirectories)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(AssetRegistryConstants::ModuleName);

	AssetRegistryModule.Get().SearchAllAssets(true);
	
	FString ClassName = TEXT("StaticMesh");

	TArray<FAssetData> AssetList;
	AssetRegistryModule.Get().GetAssetsByClass(*ClassName, AssetList, true);

	for (FAssetData& AssetData : AssetList)
	{
		for (FString RootDirectory : RootDirectories)
		{
			if (AssetData.ObjectPath.ToString().StartsWith(RootDirectory, ESearchCase::IgnoreCase))
			{
				UObject* AssetInstance = AssetData.GetAsset();
				
				ModifyLod(AssetInstance);

				SaveAsset(AssetInstance);

				break;
			}
		}
	}
}

void ULODsCommandlet::ModifyLod(UObject* AssetInstance)
{
	if (UStaticMesh* Mesh = Cast<UStaticMesh>(AssetInstance))
	{
		// LOD0
		Mesh->GetSourceModel(0).ScreenSize.Default = 0.9f;

		// LOD1
		FStaticMeshSourceModel& LOD1Model = Mesh->AddSourceModel();
		LOD1Model.ReductionSettings.PercentTriangles = 0.5f;
		LOD1Model.ScreenSize.Default = 0.5f;
		LOD1Model.BuildSettings.bRecomputeNormals = false;
		LOD1Model.BuildSettings.bRecomputeTangents = false;

		// LOD2
		FStaticMeshSourceModel& LOD2Model = Mesh->AddSourceModel();
		LOD2Model.ReductionSettings.PercentTriangles = 0.1f;
		LOD2Model.ScreenSize.Default = 0.3f;
		LOD2Model.BuildSettings.bRecomputeNormals = false;
		LOD2Model.BuildSettings.bRecomputeTangents = false;

		Mesh->PostEditChange();
		Mesh->MarkPackageDirty();

		UE_LOG(LogClass, Log, TEXT("LODs configured for mesh: %s"), *Mesh->GetName());
	}
}

void ULODsCommandlet::SaveAsset(UObject* AssetInstance)
{
	if (AssetInstance)
	{
		if (UPackage* Package = AssetInstance->GetPackage())
		{
			FString PackageName = FPackageName::LongPackageNameToFilename(Package->GetPathName(), FPackageName::GetAssetPackageExtension());

			UE_LOG(LogClass, Log, TEXT("Saving asset to: %s..."), *PackageName);

			if (Package->SavePackage(Package, AssetInstance, RF_Standalone, *PackageName, GLog))
			{
				UE_LOG(LogClass, Log, TEXT("Saved asset successfully"));
			}
			else
			{
				UE_LOG(LogClass, Warning, TEXT("Failed to save asset to %s."), *PackageName);
			}
		}
	}
}