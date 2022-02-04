// Copyright 2021-present LexLiu. All Rights Reserved.

#include "LGUIBitmapFontGeneratorDataTypeAction.h"
#include "ContentBrowserModule.h"
#include "LGUIBitmapFontGeneratorData.h"

#define LOCTEXT_NAMESPACE "LGUIBitmapFontGeneratorDataTypeAction"

FLGUIBitmapFontGeneratorDataTypeAction::FLGUIBitmapFontGeneratorDataTypeAction(EAssetTypeCategories::Type InAssetType)
	: assetType(InAssetType)
{

}

bool FLGUIBitmapFontGeneratorDataTypeAction::CanFilter()
{
	return true;
}

void FLGUIBitmapFontGeneratorDataTypeAction::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{
	FAssetTypeActions_Base::GetActions(InObjects, MenuBuilder);
}

uint32 FLGUIBitmapFontGeneratorDataTypeAction::GetCategories()
{
	return assetType;
}

FText FLGUIBitmapFontGeneratorDataTypeAction::GetName()const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_BitmapFontDataAsset", "LGUI BitmapFontGenerator Importer");
}

UClass* FLGUIBitmapFontGeneratorDataTypeAction::GetSupportedClass()const
{
	return ULGUIBitmapFontGeneratorData::StaticClass();
}

FColor FLGUIBitmapFontGeneratorDataTypeAction::GetTypeColor()const
{
	return FColor::White;
}

bool FLGUIBitmapFontGeneratorDataTypeAction::HasActions(const TArray<UObject*>& InObjects)const
{
	return true;
}


#undef LOCTEXT_NAMESPACE
