// Copyright Epic Games, Inc. All Rights Reserved.

#include "MineSweeperToolStyle.h"
#include "MineSweeperTool.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FMineSweeperToolStyle::StyleInstance = nullptr;

void FMineSweeperToolStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FMineSweeperToolStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FMineSweeperToolStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("MineSweeperToolStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FMineSweeperToolStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("MineSweeperToolStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("MineSweeperTool")->GetBaseDir() / TEXT("Resources"));

	Style->Set("MineSweeperTool.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
	return Style;
}

void FMineSweeperToolStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FMineSweeperToolStyle::Get()
{
	return *StyleInstance;
}
