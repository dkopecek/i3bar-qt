#ifndef SCRATCHPADWIDGET_HPP
#define SCRATCHPADWIDGET_HPP

#include "BarWidget.hpp"

namespace Ui {
class ScratchpadWidget;
}

class ScratchpadWidget : public BarWidget
{
    Q_OBJECT

public:
    explicit ScratchpadWidget(const QString &pad_name = QString(), QWidget *parent = 0);
    ~ScratchpadWidget();

private:
    Ui::ScratchpadWidget *ui;
};

#endif // SCRATCHPADWIDGET_HPP
