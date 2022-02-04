// Copyright 2021-present LexLiu. All Rights Reserved.

#include "LGUI_BMFGenImporterEditorModule.h"
#include "LGUI_BMFGenImporterEditorPrivatePCH.h"

#include "SlateBasics.h"
#include "SlateExtras.h"
#include "LevelEditor.h"

#define LOCTEXT_NAMESPACE "LGUI_BMFGenImporterEditorModule"
DEFINE_LOG_CATEGORY(LGUI_BMFGenImporterEditor)

void FLGUI_BMFGenImporterEditorModule::StartupModule()
{		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	//register detail editor
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.RegisterCustomClassLayout(ULGUIBitmapFontGeneratorData::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FLGUIBitmapFontGeneratorDataCustomization::MakeInstance));
	}
	//register asset
	{
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		//register AssetCategory
		EAssetTypeCategories::Type LGUIAssetCategoryBit = AssetTools.FindAdvancedAssetCategory(FName(TEXT("LGUI")));
		if (LGUIAssetCategoryBit == EAssetTypeCategories::Misc)
		{
			LGUIAssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("LGUI")), LOCTEXT("LGUIAssetCategory", "LGUI"));
		}

		FontDataTypeAction = MakeShareable(new FLGUIBitmapFontGeneratorDataTypeAction(LGUIAssetCategoryBit));
		AssetTools.RegisterAssetTypeActions(FontDataTypeAction.ToSharedRef());
	}
}

void FLGUI_BMFGenImporterEditorModule::ShutdownModule()
{
	if (UObjectInitialized() && FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomClassLayout(ULGUIBitmapFontGeneratorData::StaticClass()->GetFName());
	}

	if (FModuleManager::Get().IsModuleLoaded(TEXT("AssetTools")))
	{
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		AssetTools.UnregisterAssetTypeActions(FontDataTypeAction.ToSharedRef());
	}
}
	
IMPLEMENT_MODULE(FLGUI_BMFGenImporterEditorModule, LGUI_BMFGenImporterEditor)

#undef LOCTEXT_NAMESPACE