#include "NewItemDialog.h"

#include <QListWidget>
#include <QScrollBar>

NewItemDialog::NewItemDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    foreach(PineItem pi, pineItemList)
    {
       if (pi.itemType != GAME_BREAKING_ITEM && pi.itemType != NO_ITEM)
       {
          QListWidgetItem* item = new QListWidgetItem(QString("%1 (%2)").arg(pi.itemName, itemNameMap.find(pi.itemType).value()));
          item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
          item->setCheckState(Qt::Unchecked);
          ui.listWidget->addItem(item);
       }
    }
    ui.listWidget->sortItems();
    ui.listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(CompileSelectedItems()));
    setWindowTitle("Add New Item");
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
}

NewItemDialog::~NewItemDialog()
{}

void NewItemDialog::UpdateList(QList<PineItem> playerList)
{
   ui.listWidget->clear();
   QList<PineItem> baseList = pineItemList;
   foreach(PineItem pi, playerList)
   {
      for (int index = baseList.count() - 1; index >= 0; index--)
      {
         if (pi.itemId == baseList[index].itemId)
         {
            baseList.removeAt(index);
         }
      }
   }
   foreach(PineItem pi, baseList)
   {
      if (pi.itemType != GAME_BREAKING_ITEM && pi.itemType != NO_ITEM)
      {
         QListWidgetItem* item = new QListWidgetItem(QString("%1 (%2)").arg(pi.itemName, itemNameMap.find(pi.itemType).value()));
         item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
         item->setCheckState(Qt::Unchecked);
         ui.listWidget->addItem(item);
      }
   }
   ui.listWidget->sortItems();
}

void NewItemDialog::CompileSelectedItems()
{
   QListWidgetItem* item = Q_NULLPTR;

   QList<PineItem> addThese;
   for (int i = 0; i < ui.listWidget->count(); i++)
   {
      item = ui.listWidget->item(i);
      if (item->checkState() == Qt::Checked)
      {
         //Find the item in question.
         QString itemName = item->text().left(item->text().indexOf("(") - 1);
         foreach(PineItem pi, pineItemList)
         {
            if (QString::compare(pi.itemName, itemName) == 0)
            {
               addThese.append(pi);
               break;
            }
         }
      }
      item->setCheckState(Qt::Unchecked);
   }
   emit AddSelectedItems(addThese);
   this->close();
}