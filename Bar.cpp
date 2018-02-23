#include "Bar.hpp"
#include "ui_Bar.h"

#include "CalendarWidget.hpp"

#include <QDateTime>
#include <QCursor>
#include <QDebug>

Bar::Bar(i3ipc& i3_ipc, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Bar),
    _i3_ipc(i3_ipc),
    _workspace_button_group(this)
{
    setAttribute(Qt::WA_X11NetWmWindowTypeDock);
    ui->setupUi(this);

    minimizeBar();

    _minimize_timer.setSingleShot(true);
    _minimize_timer.setInterval(2 * 1000);

    _datetime_timer.setInterval(5 * 1000);

    QObject::connect(&_minimize_timer, SIGNAL(timeout()), this, SLOT(minimizeBar()));
    QObject::connect(&_datetime_timer, SIGNAL(timeout()), this, SLOT(updateDatetime()));
    QObject::connect(&_workspace_button_group, SIGNAL(buttonPressed(QAbstractButton*)), this, SLOT(handleWorkspaceButtonPressed(QAbstractButton*)));

    _datetime_timer.start();

    addBarWidget(new CalendarWidget());
}

Bar::~Bar()
{
    delete ui;
}

void Bar::setDatetimeInterval(unsigned int secs)
{
    _datetime_timer.setInterval(secs * 1000);
}

void Bar::addBarWidget(BarWidget *widget)
{
    if (widget) {
        ui->app_widget->addTab(widget, widget->getName(/*full=*/true));
    }
}

void Bar::deleteBarWidget(BarWidget* widget)
{
    if (widget) {
        int delete_index = -1;
        for (int i = 0; i < ui->app_widget->count(); ++i) {
            if (ui->app_widget->widget(i) == widget) {
                delete_index = i;
                break;
            }
        }
        if (delete_index >= 0) {
            ui->app_widget->removeTab(delete_index);
        }
    }
}

bool Bar::hasWorkspaceButton(const QString &name)
{
    return _workspace_map.contains(name);
}

bool Bar::event(QEvent *event)
{
    qDebug() << "event=" << event->type();

    if (event->type() == QEvent::Enter) {
            _minimize_timer.stop();
            activateBar(/*focus=*/true);
    }

    if (event->type() == QEvent::Leave) {
        if (!rect().contains(mapFromGlobal(QCursor::pos()))) {
            _minimize_timer.start();
        }
    }

    if (event->type() == QEvent::KeyPress) {
        if (ui->top_lineedit->event(event)) {
            return true;
        }
    }

    return QWidget::event(event);
}

void Bar::minimizeBar()
{
    if (!ui->app_widget->isHidden()) {
        ui->app_widget->hide();
        adjustSize();
        ui->top_lineedit->setReadOnly(true);
        ui->top_lineedit->clear();
    }
    releaseKeyboard();
}

void Bar::activateBar(bool focus)
{
    if (ui->app_widget->isHidden()) {
        ui->app_widget->show();
        adjustSize();
    }

    if (focus) {
        activateWindow();
        grabKeyboard();
        ui->top_lineedit->setReadOnly(false);
        ui->top_lineedit->setFocus();
    }
}

void Bar::addWorkspaceButton(const QString &name)
{
    QPushButton* workspace_button = new QPushButton(name);

    workspace_button->setProperty("i3.workspace", name);
    workspace_button->setCheckable(true);

    _workspace_button_group.addButton(workspace_button);
    _workspace_map[name] = workspace_button;

    ui->workspace_widget->layout()->addWidget(workspace_button);
    //ui->workspace_widget->layout()->update();
}

void Bar::setCurrentWorkspace(const QString &name)
{
    qDebug() << "setCurrentWorkspace=" << name;

    if (_workspace_map.contains(name)) {

        if (_workspace_button_pressed) {
            _workspace_button_pressed->setChecked(false);
            _workspace_button_pressed->setDown(false);
        }

        _workspace_button_pressed = _workspace_map.value(name);

        _workspace_button_pressed->setChecked(true);
        _workspace_button_pressed->setDown(true);
    }
}

void Bar::deleteWorkspaceButton(const QString &name)
{
    if (_workspace_map.contains(name)) {
        QPushButton* button = _workspace_map.value(name);
        //ui->workspace_widget->layout()->removeWidget(button);
        _workspace_map.remove(name);
        _workspace_button_group.removeButton(button);
    }
}

void Bar::handleWorkspaceButtonPressed(QAbstractButton *button)
{
    qDebug() << "object=" << button;

    if (button == 0) {
        return;
    }

    const QString& workspace_name = button->property("i3.workspace").toString();

    qDebug() << "name=" << workspace_name;

    _i3_ipc.runCommand(QString("workspace %1").arg(workspace_name));

    setCurrentWorkspace(workspace_name);
}

void Bar::updateDatetime()
{
    const auto datetime_local = QDateTime::currentDateTime().toLocalTime();
    ui->datetime_label->setText(datetime_local.toString("yyyy-MM-dd HH:mm:ss"));
}
