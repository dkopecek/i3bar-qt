#ifndef BARWIDGET_HPP
#define BARWIDGET_HPP

#include <QWidget>

class BarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BarWidget(const QString& name, const QString& instance = QString(), QWidget *parent = nullptr);

    QString getName(bool fullname = false) const;

signals:

public slots:

private:
    QString _name;
    QString _instance;
};

#endif // BARWIDGET_HPP
