#include "Bar.hpp"
#include "BarManager.hpp"

#include <QDebug>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
    qDebug() << "Debugging is ON.";
    BarManager manager(argc, argv);
    return manager.exec();
}
