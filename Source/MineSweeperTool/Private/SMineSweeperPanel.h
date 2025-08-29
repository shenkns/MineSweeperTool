#pragma once

#include "Widgets/SCompoundWidget.h"

class MINESWEEPERTOOL_API SMineSweeperPanel : public SCompoundWidget
{
public:
	
	SLATE_BEGIN_ARGS(SMineSweeperPanel)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
};
