// Copyright 2019-2021 LexLiu. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "Core/LGUIFontData_BaseObject.h"
#include "LGUIBitmapFontGeneratorData.generated.h"

UENUM(BlueprintType)
enum class ELGUIBitmapFontGeneratorTextureType:uint8
{
	/** White text color, background is black. Will convert to rgba texture. */
	WhiteTextOnBlack,
	/** Keep texture's rgba color, do not convert. */
	KeepAll,
};

/** Font asset for UIText to render. Import font asset generated from "Bitmap Font Generator". */
UCLASS(BlueprintType)
class LGUI_BMFGENIMPORTER_API ULGUIBitmapFontGeneratorData : public ULGUIFontData_BaseObject
{
	GENERATED_BODY()
private:
	friend class FLGUIBitmapFontGeneratorDataCustomization;

	UPROPERTY(EditAnywhere, Category = "LGUI BitmapFont")
		FString fntFileSourcePath;
	UPROPERTY(EditAnywhere, Category = "LGUI BitmapFont")
		ELGUIBitmapFontGeneratorTextureType convertTextureType = ELGUIBitmapFontGeneratorTextureType::WhiteTextOnBlack;
	UPROPERTY(VisibleAnywhere, Category = "LGUI BitmapFont")
		FString name;
	UPROPERTY(VisibleAnywhere, Category = "LGUI BitmapFont")
		uint16 size;
	UPROPERTY(VisibleAnywhere, Category = "LGUI BitmapFont")
		uint16 lineHeight;
	UPROPERTY(VisibleAnywhere, Category = "LGUI BitmapFont")
		uint16 base;
	UPROPERTY(VisibleAnywhere, Category = "LGUI BitmapFont")
		uint16 scaleW;
	UPROPERTY(VisibleAnywhere, Category = "LGUI BitmapFont")
		uint16 scaleH;
	UPROPERTY(VisibleAnywhere, Category = "LGUI BitmapFont")
		uint16 charCount;

	UPROPERTY(VisibleAnywhere, Category = "LGUI BitmapFont")
		UTexture2D* texture;
	UPROPERTY(VisibleAnywhere, Category = "LGUI BitmapFont")
		TMap<uint16, FLGUICharData> charDataMap;

	UPROPERTY(EditAnywhere, Category = "LGUI BitmapFont")
		float fixedVerticalOffset = 0.0f;
	/** angle of italic style in degree */
	UPROPERTY(EditAnywhere, Category = "LGUI BitmapFont")
		float italicAngle = 15.0f;
	/** bold size radio for bold style, large number create more bold effect */
	UPROPERTY(EditAnywhere, Category = "LGUI BitmapFont")
		float boldRatio = 0.015f;

	//Begin ULGUIFontDataBaseObject interface
	virtual UTexture2D* GetFontTexture()override;
	virtual FLGUICharData_HighPrecision GetCharData(const TCHAR& charIndex, const uint16& charSize)override;
	virtual float GetBoldRatio() override { return boldRatio; }
	virtual float GetItalicAngle()override { return italicAngle; }
	virtual float GetFixedVerticalOffset()override { return fixedVerticalOffset - 0.25f; }//-0.25 is a common number for most fonts
	virtual void InitFont()override {};
	virtual void AddUIText(UUIText* InText)override;
	virtual void RemoveUIText(UUIText* InText)override;
	//End ULGUIFontDataBaseObject interface

	/** Collection of UIText which use this font to render. */
	UPROPERTY(VisibleAnywhere, Transient, Category = "LGUI BitmapFont")
		TArray<TWeakObjectPtr<UUIText>> renderTextArray;
#if WITH_EDITOR
	void ApplyFontTextureChange();
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
#endif
};
