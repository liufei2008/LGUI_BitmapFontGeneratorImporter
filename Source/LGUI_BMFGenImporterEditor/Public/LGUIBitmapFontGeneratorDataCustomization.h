// Copyright 2021-present LexLiu. All Rights Reserved.

#include "IDetailCustomization.h"
#pragma once

/**
 * 
 */
class FLGUIBitmapFontGeneratorDataCustomization : public IDetailCustomization
{
public:

	static TSharedRef<IDetailCustomization> MakeInstance();
	/** IDetailCustomization interface */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
private:
	TWeakObjectPtr<class ULGUIBitmapFontGeneratorData> TargetScriptPtr;
	FReply OnCreateButtonClicked(TSharedRef<IPropertyHandle> PropertyHandle);
	void OnPathTextChanged(const FString& InText, TSharedRef<IPropertyHandle> PropertyHandle);
	void OnPathTextCommitted(const FString& InText, TSharedRef<IPropertyHandle> PropertyHandle);
	FText OnGetFontFilePath()const;
};
