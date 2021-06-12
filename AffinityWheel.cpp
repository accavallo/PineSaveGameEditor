#include "AffinityWheel.h"

#include <QPainter>
#include <QPaintEvent>

AffinityWheel::AffinityWheel(QWidget *parent)
    : QLabel(parent)
{
}

AffinityWheel::~AffinityWheel()
{
}

void AffinityWheel::SetCenter(double center)
{
   this->center = center * 90 + 90;
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
   //painter.drawLine(QPointF(center, 0), QPointF(center, this->height()));

   painter.drawText(QPoint(15, 20), "Hated");
   painter.drawText(QPoint(70, 20), "Neutral");
   painter.drawText(QPoint(127, 20), "Friendly");
}
