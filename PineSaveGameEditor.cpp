#include "PineSaveGameEditor.h"

#include <QColorDialog>
#include <QComboBox>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QJSONDocument>
#include <QJSONObject>
#include <QJSONValue>
#include <QMessageBox>
#include <QQueue>
#include <QResizeEvent>
#include <QScrollBar>
#include <QSpinBox>
#include <QStatusBar>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTime>
#include <QTimer>

#include "AffinityWheel.h"
#include "NewItemDialog.h"

namespace
{
   QTimer messageTimer;
   QQueue<QString> messageQueue;
}

PineSaveGameEditor::PineSaveGameEditor(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.affinitySpinBox, SIGNAL(valueChanged(double)), ui.affinityWheel, SLOT(SetCenter(double)));
    connect(&messageTimer, SIGNAL(timeout()), this, SLOT(SetStatusMessage()));

    ui.tableWidget->setColumnCount(5);
    double baseSize = ui.tableWidget->width() * 0.47;

    //baseSize was originally 820
    ui.tableWidget->setColumnWidth(0, 173/*baseSize * 0.45*/);
    ui.tableWidget->setColumnWidth(1, 77/*baseSize * 0.2*/);
    ui.tableWidget->setColumnWidth(2, 77/*baseSize * 0.2*/);
    ui.tableWidget->setColumnWidth(3, 77/*baseSize * 0.2*/);
    ui.tableWidget->setColumnWidth(4, baseSize * 2 * 0.4);
    ui.tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui.tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui.tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);

    nid = new NewItemDialog(this);

    connect(nid, SIGNAL(AddSelectedItems(QList<PineItem>)), this, SLOT(AddSelectedItems(QList<PineItem>)));
    connect(ui.actionText_Color, SIGNAL(triggered()), this, SLOT(NewColor()));
    connect(ui.actionPrimary_Row_Color, SIGNAL(triggered()), this, SLOT(NewColor()));
    connect(ui.actionAlternate_Row_Color, SIGNAL(triggered()), this, SLOT(NewColor()));

    connect(ui.actionReset_Theme, &QAction::triggered, [this] { primaryRowColor = pineGreen; alternateRowColor = gooderGreen; textColor = gold; ChangeTableRowColors(); });

    setWindowTitle("Pine Save Game Editor");
    messageTimer.start(2000);
    //Save this for LAST
    QTimer::singleShot
    (
       500,
       [this] 
       {
          InitialSaveGameLocationChooser();
       }
    );
}

void PineSaveGameEditor::NonBlockingSleep(int sleepDuration, int checkInterval)
{
   QTime stopTime = QTime::currentTime().addMSecs(sleepDuration);
   while (QTime::currentTime() < stopTime)
   {
      QCoreApplication::processEvents(QEventLoop::AllEvents, checkInterval);
   }
}

//Private Functions

void PineSaveGameEditor::ChangeTableRowColors()
{
   for (int index = 0; index < ui.tableWidget->rowCount(); index++)
   {
      for (int ind = 0; ind < ui.tableWidget->columnCount(); ind++)
      {
         QColor background;
         if (index % 2)
         {
            background = alternateRowColor;
         }
         else
         {
            background = primaryRowColor;
         }
         ui.tableWidget->item(index, ind)->setBackground(background);
         ui.tableWidget->item(index, ind)->setTextColor(textColor);
      }
   }
}

void PineSaveGameEditor::GetSaveFiles()
{
   ui.saveFileList->clear();
   ui.label->setText(saveGameDir);
   //Get all the .pine files in that directory
   QDir saveDir(saveGameDir, "*.pine");
   ui.saveFileList->addItems(saveDir.entryList());
}

void PineSaveGameEditor::resizeEvent(QResizeEvent* event)
{
   ui.tableWidget->setGeometry(ui.tableWidget->x(), ui.tableWidget->y(), event->size().width(), ui.tableWidget->height());

   QWidget::resizeEvent(event);
   update();
}

//Private Slots

void PineSaveGameEditor::AddNewItemToInventory()
{
   nid->show();
}

void PineSaveGameEditor::AddSelectedItems(QList<PineItem> newItems)
{
   QList<PineItem> baseList;
   QTableWidgetItem* protoItem = new QTableWidgetItem();
   protoItem->setFlags(Qt::ItemFlag::NoItemFlags);

   foreach(PineItem pi, newItems)
   {
      PlayerItem p;
      p.item = pi;
      p.count = 1;
      playerInventory.append(p);
      baseList.append(pi);

      ui.tableWidget->insertRow(ui.tableWidget->rowCount());

      //Set the 'Item Name' column
      QTableWidgetItem* nameItem = protoItem->clone();
      nameItem->setText(pi.itemName);
      ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, 0, nameItem);
      //Set the 'Amount' column
      ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(1)));
      //Set the 'Max Per Stack' column
      QTableWidgetItem* maxItem = protoItem->clone();
      maxItem->setText(QString::number(pi.maxStack));
      ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, 2, maxItem);
      //Set the 'Item Type' column
      QTableWidgetItem* typeItem = protoItem->clone();
      typeItem->setText(itemNameMap.find(pi.itemType).value());
      ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, 3, typeItem);
      //Set the 'Item Description' column
      QTableWidgetItem* descriptionItem = protoItem->clone();
      descriptionItem->setText(pi.itemDescription);
      ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, 4, descriptionItem);
   }

   foreach(PlayerItem pi, playerInventory)
   {
      baseList.append(pi.item);
   }

   nid->UpdateList(baseList);
   ChangeTableRowColors();
}

void PineSaveGameEditor::InitialSaveGameLocationChooser()
{
   QString currentPath = QDir::currentPath();

   //See if file exists first
   QFile info(QString("%1/info.txt").arg(currentPath), this);
   if (!info.open(QIODevice::ReadOnly))
   {
      QMessageBox mb(this);
      mb.setStyleSheet("color: black;");
      mb.setWindowTitle("Choose Save Game Directory");
      mb.setText("The next window will ask you to choose the save file location for Pine.\n(You can always choose later by pressing cancel\nthen going to File -> Select Save Directory.)");
      mb.exec();
      SaveGameLocationChooser();
      return;
   }

   saveGameDir = info.readAll();

   GetSaveFiles();
}

void PineSaveGameEditor::NewColor()
{
   QAction* action = qobject_cast<QAction*>(sender());

   QColorDialog cd(this);
   cd.setStyleSheet("color: black;");
   if (cd.exec() != QDialog::Accepted)
   {
      return;
   }
   QColor newColor = cd.selectedColor();

   if (action == ui.actionText_Color)
   {
      textColor = newColor;
   }
   else if (action == ui.actionPrimary_Row_Color)
   {
      primaryRowColor = newColor;
   }
   else if (action == ui.actionAlternate_Row_Color)
   {
      alternateRowColor = newColor;
   }
   ChangeTableRowColors();
}

void PineSaveGameEditor::OpenSaveFile()
{
   QFile saveFile(QString("%1/%2").arg(saveGameDir, ui.saveFileList->currentItem()->text()), this);
   if (!saveFile.open(QIODevice::ReadOnly | QIODevice::Text))
   {
      QMessageBox mb(this);
      mb.setText(QString("Unable to open:\n%1/%2").arg(saveGameDir, ui.saveFileList->currentItem()->text()));
      mb.exec();
      return;
   }

   //Consider doing some type of busy indicator
   for (int i = ui.tableWidget->rowCount() - 1; i >= 0; i--)
   {
      ui.tableWidget->removeRow(i);
   }
   speciesAffinityList.clear();
   ui.speciesComboBox->clear();
   ui.affinitySpinBox->setValue(0);
   ui.affinitySpinBox->setValue(0.0);
   ui.applyAffinityBtn->setEnabled(true);
   ui.applyItemBtn->setEnabled(true);
   ui.affinitySpinBox->setEnabled(true);
   ui.saveToFileBtn->setEnabled(true);
   ui.addItemBtn->setEnabled(true);
   playerInventory.clear();

   QByteArrayList arrList = saveFile.readAll().split('\n');
   saveFile.close();

   QList<PineItem> baseList;
   for (int index = 0; index < arrList.size(); ++index)
   {
      QByteArray bArray = arrList[index];
      QJsonDocument jDoc = QJsonDocument::fromJson(bArray);
      QJsonObject jObj = jDoc.object();

      //Get the player inventory:
      QJsonValue val = jObj.value("playerData");
      QJsonObject playerData = val.toObject();
      if (playerData.isEmpty())
      {
         continue;
      }

      inventory = playerData["inventory"].toArray();
      QTableWidgetItem* protoItem = new QTableWidgetItem();
      protoItem->setFlags(Qt::ItemFlag::NoItemFlags);
      for (int index = 0; index < inventory.count(); ++index)
      {
         int itemIndex = inventory[index].toObject().value("id").toObject().value("value").toInt();
         int amount = inventory[index].toObject().value("amount").toInt();
         playerInventory.append(PlayerItem{ pineItemList[itemIndex], amount });
         baseList.append(pineItemList[itemIndex]);
         //ui.itemList->addItem(pineItemList[itemIndex].itemName);

         int itemNameLen = pineItemList[itemIndex].itemName.length();
         QString spaceString = "";
         for (int spaceInd = itemNameLen; spaceInd < 30; spaceInd++)
         {
            spaceString.append(" ");
         }
         ui.tableWidget->insertRow(ui.tableWidget->rowCount());

         //Set the 'Item Name' column
         QTableWidgetItem* nameItem = protoItem->clone();
         nameItem->setText(pineItemList[itemIndex].itemName);
         ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, 0, nameItem);
         //Set the 'Amount' column
         ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(amount)));
         //Set the 'Max Per Stack' column
         QTableWidgetItem* maxItem = protoItem->clone();
         maxItem->setText(QString::number(pineItemList[itemIndex].maxStack));
         ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, 2, maxItem);
         //Set the 'Item Type' column
         QTableWidgetItem* typeItem = protoItem->clone();
         typeItem->setText(itemNameMap.find(pineItemList[itemIndex].itemType).value());
         ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, 3, typeItem);
         //Set the 'Item Description' column
         QTableWidgetItem* descriptionItem = protoItem->clone();
         descriptionItem->setText(pineItemList[itemIndex].itemDescription);
         ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, 4, descriptionItem);
      }
      nid->UpdateList(baseList);

      val = jObj.value("speciesAffinities");
      QJsonArray speciesAffinityArray = val.toArray();
      val = speciesAffinityArray[0].toObject();
      for (int index = 0; index < speciesAffinityArray.count(); ++index)
      {
         QJsonObject sObj = speciesAffinityArray[index].toObject();
         SpeciesRelationship relationship(
            SpeciesType(sObj.value("speciesA").toInt()),
            SpeciesType(sObj.value("speciesB").toInt()),
            sObj.value("affinity").toDouble()
         );
         speciesAffinityList.append(relationship);
         ui.speciesComboBox->addItem(QString("%1 and %2").arg(GetSpeciesName(relationship.GetSpeciesA()), GetSpeciesName(relationship.GetSpeciesB())));
      }
   }

   ui.tableWidget->resizeColumnToContents(4);
   ChangeTableRowColors();
}

void PineSaveGameEditor::SaveGameLocationChooser()
{
   saveGameDir = QFileDialog::getExistingDirectory(this, "Choose Pine save game location", saveGameDir);

   if (saveGameDir.isEmpty())
   {
      return;
   }

   GetSaveFiles();
}

void PineSaveGameEditor::SaveToFile()
{
   if(ui.saveFileList->currentItem() == Q_NULLPTR)
   {
      messageQueue.append("No save file selected!");
      return;
   }
   QFile file(QString("%1/%2").arg(saveGameDir, ui.saveFileList->currentItem()->text()));
   if (!file.open(QIODevice::ReadOnly))
   {
      QMessageBox::warning(this, "Error with file", QString("Unable to open save file %1 located in %2").arg(ui.saveFileList->currentItem()->text(), saveGameDir));
      file.close();
      return;
   }
   QStringList fileContents;
   while (!file.atEnd())
   {
      fileContents.append(file.readLine());
   }
   file.close();

   int index = 0;
   for (int i = 0; i < fileContents.count(); i++)
   {
      if (fileContents[i].contains("inventory"))
      {
         index = i;
         break;
      }
   }
   int inventoryOffset = fileContents[index].indexOf("inventory");

   //Start of the inventory
   int inventoryStart = fileContents[index].indexOf("[", inventoryOffset);
   //End of the inventory
   int inventoryEnd = fileContents[index].indexOf("],\"equippedItems\"", inventoryOffset);
   //Iterate through the playerInventory list and find the item id and update the amount
   QString newInventory = "";
   for(int index = 0; index < playerInventory.count(); index++)
   {
      PlayerItem pi = playerInventory[index];
      if (pi.item.itemChanged)
      {
         //Notify of item changes
         messageQueue.append(QString("Updating %1").arg(pi.item.itemName));
         playerInventory[index].item.itemChanged = false;
      }

      newInventory.append(QString("{\"id\":{\"value\":%1},\"amount\":%2},").arg(QString::number(pi.item.itemId), QString::number(pi.count)));
   }
   newInventory = newInventory.left(newInventory.lastIndexOf(','));
   fileContents[index].replace(inventoryStart + 1, inventoryEnd - inventoryStart - 1, newInventory);

   //Save the affinities between villages/species.
   //speciesAffinities
   int speciesAffinityOffset = fileContents[index].indexOf("\"speciesAffinities\"", inventoryOffset);
   int startAffinity = fileContents[index].indexOf("{", speciesAffinityOffset);
   int endAffinity = fileContents[index].indexOf("}],\"weather\"", startAffinity);
   QString affinity = fileContents[index].mid(startAffinity, endAffinity - startAffinity);

   QString newAffinity = "";
   foreach(SpeciesRelationship sr, speciesAffinityList)
   {
      newAffinity.append(QString("{\"speciesA\":%1,\"speciesB\":%2,\"affinity\":%3},")
         .arg(
            QString::number(sr.GetSpeciesA()),
            QString::number(sr.GetSpeciesB()),
            QString::number(sr.GetSpeciesAffinity(),'g', 16)
         ));
   }
   int charIndex = 0;
   newAffinity.remove(QRegularExpression(",$"));
   fileContents[index].replace(startAffinity, endAffinity - startAffinity + 1, newAffinity);

   if (!file.open(QIODevice::WriteOnly))
   {
      QMessageBox::warning(this, "Error with file", QString("Unable to open save file %1 located in %2").arg(ui.saveFileList->currentItem()->text(), saveGameDir));
      file.close();
      return;
   }
   foreach(QString str, fileContents)
   {
      file.write(str.toStdString().c_str());
   }

   file.close();
   messageQueue.append("Save complete.");
}

void PineSaveGameEditor::SetAffinity()
{
   QString friendliness = "";
   if (ui.affinitySpinBox->value() < -0.33)
   {
      friendliness = "Hated";
   }
   else if (ui.affinitySpinBox->value() > 0.33)
   {
      friendliness = "Friendly";
   }
   else
   {
      friendliness = "Neutral";
   }
   messageQueue.append(QString("Setting affinity between %1 to %2 (%3)").arg(ui.speciesComboBox->currentText(), QString::number(ui.affinitySpinBox->value()), friendliness));
   
   //Find the correct affinity to modify
   QStringList species = ui.speciesComboBox->currentText().split(' ');
   species.removeAt(1);

   //Get the new value and apply.
   for (int i = 0; i < speciesAffinityList.count(); i++)
   {
      SpeciesRelationship sr = speciesAffinityList[i];
      if ((QString::compare(GetSpeciesName(sr.GetSpeciesA()), species[0], Qt::CaseInsensitive) == 0) && (QString::compare(GetSpeciesName(sr.GetSpeciesB()), species[1], Qt::CaseInsensitive) == 0) ||
          (QString::compare(GetSpeciesName(sr.GetSpeciesB()), species[0], Qt::CaseInsensitive) == 0) && (QString::compare(GetSpeciesName(sr.GetSpeciesA()), species[1], Qt::CaseInsensitive) == 0))
      {
         speciesAffinityList[i].SetAffinity(ui.affinitySpinBox->value());
         break;
      }
   }
}

void PineSaveGameEditor::SetItemCount()
{
   int rowCount = ui.tableWidget->rowCount();
   int tableCount(0);
   for (int index = 0; index < ui.tableWidget->rowCount(); index++)
   {
      switch (playerInventory[index].item.itemType)
      {
         case FOOD:       case MATERIAL:
         case ESSENCE:    case MIXTURE:
         case TRAP:       case RAID_FLARE:
         case OTHER:      case CRAFTED:
         case SIMULATION: case AMMUNITION:
            tableCount = ui.tableWidget->item(index, 1)->text().toInt();
            if (tableCount != playerInventory[index].count)
            {
               playerInventory[index].count = tableCount;
               playerInventory[index].item.itemChanged = true;
            }
            break;
         case EQUIPMENT:
         case TOKEN:  case KEYS:
         case IDEA:   case IMPORTANT:
            if (ui.tableWidget->item(index, 1)->text().toInt() > 1)
            {
               playerInventory[index].count = 1;
               ui.tableWidget->item(index, 1)->setText("1");
               messageQueue.append(QString("Setting %1 to count of 1 due to type '%2'.").arg(playerInventory[index].item.itemName, itemNameMap.find(playerInventory[index].item.itemType).value()));
               update();
               ui.tableWidget->update();
            }
            break;
         case SORT_OF_ITEM:
         case GAME_BREAKING_ITEM:
         case NO_ITEM:
         default:
            //These items need to be removed from the inventory, the table,
            //and the index needs to be set to one less than current.
            playerInventory.removeAt(index);
            ui.tableWidget->removeRow(index);
            messageQueue.append(QString("Removing %1 from the inventory list.").arg(playerInventory[index].item.itemName));
            index--;
      }
   }
}

void PineSaveGameEditor::SetStatusMessage()
{
   messageTimer.stop();
   while (!messageQueue.isEmpty())
   {
      ui.statusBar->showMessage(messageQueue.takeFirst(), 1000);
      NonBlockingSleep(1000, 100);
   }
   messageTimer.start(500);
}

void PineSaveGameEditor::ShowAffinity(int index)
{
   if (index < 0)
   {
      return;
   }
   ui.affinitySpinBox->setValue(speciesAffinityList[index].GetSpeciesAffinity());
}
