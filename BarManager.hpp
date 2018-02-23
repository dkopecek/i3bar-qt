#ifndef BARMANAGER_HPP
#define BARMANAGER_HPP

#include "Bar.hpp"
#include "i3ipc-qt/i3ipc.hpp"

#include <QApplication>
#include <QMap>
#include <QString>
#include <QMutex>
#include <QRect>

#include <memory>

class BarManager : public QApplication
{
    Q_OBJECT
public:
    BarManager(int argc, char *argv[]);

private slots:
    void i3EventHook(quint32 type, QJsonDocument payload_json);

    void setupBars();
    void setupWorkspaces();

    void createBar(const QString& id, const QRect& output_rect);
    //void updateBar(const QString& id, const QRect& output_rect);
    //void deleteBar(const QString& id);

private:
    QMutex _mutex;
    i3ipc _i3_ipc;
    QMap<QString,std::shared_ptr<Bar>> _bar_map;
};

#endif // BARMANAGER_HPP
