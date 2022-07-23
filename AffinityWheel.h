#pragma once

#include <QLabel>

class AffinityWheel : public QLabel
{
   Q_OBJECT

public:
    AffinityWheel(QWidget *parent);
    ~AffinityWheel();

public slots:
   void SetCenter(double center);

protected:
   void paintEvent(QPaintEvent* event) override;

private:
   double center = 110.0;
};
