#ifndef CALENDARWIDGET_HPP
#define CALENDARWIDGET_HPP

#include "BarWidget.hpp"

namespace Ui {
class CalendarWidget;
}

class CalendarWidget : public BarWidget
{
    Q_OBJECT

public:
    explicit CalendarWidget(QWidget *parent = 0);
    ~CalendarWidget();

private:
    Ui::CalendarWidget *ui;
};

#endif // CALENDARWIDGET_HPP
