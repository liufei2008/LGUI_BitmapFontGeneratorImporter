// Copyright 2021-present LexLiu. All Rights Reserved.

#include "LGUI_BMFGenImporterModule.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FLGUIModule"

void FLGUI_BMFGenImporterModule::StartupModule()
{
	
}

void FLGUI_BMFGenImporterModule::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLGUI_BMFGenImporterModule, LGUI_BMFGenImporter)