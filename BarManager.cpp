#include "BarManager.hpp"
#include "Bar.hpp"

#include <QDebug>
#include <QLoggingCategory>

BarManager::BarManager(int argc, char *argv[])
    : QApplication(argc, argv)
{
    QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);

    qDebug() << "ctor";
    QObject::connect(&_i3_ipc, SIGNAL(event(quint32,QJsonDocument)), this, SLOT(i3EventHook(quint32,QJsonDocument)));
    qDebug() << "before conn";
    _i3_ipc.connect();
    qDebug() << "connected";
    _i3_ipc.subscribe({"workspace"});
    qDebug() << "subscribed";
    setupWorkspaces();
}

void BarManager::i3EventHook(quint32 type, QJsonDocument payload_json)
{
    qDebug() << "event: t=" << type << " j=" << payload_json;
}

void BarManager::setupBars()
{
    qDebug() << "setup bars";
    const QMutexLocker locker(&_mutex);
    // getOutputs
    // create bar for each output
    const QJsonArray outputs_json = _i3_ipc.getOutputs();

    for (const QJsonValue& output_json : outputs_json) {
        qDebug() << "value: " << output_json;
        if (output_json.isObject()) {
            const QJsonObject output(output_json.toObject());

            if (!output["active"].toBool()) {
                qDebug() << "output inactive, skipping.";
                continue;
            }

            const QString id(output["name"].toString());
            const QRect rect(/*  left=*/output["rect"].toObject()["x"].toInt(),
                             /*   top=*/output["rect"].toObject()["y"].toInt(),
                             /* width=*/output["rect"].toObject()["width"].toInt(),
                             /*height=*/output["rect"].toObject()["height"].toInt());
            qDebug() << "object: " << output_json;
            createBar(id, rect);
        }
    }
    qDebug() << "setup bars exit";
}

void BarManager::setupWorkspaces()
{
    const QMutexLocker locker(&_mutex);
    const QJsonArray workspaces_json = _i3_ipc.getWorkspaces();
    
    for (const QJsonValue& workspace_json : workspaces_json) {
        qDebug() << "value: " << workspace_json;

        if (workspace_json.isObject()) {
            const QJsonObject workspace(workspace_json.toObject());
            const bool visible = workspace["visible"].toBool();
            const bool focused = workspace["focused"].toBool();
            const QString name = workspace["name"].toString();
            const QString output_id = workspace["output"].toString();
            const QRect output_rect(/*  left=*/workspace["rect"].toObject()["x"].toInt(),
                                    /*   top=*/workspace["rect"].toObject()["y"].toInt(),
                                    /* width=*/workspace["rect"].toObject()["width"].toInt(),
                                    /*height=*/workspace["rect"].toObject()["height"].toInt());

            qDebug() << "visible=" << visible;
            qDebug() << "focused=" << focused;
            qDebug() << "name=" << name;
            qDebug() << "output_id=" << output_id;

            if (!_bar_map.contains(output_id)) {
                createBar(output_id, output_rect);
            }

            const auto bar_widget = _bar_map[output_id];
            bar_widget->addWorkspaceButton(name);

            if (visible) {
                bar_widget->setCurrentWorkspace(name);
            }
        }
    }
}

void BarManager::createBar(const QString &id, const QRect &output_rect)
{
    //const QMutexLocker locker(&_mutex);
    qDebug() << "createBar: id=" << id << " rect=" << output_rect;
    auto& bar = _bar_map[id] = std::make_shared<Bar>(_i3_ipc);

    //bar->setGeometry(output_rect.x(), output_rect.y(), output_rect.width(
    bar->move(output_rect.x(), output_rect.y());
    bar->show();
}
