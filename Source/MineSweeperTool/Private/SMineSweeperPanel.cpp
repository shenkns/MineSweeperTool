#include "SMineSweeperPanel.h"

#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Misc/MessageDialog.h"

void SMineSweeperPanel::Construct(const FArguments& InArgs)
{
    ChildSlot
    [
        SNew(SVerticalBox)

        // Game setup
        + SVerticalBox::Slot().AutoHeight().Padding(5)
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot().AutoWidth().Padding(2) [ SNew(STextBlock).Text(FText::FromString("Width:")) ]
            + SHorizontalBox::Slot().AutoWidth().Padding(2) [ SAssignNew(WidthBox, SEditableTextBox).Text(FText::AsNumber(GridWidth)) ]
            + SHorizontalBox::Slot().AutoWidth().Padding(2) [ SNew(STextBlock).Text(FText::FromString("Height:")) ]
            + SHorizontalBox::Slot().AutoWidth().Padding(2) [ SAssignNew(HeightBox, SEditableTextBox).Text(FText::AsNumber(GridHeight)) ]
            + SHorizontalBox::Slot().AutoWidth().Padding(2) [ SNew(STextBlock).Text(FText::FromString("Bombs:")) ]
            + SHorizontalBox::Slot().AutoWidth().Padding(2) [ SAssignNew(BombsBox, SEditableTextBox).Text(FText::AsNumber(Bombs)) ]
        ]

        // Generate button
        + SVerticalBox::Slot().AutoHeight().Padding(5)
        [
            SNew(SButton)
            .Text(FText::FromString("Generate New Grid"))
            .OnClicked(this, &SMineSweeperPanel::OnNewGameClicked)
        ]

        // Grid
        + SVerticalBox::Slot().FillHeight(1.f).Padding(5)
        [
            SAssignNew(GridPanel, SUniformGridPanel)
        ]
    ];

    StartNewGame();
}

FReply SMineSweeperPanel::OnNewGameClicked()
{
    // Collect from text boxes
    const FString WString = WidthBox->GetText().ToString();
    const FString HString = HeightBox->GetText().ToString();
    const FString BString = BombsBox->GetText().ToString();

    GridWidth = FCString::Atoi(*WString);
    GridHeight = FCString::Atoi(*HString);
    Bombs = FCString::Atoi(*BString);

    StartNewGame();
    
    return FReply::Handled();
}

void SMineSweeperPanel::StartNewGame()
{
    BombGrid.Init(false, GridWidth * GridHeight);
    Revealed.Init(false, GridWidth * GridHeight);

    bGameOver = false;
    bWin = false;

    // Place bombs
    int32 Placed = 0;
    while (Placed < Bombs && Placed < BombGrid.Num())
    {
        int32 Index = FMath::RandRange(0, BombGrid.Num() - 1);
        if (!BombGrid[Index])
        {
            BombGrid[Index] = true;
            Placed++;
        }
    }

    // Generate widgets
    GridPanel->ClearChildren();

    for (int32 Y = 0; Y < GridHeight; Y++)
    {
        for (int32 X = 0; X < GridWidth; X++)
        {
            int32 Index = Y * GridWidth + X;

            GridPanel->AddSlot(X, Y)
            [
                // Paint buttons
                SNew(SButton)
                .ButtonColorAndOpacity_Lambda([this, Index]()
                {
                    if (!Revealed[Index]) return FLinearColor(0.2f,0.2f,0.2f,1.f);

                    if (BombGrid[Index]) 
                        return bWin ? FLinearColor(0.f,0.6f,0.f,1.f)
                                    : FLinearColor(0.8f,0.f,0.f,1.f);

                    return FLinearColor(0.7f,0.7f,0.7f,1.f);
                })
                .IsEnabled_Lambda([this, Index]() { return !Revealed[Index] && !bGameOver; })
                .Text_Lambda([this, Index, X, Y]()
                {
                    if (!Revealed[Index]) return FText::GetEmpty();
                    if (BombGrid[Index]) return FText::FromString("X");

                    int32 Adj = CountAdjacentBombs(X, Y);
                    return Adj > 0 ? FText::FromString(FString::FromInt(Adj)) : FText::GetEmpty();
                })
                .OnClicked_Lambda([this, X, Y]()
                {
                    RevealTile(X, Y);
                    return FReply::Handled();
                })
            ];
        }
    }
}

void SMineSweeperPanel::RevealTile(int32 X, int32 Y)
{
    if (X < 0 || Y < 0 || X >= GridWidth || Y >= GridHeight) return;

    int32 Index = Y * GridWidth + X;
    if (Revealed[Index]) return;

    Revealed[Index] = true;

    if (BombGrid[Index])
    {
        bGameOver = true;
        RevealAllBombs();
        FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("Game Over!"));
        return;
    }

    int32 Adjacent = CountAdjacentBombs(X, Y);
    if (Adjacent == 0)
    {
        RevealTile(X - 1, Y);
        RevealTile(X + 1, Y);
        RevealTile(X, Y - 1);
        RevealTile(X, Y + 1);
    }

    CheckWinCondition();
}

int32 SMineSweeperPanel::CountAdjacentBombs(int32 X, int32 Y) const
{
    int32 BombCount = 0;

    // Check all surrounding offsets (-1, 0, +1)
    for (int32 OffsetY = -1; OffsetY <= 1; OffsetY++)
    {
        for (int32 OffsetX = -1; OffsetX <= 1; OffsetX++)
        {
            // Skip the center cell itself
            if (OffsetX == 0 && OffsetY == 0)
                continue;

            int32 NeighborX = X + OffsetX;
            int32 NeighborY = Y + OffsetY;

            // Make sure neighbor is inside the grid
            if (NeighborX >= 0 && NeighborY >= 0 && NeighborX < GridWidth && NeighborY < GridHeight)
            {
                int32 Index = NeighborY * GridWidth + NeighborX;
                if (BombGrid[Index])
                {
                    BombCount++;
                }
            }
        }
    }

    return BombCount;
}


void SMineSweeperPanel::CheckWinCondition()
{
    for (int32 i = 0; i < BombGrid.Num(); i++)
    {
        // Closed safe exist
        if (!BombGrid[i] && !Revealed[i]) return;
    }

    bGameOver = true;
    bWin = true;
    RevealAllBombs();

    FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("You Win!"));
}

void SMineSweeperPanel::RevealAllBombs()
{
    for (int32 i = 0; i < BombGrid.Num(); i++)
    {
        if (BombGrid[i]) Revealed[i] = true;
    }
}
