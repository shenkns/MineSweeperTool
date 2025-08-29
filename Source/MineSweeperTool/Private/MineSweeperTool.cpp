#include "MinesweeperTool.h"

#include "ToolMenus.h"
#include "Widgets/Docking/SDockTab.h"
#include "SMineSweeperPanel.h"

#define LOCTEXT_NAMESPACE "FMineSweeperToolModule"

static const FName MinesweeperTabName("MineSweeperToolTab");

void FMineSweeperToolModule::StartupModule()
{
    FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
        MinesweeperTabName,
        FOnSpawnTab::CreateRaw(this, &FMineSweeperToolModule::OnSpawnPluginTab))
        .SetDisplayName(LOCTEXT("MineSweeperTabTitle", "MineSweeper"))
        .SetTooltipText(LOCTEXT("MineSweeperTabTooltip", "Open MineSweeper tool"))
        .SetMenuType(ETabSpawnerMenuType::Hidden);
    
    UToolMenus::RegisterStartupCallback(
        FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMineSweeperToolModule::RegisterMenus));
}

void FMineSweeperToolModule::ShutdownModule()
{
    UToolMenus::UnRegisterStartupCallback(this);
    UToolMenus::UnregisterOwner(this);

    FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MinesweeperTabName);
}

void FMineSweeperToolModule::RegisterMenus()
{
    if (UToolMenu* Toolbar = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar"))
    {
        FToolMenuSection& Section = Toolbar->AddSection("MineSweeperTool", LOCTEXT("MineSweeperSection", "Tools"));
        Section.AddEntry(FToolMenuEntry::InitToolBarButton(
            "OpenMineSweeper",
            FUIAction(FExecuteAction::CreateRaw(this, &FMineSweeperToolModule::PluginButtonClicked)),
            LOCTEXT("MineSweeperButton", "MineSweeper"),
            LOCTEXT("MineSweeperTooltip", "Open the MineSweeper Tool")
        ));
    }
}

void FMineSweeperToolModule::PluginButtonClicked()
{
    FGlobalTabmanager::Get()->TryInvokeTab(MinesweeperTabName);
}

TSharedRef<SDockTab> FMineSweeperToolModule::OnSpawnPluginTab(const FSpawnTabArgs& Args)
{
    return SNew(SDockTab)
        .TabRole(ETabRole::NomadTab)
        [
            SNew(SMineSweeperPanel)
        ];
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMineSweeperToolModule, MineSweeperTool)
