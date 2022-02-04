// Copyright 2021-present LexLiu. All Rights Reserved.

#pragma once

#include "LGUIBitmapFontGeneratorData.h"
#include "Core/ActorComponent/UIText.h"
#include "Core/Actor/LGUIManagerActor.h"

UTexture2D* ULGUIBitmapFontGeneratorData::GetFontTexture()
{
	return texture;
}
FLGUICharData_HighPrecision ULGUIBitmapFontGeneratorData::GetCharData(const TCHAR& charIndex, const uint16& charSize)
{
	FLGUICharData_HighPrecision result;
	float scale = (charSize == size) ? 1.0f : (float)charSize / size;
	if (auto charPtr = charDataMap.Find(charIndex))
	{
		result = *charPtr;
	}
	else
	{
		result = *charDataMap.Find((TCHAR)' ');//return space if not exist
	}
	result.width *= scale;
	result.height *= scale;
	result.xadvance *= scale;
	result.xoffset *= scale;
	result.yoffset *= scale;
	return result;
}

void ULGUIBitmapFontGeneratorData::AddUIText(UUIText* InText)
{
	renderTextArray.AddUnique(InText);
}
void ULGUIBitmapFontGeneratorData::RemoveUIText(UUIText* InText)
{
	renderTextArray.Remove(InText);
}
#if WITH_EDITOR
void ULGUIBitmapFontGeneratorData::ApplyFontTextureChange()
{
	auto tempArray = renderTextArray;
	for (auto textItem : tempArray)
	{
		if (textItem.IsValid())
		{
			textItem->ApplyFontTextureChange();
		}
	}
	ULGUIEditorManagerObject::RefreshAllUI();
}
void ULGUIBitmapFontGeneratorData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (auto Property = PropertyChangedEvent.Property)
	{
		auto PropertyName = Property->GetName();
		if (PropertyName == TEXT("fixedVerticalOffset") || PropertyName == TEXT("italicAngle") || PropertyName == TEXT("boldRatio"))
		{
			for (auto textItem : renderTextArray)
			{
				if (textItem.IsValid())
				{
					textItem->ApplyRecreateText();
				}
			}
		}
	}
}
#endif
