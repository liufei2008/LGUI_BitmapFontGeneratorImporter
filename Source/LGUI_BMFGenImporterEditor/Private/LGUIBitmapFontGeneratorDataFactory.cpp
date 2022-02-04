// Copyright 2021-present LexLiu. All Rights Reserved.

#include "LGUIBitmapFontGeneratorDataFactory.h"
#include "LGUI_BMFGenImporterEditorPrivatePCH.h"
#include "LGUIBitmapFontGeneratorData.h"

#define LOCTEXT_NAMESPACE "LGUIBitmapFontGeneratorDataFactory"


ULGUIBitmapFontGeneratorDataFactory::ULGUIBitmapFontGeneratorDataFactory()
{
	SupportedClass = ULGUIBitmapFontGeneratorData::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}
UObject* ULGUIBitmapFontGeneratorDataFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<ULGUIBitmapFontGeneratorData>(InParent, Class, Name, Flags | RF_Transactional);
}

#undef LOCTEXT_NAMESPACE
