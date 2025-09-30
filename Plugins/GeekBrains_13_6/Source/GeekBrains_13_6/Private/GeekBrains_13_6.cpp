// Copyright Epic Games, Inc. All Rights Reserved.

#include "GeekBrains_13_6.h"

#define LOCTEXT_NAMESPACE "FGeekBrains_13_6Module"

void FGeekBrains_13_6Module::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FGeekBrains_13_6Module::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGeekBrains_13_6Module, GeekBrains_13_6)