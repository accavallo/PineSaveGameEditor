#include "PineSaveGameEditor.h"

#include <QComboBox>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QJSONDocument>
#include <QJSONObject>
#include <QJSONValue>
#include <QMessageBox>
#include <QResizeEvent>
#include <QScrollBar>
#include <QSpinBox>
#include <QStatusBar>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTimer>

#include "AffinityWheel.h"
#include "ItemInfo.h"

#include <QDebug>

namespace
{
   void readJson();
   QList<SpeciesRelationship> speciesAffinityList;
   QList<PlayerItem> playerInventory;
}

PineSaveGameEditor::PineSaveGameEditor(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.affinitySpinBox, SIGNAL(valueChanged(double)), ui.affinityWheel, SLOT(SetCenter(double)));

    ui.tableWidget->setColumnCount(5);
    double baseSize = ui.tableWidget->width() * 0.47;

    //baseSize was originally 820

    

    ui.tableWidget->setColumnWidth(0, 173/*baseSize * 0.45*/);
    ui.tableWidget->setColumnWidth(1, 77/*baseSize * 0.2*/);
    ui.tableWidget->setColumnWidth(2, 77/*baseSize * 0.2*/);
    ui.tableWidget->setColumnWidth(3, 77/*baseSize * 0.2*/);
    ui.tableWidget->setColumnWidth(4, baseSize * 2 * 0.4);
    //ui.tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui.tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui.tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui.tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);

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

//Private Functions

void PineSaveGameEditor::GetSaveFiles()
{
   ui.saveFileList->clear();
   ui.label->setText(saveGameDir);
   //Get all the .pine files in that directory
   QDir saveDir(saveGameDir, "*.pine");
   ui.saveFileList->addItems(saveDir.entryList());
}

//void PineSaveGameEditor::ReadJSONObject(QJsonObject obj, int depth)
//{
//   int keyCount = obj.keys().count();
//   for (int index = 0; index < keyCount; ++index)
//   {
//      QString key = obj.keys()[index];
//      QJsonValue val = obj.value(key);
//      QJsonObject jObj = val.toObject();
//      if (!jObj.isEmpty() && jObj.keys().count() > 0)
//      {
//         ReadJSONObject(jObj, depth + 1);
//      }
//      else
//      {
//         QJsonArray jArr = obj[key].toArray();
//         foreach(QJsonValue jVal, jArr)
//         {
//            QJsonObject lastObj = jVal.toObject();
//            if (!lastObj.isEmpty())
//            {
//               ReadJSONObject(lastObj, depth + 1);
//            }
//         }
//      }
//   }
//}

void readJson()
{
   QString val;
   QFile file;
   file.setFileName("test.json");
   file.open(QIODevice::ReadOnly | QIODevice::Text);
   val = file.readAll();
   file.close();
   qWarning() << val;
   QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
   QJsonObject sett2 = d.object();
   QJsonValue value = sett2.value(QString("appName"));
   qWarning() << value;
   QJsonObject item = value.toObject();
   qWarning() << ("QJsonObject of description: ") << item;

   /* in case of string value get value and convert into string*/
   qWarning() << ("QJsonObject[appName] of description: ") << item["description"];
   QJsonValue subobj = item["description"];
   qWarning() << subobj.toString();

   /* in case of array get array and convert into string*/
   qWarning() << ("QJsonObject[appName] of value: ") << item["imp"];
   QJsonArray test = item["imp"].toArray();
   qWarning() << test[1].toString();
}

void PineSaveGameEditor::SaveToFile()
{
   if(ui.saveFileList->currentItem() == Q_NULLPTR)
   {
      SetStatusMessage("No save file selected!");
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

   //Iterate through the playerInventory list and find the item id and update the amount
   foreach(PlayerItem pi, playerInventory)
   {
      SetStatusMessage(QString("Updating %1").arg(pi.item.itemName));
      update();
      QString itemToUpdate = QString("\"id\":{\"value\":%1},\"amount\":").arg(pi.item.itemId);
      int itemOffset = fileContents[index].indexOf(itemToUpdate, inventoryOffset);
      //Check if the itemOffset value is even there. (an item was added to the inventory list)

      int countLen = fileContents[index].indexOf("}", itemOffset + itemToUpdate.length()) - itemOffset;

      
      QString newItem = QString("%1%2}").arg(itemToUpdate, QString::number(pi.count));
      fileContents[index].replace(itemOffset, countLen+1, newItem);

      inventoryOffset = itemOffset;
   }

   //Save the affinities between villages/species.
   //speciesAffinities
   int speciesAffinityOffset = fileContents[index].indexOf("\"speciesAffinities\"", inventoryOffset);
   int startAffinity = fileContents[index].indexOf("{", speciesAffinityOffset);
   //int affinityOffset = fileContents[index].indexOf("\"affinity\":", startAffinity);
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
   SetStatusMessage("Save complete.");
}

void PineSaveGameEditor::SetStatusMessage(QString message)
{
   ui.statusBar->showMessage(message, 5000);
}

void PineSaveGameEditor::resizeEvent(QResizeEvent* event)
{
   ui.tableWidget->setGeometry(ui.tableWidget->x(), ui.tableWidget->y(), event->size().width(), ui.tableWidget->height());

   qDebug() << event->size().width();

   QWidget::resizeEvent(event);
   update();
}

//Private Slots

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
         ui.tableWidget->setItem(ui.tableWidget->rowCount()-1, 0, nameItem);
         //Set the 'Amount' column
         ui.tableWidget->setItem(ui.tableWidget->rowCount()-1, 1, new QTableWidgetItem(QString::number(amount)));
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

      val = jObj.value("speciesAffinities");
      QJsonArray speciesAffinityArray = val.toArray();
      val = speciesAffinityArray[0].toObject();
      for(int index = 0; index < speciesAffinityArray.count(); ++index)
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
}

void PineSaveGameEditor::InitialSaveGameLocationChooser()
{
   QString currentPath = QDir::currentPath();

   //See if file exists first
   QFile info(QString("%1/info.txt").arg(currentPath), this);
   if (!info.open(QIODevice::ReadOnly))
   {
      QMessageBox mb(this);
      mb.setText("The next window will ask you to choose the save file location for Pine.\n(You can always choose later by pressing cancel\nthen going to File -> Select Save Directory.)");
      mb.exec();
      SaveGameLocationChooser();
      return;
   }

   saveGameDir = info.readAll();

   GetSaveFiles();
}

void PineSaveGameEditor::SaveGameLocationChooser()
{
   saveGameDir = QFileDialog::getExistingDirectory(this, "Choose Pine save game location");

   if (saveGameDir.isEmpty())
   {
      return;
   }

   GetSaveFiles();
   SaveToFile();
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
   SetStatusMessage(QString("Setting affinity between %1 to %2 (%3)").arg(ui.speciesComboBox->currentText(), QString::number(ui.affinitySpinBox->value()), friendliness));
   
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
   int tableRowCount = ui.tableWidget->rowCount();
   QString data = "";
   for (int index = 0; index < tableRowCount; index++)
   {
      playerInventory[index].count = ui.tableWidget->item(index, 1)->text().toInt();
   }
}

void PineSaveGameEditor::ShowAffinity(int index)
{
   if (index < 0)
   {
      return;
   }
   ui.affinitySpinBox->setValue(speciesAffinityList[index].GetSpeciesAffinity());
}

void PineSaveGameEditor::ShowItemCount(int index)
{
   if (index < 0)
   {
      return;
   }
   //ui.itemCountBox->setValue(inventory[index].toObject().value("amount").toInt());
}
