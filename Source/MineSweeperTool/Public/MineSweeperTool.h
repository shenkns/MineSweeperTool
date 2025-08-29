#pragma once

#include "CoreMinimal.h"

#include "Modules/ModuleManager.h"

class FMineSweeperToolModule : public IModuleInterface
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void PluginButtonClicked();

private:
	
	void RegisterMenus();
	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& Args);
};
