#include "mainwindow.h"

#include <QApplication>
#include <QSettings>

#define RUN_PATH "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"

top_window_main *top_win;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    qDebug()<<qApp->applicationFilePath();
    //w.show();
//开机启动
    QSettings settings(RUN_PATH,QSettings::NativeFormat);

    QString app_name = QApplication::applicationName();
    QString app_path = QApplication::applicationFilePath();

    settings.setValue(app_name,app_path.replace("/","\\"));

    return a.exec();
}
