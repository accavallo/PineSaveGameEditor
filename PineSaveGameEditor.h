#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PineSaveGameEditor.h"

#include "ItemInfo.h"

class PineSaveGameEditor : public QMainWindow
{
    Q_OBJECT

public:
    PineSaveGameEditor(QWidget *parent = Q_NULLPTR);

private:
    Ui::PineSaveGameEditorClass ui;

    QString saveGameDir = "";

    void GetSaveFiles();
    void PrintItemsForType(PineItemType type);
    void ReadJSONObject(QJsonObject obj, int depth = 0);
    void SaveToFile();

    QList <QColor> depthColors = 
    {
       QColor(0, 0, 0), QColor(125, 0, 0), QColor(0, 125, 0), QColor(0, 0, 125), QColor(50, 50, 50),
       QColor(125, 125, 125), QColor(150, 150, 150), QColor(175, 175, 175), QColor(200, 200, 200), QColor(85, 170, 255)
    };

private slots:
   void InitialSaveGameLocationChooser();
   void OpenSaveFile();
   void SaveGameLocationChooser();
};
