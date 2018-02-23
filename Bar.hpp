#ifndef BAR_HPP
#define BAR_HPP

#include "i3ipc-qt/i3ipc.hpp"
#include "BarWidget.hpp"

#include <QWidget>
#include <QString>
#include <QMutex>
#include <QMap>
#include <QPushButton>
#include <QTimer>
#include <QButtonGroup>

namespace Ui {
class Bar;
}

class Bar : public QWidget
{
    Q_OBJECT

public:
    explicit Bar(i3ipc& i3_ipc, QWidget *parent = 0);
    ~Bar();

    /* How often should the datetime value be updated */
    void setDatetimeInterval(unsigned int secs);

    void addBarWidget(BarWidget* widget);
    void deleteBarWidget(BarWidget* widget);

public slots:
    void minimizeBar();
    void activateBar(bool focus = false);

    bool hasWorkspaceButton(const QString& name);

    void addWorkspaceButton(const QString& name);
    void deleteWorkspaceButton(const QString& name);

    void setCurrentWorkspace(const QString& name);

private slots:
    void handleWorkspaceButtonPressed(QAbstractButton* button);
    void updateDatetime();

private:
    bool event(QEvent *event) override;

    Ui::Bar *ui{nullptr};
    QMutex _mutex;
    i3ipc& _i3_ipc;
    QTimer _minimize_timer;
    QTimer _datetime_timer;

    QMap<QString,QPushButton*> _workspace_map;
    QButtonGroup _workspace_button_group;
    QPushButton* _workspace_button_pressed{nullptr};
};

#endif // BAR_HPP
