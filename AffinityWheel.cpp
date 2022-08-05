#include "AffinityWheel.h"

#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>

AffinityWheel::AffinityWheel(QWidget *parent)
    : QLabel(parent)
{}

AffinityWheel::~AffinityWheel()
{}

void AffinityWheel::SetCenter(double center)
{
   //Take the 'center' of the triangle which is based off the affinity value.
   //But since the value goes from -1 to 1, we need to take the width / 2 and multiply that by the affinity value,
   //THEN add the width / 2 to that value to properly draw the triangle.
   this->center = center * this->width() / 2 + this->width() / 2;
   update();
}

void AffinityWheel::paintEvent(QPaintEvent* event)
{
   QPainter painter(this);
   
   QPolygon poly;
   poly.append(QPoint(center - 5, 0));
   poly.append(QPoint(center, 10));
   poly.append(QPoint(center + 5, 0));

   QPainterPath path;
   path.addPolygon(poly);
   path.closeSubpath();

   QBrush brush(QColor::fromRgb(255, 170, 0), Qt::BrushStyle::SolidPattern);

   painter.fillPath(path, brush);

   static bool firstTime(true);
   if (firstTime && this->isVisible())
   {
      //Draw each of the affinities' text in the middle
      //of their respective bars.
      double wide = this->width() / 3.0;

      QLabel* hLbl = new QLabel("Hated", this);
      hLbl->setStyleSheet("color: orange;");
      hLbl->setGeometry(0, 0, wide, this->height());
      hLbl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
      hLbl->show();

      QLabel* nLbl = new QLabel("Neutral", this);
      nLbl->setStyleSheet("color: orange;");
      nLbl->setGeometry(wide, 0, wide, this->height());
      nLbl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
      nLbl->show();

      QLabel* fLbl = new QLabel("Friendly", this);
      fLbl->setStyleSheet("color: orange;");
      fLbl->setGeometry(wide * 2, 0, wide, this->height());
      fLbl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
      fLbl->show();

      firstTime = false;
   }
}
