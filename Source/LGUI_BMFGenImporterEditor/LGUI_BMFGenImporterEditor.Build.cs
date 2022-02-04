// Copyright 2021-present LexLiu. All Rights Reserved.

using UnrealBuildTool;

public class LGUI_BMFGenImporterEditor : ModuleRules
{
	public LGUI_BMFGenImporterEditor(ReadOnlyTargetRules Target) : base(Target)
    {
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "CoreUObject",
                "Slate",
                "SlateCore",
                "Engine",
                "UnrealEd",
                "PropertyEditor",
                "RenderCore",
                "RHI",
				"LGUI_BMFGenImporter",
				"LGUIEditor",
                "LevelEditor",
                "Projects",
                "EditorWidgets",
                "DesktopPlatform",
                "ImageWrapper",
                "AssetTools",//Asset editor
				"SharedSettingsWidgets",
				// ... add other public dependencies that you statically link with here ...
                
            }
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "EditorStyle",
				// ... add private dependencies that you statically link with here ...	

            }
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

    }
}
