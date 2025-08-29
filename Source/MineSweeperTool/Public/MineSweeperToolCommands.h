// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MineSweeperToolStyle.h"

class FMineSweeperToolCommands : public TCommands<FMineSweeperToolCommands>
{
public:

	FMineSweeperToolCommands()
		: TCommands<FMineSweeperToolCommands>(TEXT("MineSweeperTool"), NSLOCTEXT("Contexts", "MineSweeperTool", "MineSweeperTool Plugin"), NAME_None, FMineSweeperToolStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
