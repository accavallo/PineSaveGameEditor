#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PineSaveGameEditor.h"

#include <QJsonArray>

#include "GameInfo.h"

QT_FORWARD_DECLARE_CLASS(AffinityWheel)
QT_FORWARD_DECLARE_CLASS(NewItemDialog)

class PineSaveGameEditor : public QMainWindow
{
    Q_OBJECT

public:
    PineSaveGameEditor(QWidget *parent = Q_NULLPTR);

    /* Function: NonBlockingSleep
    * Arguments: sleepDuration - Time, in milliseconds, to sleep.
    *            checkInterval - Time, in milliseconds, to check if
    *                            the sleep duration has expired.
    * Returns:   ---
    * NonBlockingSleep provides a way to put functionality to sleep
    * for a time, to wait for finished execution of something else,
    * without blocking the GUI itself.
    */
    void NonBlockingSleep(int sleepDuration = 50, int checkInterval = 10);

private:
    Ui::PineSaveGameEditorClass ui;

    QString saveGameDir = "";
    QJsonArray inventory;

    AffinityWheel* aw = Q_NULLPTR;
    NewItemDialog* nid = Q_NULLPTR;
    
    /* Function: ---
    * Arguments: ---
    * Returns:   ---
    * ---
    */
    void ChangeTableRowColors();

    /* Function: ---
    * Arguments: ---
    * Returns:   ---
    * ---
    */
    void GetSaveFiles();

    void resizeEvent(QResizeEvent* event) override;

    const QColor gold = QColor::fromRgb(255, 215, 0);
    const QColor pineGreen = QColor::fromRgb(110, 177, 131, 210);
    const QColor gooderGreen = QColor::fromRgb(89, 145, 121, 210);
    const QColor orange = QColor::fromRgb(255, 165, 0);
    const QColor greenish = QColor::fromRgb(137, 211, 94);

    QColor primaryRowColor = pineGreen;
    QColor alternateRowColor = gooderGreen;
    QColor textColor = gold;

    QList<SpeciesRelationship> speciesAffinityList;
    QList<PlayerItem> playerInventory;

private slots:

   /* Function: AddNewItemToInventory
   * Arguments: ---
   * Returns:   ---
   * Opens the NewItemDialog.
   */
   void AddNewItemToInventory();

   /* Function: AddSelectedItems
   * Arguments: QList<PineItem>
   * Returns:   ---
   * Iterates through the list and adds the
   * selected items, from the NewItemDialog,
   * to the player's inventory with an initial
   * item count of 1.
   */
   void AddSelectedItems(QList<PineItem> newItems);

   /* Function: InitialSaveGameLocationChooser
   * Arguments: ---
   * Returns:   ---
   * If into.txt doesn't exist, a message box will
   * appear prompting the user to select a location
   * where the save files for the game are located.
   * Otherwise, info.txt is read and save game files
   * are put into the save game list.
   * This only occurs on startup.
   */
   void InitialSaveGameLocationChooser();

   /* Function: NewColor
   * Arguments: ---
   * Returns:   ---
   * Changes the table theme to the selected color
   * based on which item was chosen to change and
   * then updates the table to reflect the change.
   */
   void NewColor();

   /* Function: OpenSaveFile
   * Arguments: ---
   * Returns:   ---
   * Opens the current selected save file.
   */
   void OpenSaveFile();

   /* Function: SaveGameLocationChooser
   * Arguments: ---
   * Returns:   ---
   * Allows the user to select the directory
   * that contains the location of the save games.
   */
   void SaveGameLocationChooser();

   /* Function: SaveToFile
   * Arguments: ---
   * Returns:   ---
   * Saves all information to the
   * currently selected save file.
   */
   void SaveToFile();

   /* Function: SetAffinity
   * Arguments: ---
   * Returns:   ---
   * Sets the affinity between the selected
   * species to the chosen value.
   */
   void SetAffinity();

   /* Function: SetItemCount
   * Arguments: ---
   * Returns:   ---
   * Iterates through the table's rows and sets
   * the item count to what is in the row. Item
   * counts for certain items will not exceed
   * certain values. I.e., equipment will not
   * exceed an item count of 1.
   */
   void SetItemCount();

   /* Function: SetStatusMessage
   * Arguments: ---
   * Returns:   ---
   * Stops the message timer and then proceeds to
   * display status messages every second until
   * the messageQueue is empty.
   */
   void SetStatusMessage();

   /* Function: ShowAffinity
   * Arguments: index
   * Returns:   ---
   * Sets the affinity spin box to the selected
   * affinity between the two selected species.
   */
   void ShowAffinity(int index);
};
