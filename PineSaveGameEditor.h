#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PineSaveGameEditor.h"

#include <QJsonArray>

#include "ItemInfo.h"

QT_FORWARD_DECLARE_CLASS(AffinityWheel)
QT_FORWARD_DECLARE_CLASS(NewItemDialog)

class PineSaveGameEditor : public QMainWindow
{
    Q_OBJECT

public:
    PineSaveGameEditor(QWidget *parent = Q_NULLPTR);

private:
    Ui::PineSaveGameEditorClass ui;

    QString saveGameDir = "";
    QJsonArray inventory;

    AffinityWheel* aw = Q_NULLPTR;
    NewItemDialog* nid = Q_NULLPTR;

    void GetSaveFiles();
    void SetStatusMessage(QString message);

    void resizeEvent(QResizeEvent* event) override;

    QList <QColor> depthColors = 
    {
       QColor(0, 0, 0), QColor(125, 0, 0), QColor(0, 125, 0), QColor(0, 0, 125), QColor(50, 50, 50),
       QColor(125, 125, 125), QColor(150, 150, 150), QColor(175, 175, 175), QColor(200, 200, 200), QColor(85, 170, 255)
    };

    //QList<PineItem> playerInventory;

private slots:
   void AddNewItemToInventory();
   void AddSelectedItems(QList<PineItem> newItems);
   void InitialSaveGameLocationChooser();
   void OpenSaveFile();
   void SaveGameLocationChooser();
   void SaveToFile();
   void SetAffinity();
   void SetItemCount();
   void ShowAffinity(int index);
};
