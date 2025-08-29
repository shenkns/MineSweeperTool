// Copyright Epic Games, Inc. All Rights Reserved.

#include "MineSweeperToolCommands.h"

#define LOCTEXT_NAMESPACE "FMineSweeperToolModule"

void FMineSweeperToolCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "MineSweeperTool", "Execute MineSweeperTool action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
