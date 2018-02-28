#include "ScratchpadWidget.hpp"
#include "ui_ScratchpadWidget.h"

ScratchpadWidget::ScratchpadWidget(const QString& pad_name, QWidget *parent) :
    BarWidget("scratchpad", pad_name, parent),
    ui(new Ui::ScratchpadWidget)
{
    ui->setupUi(this);
}

ScratchpadWidget::~ScratchpadWidget()
{
    delete ui;
}
