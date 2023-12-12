// Copyright 2023 Cyrus 365. All Rights Reserved.

#include "C3_LAA.h"

#define LOCTEXT_NAMESPACE "FC3_LAAModule"

void FC3_LAAModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FC3_LAAModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FC3_LAAModule, C3_LAA)