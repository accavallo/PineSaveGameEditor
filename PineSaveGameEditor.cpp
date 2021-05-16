#include "PineSaveGameEditor.h"

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QJSONArray>
#include <QJSONDocument>
#include <QJSONObject>
#include <QJSONValue>
#include <QMessageBox>
#include <QScrollBar>
#include <QTimer>

#include "ItemInfo.h"

PineSaveGameEditor::PineSaveGameEditor(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.textEdit->verticalScrollBar(), SIGNAL(valueChanged(int)), ui.keyEdit->verticalScrollBar(), SLOT(setValue(int)));
    connect(ui.keyEdit->verticalScrollBar(), SIGNAL(valueChanged(int)), ui.textEdit->verticalScrollBar(), SLOT(setValue(int)));

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
   ui.listWidget->clear();
   ui.label->setText(saveGameDir);
   //Get all the .pine files in that directory
   QDir saveDir(saveGameDir, "*.pine");
   ui.listWidget->addItems(saveDir.entryList());
}

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
      ui.keyEdit->append(QString("Key %3 of %4").arg(QString::number(index + 1), QString::number(keyCount)));
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
   ui.textEdit->clear();
   ui.keyEdit->clear();
   QFile saveFile(QString("%1/%2").arg(saveGameDir, ui.listWidget->currentItem()->text()), this);
   if (!saveFile.open(QIODevice::ReadOnly | QIODevice::Text))
   {
      QMessageBox mb(this);
      mb.setText(QString("Unable to open:\n%1/%2").arg(saveGameDir, ui.listWidget->currentItem()->text()));
      mb.exec();
      return;
   }

   QByteArrayList arrList = saveFile.readAll().split('\n');
   for (int index = 0; index < arrList.size(); ++index)
   {
      QByteArray bArray = arrList[index];
      QJsonDocument jDoc = QJsonDocument::fromJson(bArray);
      QJsonObject jObj = jDoc.object();
      
      if (!jObj.isEmpty())
      {
         ReadJSONObject(jObj);
      }
   }

   saveFile.close();
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