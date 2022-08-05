#pragma once

#include <QDialog>
#include "ui_NewItemDialog.h"

#include "GameInfo.h"

class NewItemDialog : public QDialog
{
   Q_OBJECT

public:
   NewItemDialog(QWidget *parent = nullptr);
   ~NewItemDialog();

   void UpdateList(QList<PineItem> playerList);

private:
   Ui::NewItemDialogClass ui;

signals:
   void AddSelectedItems(QList<PineItem>);

private slots:
   void CompileSelectedItems();
};
