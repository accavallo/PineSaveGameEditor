#include "PineSaveGameEditor.h"

#include <QComboBox>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QJSONDocument>
#include <QJSONObject>
#include <QJSONValue>
#include <QMessageBox>
#include <QScrollBar>
#include <QSpinBox>
#include <QTimer>

#include "AffinityWheel.h"
#include "ItemInfo.h"

#include <QDebug>

namespace
{
   void readJson();
   QList<SpeciesRelationship> speciesAffinityList;
}

PineSaveGameEditor::PineSaveGameEditor(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.affinitySpinBox, SIGNAL(valueChanged(double)), ui.affinityWheel, SLOT(SetCenter(double)));
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

//void PineSaveGameEditor::PrintItemsForType(PineItemType type)
//{
//   ui.textEdit->append(QString("Showing all %1 items.").arg(itemNameMap.value(type)));
//   foreach(PineItem item, pineItemList)
//   {
//      if (item.itemType == type)
//      {
//         ui.textEdit->append(QString("Item %1: %2").arg(QString::number(item.itemId), item.itemName));
//      }
//   }
//   ui.textEdit->append("");
//}

void PineSaveGameEditor::ReadJSONObject(QJsonObject obj, int depth)
{
   QString prependSpaces = "";
   int d = 0;
   while (d < depth)
   {
      d++;
      prependSpaces.append("    ");
   }
   
   int keyCount = obj.keys().count();
   //ui.textEdit->append(QString("%2Object key count: %1").arg(QString::number(keyCount), prependSpaces));
   //foreach(QString key, obj.keys())
   for (int index = 0; index < keyCount; ++index)
   {
      QString key = obj.keys()[index];
      ui.textEdit->setTextColor(depthColors[depth % 10]);
      ui.textEdit->append(QString("%1%2").arg(prependSpaces, key));
      //ui.keyEdit->append(QString("Key %3 of %4").arg(QString::number(index + 1), QString::number(keyCount)));
      QJsonValue val = obj.value(key);
      QJsonObject jObj = val.toObject();
      if (!jObj.isEmpty() && jObj.keys().count() > 0)
      {
         ReadJSONObject(jObj, depth + 1);
      }
      else
      {
         //ui.textEdit->append(QString("    %1%2").arg(prependSpaces, val.toString()));
         //QJsonArray jArr = val.toArray();
         //ui.textEdit->append(QString("This should be an array: %1").arg(val.toString()));
         QJsonArray jArr = obj[key].toArray();
         ui.textEdit->setTextColor(depthColors[(depth + 1) % 10]);
         foreach(QJsonValue jVal, jArr)
         {
            QJsonObject lastObj = jVal.toObject();
            if (!lastObj.isEmpty())
            {
               ReadJSONObject(lastObj, depth + 1);
            }
            //ui.textEdit->append(QString("Key count: %1").arg(QString::number(lastObj.keys().count())));
            //if (!jVal.toString().isEmpty())
            //{
            //   ui.textEdit->append(QString("    %1%2").arg(prependSpaces, jVal.toString()));
            //}
         }
      }
      //foreach(QString key2, jObj.keys())
      //{
      //   ui.textEdit->append(QString("%1%2").arg(prependSpaces, key2));

      //}
      //QJsonArray jArr = val.toArray();
      //foreach(QJsonValue jVal, jArr)
      //{
      //   ui.textEdit->append(QString("\t%1").arg(jVal.toString()));
      //}
   }
}

void readJson()
{

   /* test.json */
   //{
   //   "appDesc": {
   //      "description": "SomeDescription",
   //         "message" : "SomeMessage"
   //   },
   //      "appName" : {
   //         "description": "Home",
   //            "message" : "Welcome",
   //            "imp" : ["awesome", "best", "good"]
   //      }
   //}
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
   QString currentPath = QDir::currentPath();
   QFile outfile(QString("%1/info.txt").arg(currentPath), this);
   if (!outfile.open(QIODevice::WriteOnly))
   {
      QMessageBox mb(this);
      mb.setText(QString("Unable to save info.txt to %1").arg(currentPath));
      mb.exec();
   }
   else
   {
      outfile.write(saveGameDir.toUtf8());
      outfile.close();
   }
}

//Private Slots

void PineSaveGameEditor::OpenSaveFile()
{
   //Consider doing some type of busy indicator
   ui.textEdit->clear();
   speciesAffinityList.clear();
   ui.speciesComboBox->clear();
   ui.affinitySpinBox->setValue(0.0);
   ui.itemList->clear();
   ui.applyAffinityBtn->setEnabled(true);
   ui.applyItemBtn->setEnabled(true);
   ui.itemCountBox->setEnabled(true);
   ui.affinitySpinBox->setEnabled(true);
   QFile saveFile(QString("%1/%2").arg(saveGameDir, ui.saveFileList->currentItem()->text()), this);
   if (!saveFile.open(QIODevice::ReadOnly | QIODevice::Text))
   {
      QMessageBox mb(this);
      mb.setText(QString("Unable to open:\n%1/%2").arg(saveGameDir, ui.saveFileList->currentItem()->text()));
      mb.exec();
      return;
   }

   QByteArrayList arrList = saveFile.readAll().split('\n');
   saveFile.close();

   for (int index = 0; index < arrList.size(); ++index)
   {
      QByteArray bArray = arrList[index];
      QJsonDocument jDoc = QJsonDocument::fromJson(bArray);
      QJsonObject jObj = jDoc.object();
      
      //Get the player inventory:
      QJsonValue val = jObj.value("playerData");
      qDebug() << "playerData as val: " << val;
      QJsonObject playerData = val.toObject();
      if (playerData.isEmpty())
      {
         continue;
      }

      qDebug() << "playerData as object: " << playerData;

      inventory = playerData["inventory"].toArray();
      ui.textEdit->append("Player inventory:");
      ui.textEdit->append("     Item Name\t         Amount");
      for (int index = 0; index < inventory.count(); ++index)
      {
         int itemIndex = inventory[index].toObject().value("id").toObject().value("value").toInt();

         ui.itemList->addItem(pineItemList[itemIndex].itemName);
         
         int itemNameLen = pineItemList[itemIndex].itemName.length();
         QString spaceString = "";
         for (int spaceInd = itemNameLen; spaceInd < 30; spaceInd++)
         {
            spaceString.append(" ");
         }
         ui.textEdit->append(QString("%1%3 \t%2").arg(pineItemList[itemIndex].itemName, QString::number(inventory[index].toObject().value("amount").toInt()), spaceString));
      }

      val = jObj.value("speciesAffinities");
      qDebug() << "speciesAffinities as val: " << val;
      QJsonArray speciesAffinityArray = val.toArray();
      qDebug() << "val as array: " << speciesAffinityArray;
      val = speciesAffinityArray[0].toObject();
      qDebug() << "array[0] as val: " << val;
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
      //if (!jObj.isEmpty())
      //{
      //   ReadJSONObject(jObj);
      //}
   }
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
   SaveToFile();
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
   ui.textEdit->append(QString("Setting affinity between %1 to %2 (%3)").arg(ui.speciesComboBox->currentText(), QString::number(ui.affinitySpinBox->value())));
}

void PineSaveGameEditor::SetItemCount()
{
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
   ui.itemCountBox->setValue(inventory[index].toObject().value("amount").toInt());
}
