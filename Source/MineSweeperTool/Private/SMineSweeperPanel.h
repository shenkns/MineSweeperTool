#pragma once

#include "Widgets/SCompoundWidget.h"

class SUniformGridPanel;

class MINESWEEPERTOOL_API SMineSweeperPanel : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SMineSweeperPanel) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	FReply OnNewGameClicked();
	void StartNewGame();

	void RevealTile(int32 X, int32 Y);
	int32 CountAdjacentBombs(int32 X, int32 Y) const;

	void CheckWinCondition();

	void RevealAllBombs();

private:

	// Configs
	int32 GridWidth = 5;
	int32 GridHeight = 5;
	int32 Bombs = 5;

	// Grid state
	TArray<bool> BombGrid;    
	TArray<bool> Revealed;    

	// Components
	TSharedPtr<SEditableTextBox> WidthBox;
	TSharedPtr<SEditableTextBox> HeightBox;
	TSharedPtr<SEditableTextBox> BombsBox;

	TSharedPtr<SUniformGridPanel> GridPanel;

	// Game state
	bool bGameOver = false;
	bool bWin = false;
};
