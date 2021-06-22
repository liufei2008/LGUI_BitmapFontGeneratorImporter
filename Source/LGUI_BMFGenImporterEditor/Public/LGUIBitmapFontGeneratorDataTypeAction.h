// Copyright 2019-2021 LexLiu. All Rights Reserved.

#pragma once

#include "AssetTypeActions_Base.h"

class FLGUIBitmapFontGeneratorDataTypeAction : public FAssetTypeActions_Base
{
public:
	FLGUIBitmapFontGeneratorDataTypeAction(EAssetTypeCategories::Type InAssetType);

	// FAssetTypeActions_Base overrides
	virtual bool CanFilter() override;
	virtual void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override;
	virtual uint32 GetCategories() override;
	virtual FText GetName() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual FColor GetTypeColor() const override;
	virtual bool HasActions(const TArray<UObject*>& InObjects) const override;
	//virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
private:
	EAssetTypeCategories::Type assetType;
};
