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

    void ChangeTableRowColors();
    void GetSaveFiles();
    void SetStatusMessage(QString message);

    void resizeEvent(QResizeEvent* event) override;

    const QColor gold = QColor::fromRgb(255, 215, 0);
    const QColor pineGreen = QColor::fromRgb(110, 177, 131, 210);
    const QColor gooderGreen = QColor::fromRgb(89, 145, 121, 210);
    const QColor orange = QColor::fromRgb(255, 165, 0);
    const QColor greenish = QColor::fromRgb(137, 211, 94);

    QColor primaryRowColor = pineGreen;
    QColor alternateRowColor = gooderGreen;
    QColor textColor = gold;

private slots:
   void AddNewItemToInventory();
   void AddSelectedItems(QList<PineItem> newItems);
   void InitialSaveGameLocationChooser();
   void NewColor();
   void OpenSaveFile();
   void SaveGameLocationChooser();
   void SaveToFile();
   void SetAffinity();
   void SetItemCount();
   void ShowAffinity(int index);
};
