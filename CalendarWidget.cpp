#include "CalendarWidget.hpp"
#include "ui_CalendarWidget.h"

CalendarWidget::CalendarWidget(QWidget *parent) :
    BarWidget("calendar", QString(), parent),
    ui(new Ui::CalendarWidget)
{
    ui->setupUi(this);
}

CalendarWidget::~CalendarWidget()
{
    delete ui;
}
