// Copyright 2019-2021 LexLiu. All Rights Reserved.

#include "LGUIBitmapFontGeneratorDataCustomization.h"
#include "LGUI_BMFGenImporterEditorPrivatePCH.h"
#include "MISC/FileHelper.h"
#include "Widget/LGUIFileBrowser.h"

#include "AssetRegistryModule.h"
#include "Factories.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "ImageUtils.h"
#include "Misc/Parse.h"
#include "SHyperlinkLaunchURL.h"
#include "ObjectTools.h"

#define LOCTEXT_NAMESPACE "LGUIBitmapFontGeneratorDataCustomization"



class TextureUtils
{
public:
	static UTexture2D* CreateTextureAsset(UTexture2D* OldTexture, FString PackageName, int Width, int Height, TArray<FColor>& Samples, TextureCompressionSettings CompressionSettings, TextureGroup LODGroup)
	{
		FCreateTexture2DParameters TexParams;
		TexParams.bUseAlpha = true;
		TexParams.CompressionSettings = CompressionSettings;
		TexParams.bDeferCompression = true;
		TexParams.bSRGB = false;
		TexParams.SourceGuidHash = FGuid::NewGuid();

		auto ExistTexture = LoadObject<UTexture2D>(NULL, *PackageName);
		if (ExistTexture)
		{
			if (FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(FString(TEXT("A delete texture dialog will showup, just confirm delete.")))) == EAppReturnType::Ok)
			{
				ObjectTools::DeleteAssets({ ExistTexture }, true);//if not show confirm dialog, then assets won't be deleted, don't know why
			}
		}
		auto Package = FindPackage(NULL, *PackageName);
		if (!IsValid(Package))
		{
#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 26
			Package = CreatePackage(*PackageName);
#else
			Package = CreatePackage(NULL, *PackageName);
#endif
		}
		UTexture2D* NewTexture = FImageUtils::CreateTexture2D(Width, Height, Samples, Package, FPaths::GetCleanFilename(PackageName), RF_Standalone | RF_Public, TexParams);
		NewTexture->LODGroup = LODGroup;
		NewTexture->DeferCompression = false;

		FAssetRegistryModule::AssetCreated(NewTexture);
		Package->SetDirtyFlag(true);

		return NewTexture;
	}

	static bool LoadTexture2DFromFile(const FString& ImagePath, EImageFormat ImageFormat, TArray<uint8>& ResultRawData)
	{
		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(ImageFormat);

		//Load From File
		TArray<uint8> RawFileData;
		if (!FFileHelper::LoadFileToArray(RawFileData, *ImagePath)) return NULL;

		//Create T2D!
		if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
		{
#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 25
			return ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, ResultRawData);
#else
			const TArray<uint8>* UncompressedBGRA = NULL;
			if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
			{
				ResultRawData = *UncompressedBGRA;
				return true;
			}
#endif
		}

		return false;
	}
};


TSharedRef<IDetailCustomization> FLGUIBitmapFontGeneratorDataCustomization::MakeInstance()
{
	return MakeShareable(new FLGUIBitmapFontGeneratorDataCustomization);
}

void FLGUIBitmapFontGeneratorDataCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> targetObjects;
	DetailBuilder.GetObjectsBeingCustomized(targetObjects);
	TargetScriptPtr = Cast<ULGUIBitmapFontGeneratorData>(targetObjects[0].Get());
	if (TargetScriptPtr == nullptr)
	{
		UE_LOG(LGUI_BMFGenImporterEditor, Log, TEXT("Get TargetScript is null"));
		return;
	}

	DetailBuilder.HideProperty("fntFileSourcePath");
	IDetailCategoryBuilder& createCategory = DetailBuilder.EditCategory("Create");
	createCategory.AddCustomRow(LOCTEXT("Tips", "Tips"))
		.WholeRowContent()
		[
			SNew(SBox)
			.HeightOverride(40)
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				[
					SNew(STextBlock)
					.Text(FText::FromString("Use \"Bitmap Font Generator\" to create .fnt and .png file. Only support one texture file, only support png format.\"Bitmap Font Generator\" can be found here:"))
					.AutoWrapText(true)
				]
				+SVerticalBox::Slot()
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SBox)
						.HAlign(HAlign_Center)
						[
							SNew(SHyperlinkLaunchURL, TEXT("https://www.angelcode.com/products/bmfont/"))
							.Text(LOCTEXT("Bitmap Font Generator", "https://www.angelcode.com/products/bmfont/"))
						]
					]
				]
			]
		];

	auto fntFileSourcePathHandle = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(ULGUIBitmapFontGeneratorData, fntFileSourcePath));
	FString fntFileSourcePath;
	fntFileSourcePathHandle->GetValue(fntFileSourcePath);
	createCategory.AddCustomRow(LOCTEXT("FontSourceFile","FontSourceFile"))
	.NameContent()
	[
		SNew(STextBlock)
		.Text(LOCTEXT("FontSourceFile", "Font Source File"))
	]
	.ValueContent()
	.MinDesiredWidth(300)
	[	
		SNew(SLGUIFileBrowser)
		.FolderPath(this, &FLGUIBitmapFontGeneratorDataCustomization::OnGetFontFilePath)
		.DialogTitle(TEXT("Browse for a font data file"))
		.DefaultFileName("font.fnt")
		.Filter("Bitmap Font Generator exported file|*.fnt")
		.OnFilePathChanged(this, &FLGUIBitmapFontGeneratorDataCustomization::OnPathTextChanged, fntFileSourcePathHandle)
		.OnFilePathCommitted(this, &FLGUIBitmapFontGeneratorDataCustomization::OnPathTextCommitted, fntFileSourcePathHandle)
	];

	createCategory.AddProperty(GET_MEMBER_NAME_CHECKED(ULGUIBitmapFontGeneratorData, convertTextureType));
	
	createCategory.AddCustomRow(LOCTEXT("LoadFont", "Load Font"))
	.WholeRowContent()
	[
		SNew(SButton)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		.OnClicked(this, &FLGUIBitmapFontGeneratorDataCustomization::OnCreateButtonClicked, fntFileSourcePathHandle)
		.Text(LOCTEXT("LoadFont", "Load Font"))
	];
}

FText FLGUIBitmapFontGeneratorDataCustomization::OnGetFontFilePath()const
{
	auto& fileManager = IFileManager::Get();
	return FText::FromString(TargetScriptPtr->fntFileSourcePath.IsEmpty() ? fileManager.GetFilenameOnDisk(*FPaths::ProjectDir()) : TargetScriptPtr->fntFileSourcePath);
}

void FLGUIBitmapFontGeneratorDataCustomization::OnPathTextChanged(const FString& InString, TSharedRef<IPropertyHandle> PropertyHandle)
{
	PropertyHandle->SetValue(InString);
}
void FLGUIBitmapFontGeneratorDataCustomization::OnPathTextCommitted(const FString& InString, TSharedRef<IPropertyHandle> PropertyHandle)
{
	PropertyHandle->SetValue(InString);
}

FReply FLGUIBitmapFontGeneratorDataCustomization::OnCreateButtonClicked(TSharedRef<IPropertyHandle> PropertyHandle)
{
	FString fntFileSourcePath;
	PropertyHandle->GetValue(fntFileSourcePath);

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (PlatformFile.FileExists(*fntFileSourcePath))
	{
		auto FontFileFolderPathStr = FPaths::GetPath(fntFileSourcePath);
		if (!FontFileFolderPathStr.EndsWith("/"))FontFileFolderPathStr = FontFileFolderPathStr.Append("/");

		TArray<FString> fontStringLines;
		FFileHelper::LoadANSITextFileToStrings(*fntFileSourcePath, NULL, fontStringLines);
		float texFullWidthRec = 0;
		float texFullHeightRec = 0;
		int size = 16;
		for (auto strLine : fontStringLines)
		{
			if (strLine.StartsWith(TEXT("info ")))
			{
				FParse::Value(*strLine, TEXT("face="), TargetScriptPtr->name);
				FParse::Value(*strLine, TEXT("size="), size);
			}
			else if (strLine.StartsWith(TEXT("common")))
			{
				FParse::Value(*strLine, TEXT("scaleH="), TargetScriptPtr->scaleH);
				FParse::Value(*strLine, TEXT("scaleW="), TargetScriptPtr->scaleW);
				FParse::Value(*strLine, TEXT("lineHeight="), TargetScriptPtr->lineHeight);
				FParse::Value(*strLine, TEXT("base="), TargetScriptPtr->base);

				texFullWidthRec = 1.0f / TargetScriptPtr->scaleW;
				texFullHeightRec = 1.0f / TargetScriptPtr->scaleH;

				if (size < 0)
				{
					TargetScriptPtr->size = -size;
				}
				else
				{
					TargetScriptPtr->size = TargetScriptPtr->base;
				}
			}
			else if (strLine.StartsWith(TEXT("page")))
			{
				TArray<FColor> fontTexturePixels;
				auto pixelCount = TargetScriptPtr->scaleH * TargetScriptPtr->scaleW;
				fontTexturePixels.AddUninitialized(pixelCount);

				FString file;
				if (FParse::Value(*strLine, TEXT("file="), file))
				{
					FString texItemFilePathStr(FontFileFolderPathStr);
					texItemFilePathStr.Append(file);
					//UE_LOG(LogTemp, Log, TEXT("---------texture file:%s, file:%s, item:%s"), *texItemFilePathStr, *file, *item);
					TArray<uint8> rawData;
					if (TextureUtils::LoadTexture2DFromFile(texItemFilePathStr, EImageFormat::PNG, rawData))
					{
						FColor* itemColor = (FColor*)(rawData.GetData());
						switch (TargetScriptPtr->convertTextureType)
						{
						default:
						case ELGUIBitmapFontGeneratorTextureType::WhiteTextOnBlack:
						{
							for (int i = 0; i < pixelCount; i++)
							{
								auto itemPixel = itemColor[i];
								FColor pixel(255, 255, 255, itemPixel.R);
								fontTexturePixels[i] = pixel;
							}
						}
						break;
						case ELGUIBitmapFontGeneratorTextureType::KeepAll:
						{
							FMemory::Memcpy(fontTexturePixels.GetData(), itemColor, rawData.Num());
						}
						break;
						}

						FString texturePackageName;
						if (IsValid(TargetScriptPtr->texture))
						{
							texturePackageName = TargetScriptPtr->texture->GetPathName();
							int indexOfDot = 0;
							if (texturePackageName.FindLastChar('.', indexOfDot))
							{
								texturePackageName = texturePackageName.Left(indexOfDot);
							}
						}
						else
						{
							texturePackageName = TargetScriptPtr->GetPathName();
							int lastCharIndex = 0;
							if (texturePackageName.FindLastChar('/', lastCharIndex))
							{
								texturePackageName = texturePackageName.Left(lastCharIndex + 1);
							}
							texturePackageName.Append(TargetScriptPtr->GetName()).Append(TEXT("_Texture"));
						}
						auto fontTexture = TextureUtils::CreateTextureAsset(TargetScriptPtr->texture, texturePackageName, TargetScriptPtr->scaleW, TargetScriptPtr->scaleH, fontTexturePixels, TextureCompressionSettings::TC_EditorIcon, TextureGroup::TEXTUREGROUP_World);
						TargetScriptPtr->texture = fontTexture;
					}
				}
			}
			else if (strLine.StartsWith(TEXT("chars")))
			{
				FParse::Value(*strLine, TEXT("count="), TargetScriptPtr->charCount);
			}
			else if (strLine.StartsWith(TEXT("char")))
			{
				FLGUICharData itemData;
				uint16 id = 0;
				int x = 0, y = 0;

				FParse::Value(*strLine, TEXT("id="), id);
				FParse::Value(*strLine, TEXT("width="), itemData.width);
				FParse::Value(*strLine, TEXT("height="), itemData.height);
				FParse::Value(*strLine, TEXT("xoffset="), itemData.xoffset);
				FParse::Value(*strLine, TEXT("yoffset="), itemData.yoffset);
				FParse::Value(*strLine, TEXT("xadvance="), itemData.xadvance);
				FParse::Value(*strLine, TEXT("x="), x);
				FParse::Value(*strLine, TEXT("y="), y);

				itemData.yoffset = TargetScriptPtr->base - itemData.yoffset;
				itemData.uv0X = x * texFullWidthRec;
				itemData.uv0Y = (y + itemData.height) * texFullHeightRec;
				itemData.uv3X = (x + itemData.width) * texFullWidthRec;
				itemData.uv3Y = y * texFullHeightRec;
				TargetScriptPtr->charDataMap.Add(id, itemData);
			}
		}
		
		auto spaceChar = (TCHAR)' ';
		if (TargetScriptPtr->charDataMap.Find(spaceChar) == nullptr)
		{
			FLGUICharData spaceCharData;
			spaceCharData.width = spaceCharData.height = spaceCharData.xadvance = TargetScriptPtr->size;
			spaceCharData.uv0X = spaceCharData.uv0Y = spaceCharData.uv3X = spaceCharData.uv3Y = 1.0f;
			TargetScriptPtr->charDataMap.Add((TCHAR)' ', spaceCharData);
		}
		TargetScriptPtr->MarkPackageDirty();
		TargetScriptPtr->ApplyFontTextureChange();
	}
	else
	{
		UE_LOG(LGUI_BMFGenImporterEditor, Error, TEXT("[LGUI_BitmapFontEditor] File: [%s] not exist!"), *fntFileSourcePath);
	}
	return FReply::Handled();
}
#undef LOCTEXT_NAMESPACE