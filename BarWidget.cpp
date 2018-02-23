#include "BarWidget.hpp"

BarWidget::BarWidget(const QString &name, const QString &instance, QWidget *parent)
    : QWidget(parent), _name(name), _instance(instance)
{
}

QString BarWidget::getName(bool fullname) const
{
    if (fullname && !_instance.isEmpty()) {
        return QString("%1 (%2)").arg(_name).arg(_instance);
    }
    else {
        return _name;
    }
}
