#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

#include <QCloseEvent>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

#include <QFile>
#include <QStandardPaths>

#include <QDebug>

#include "top_window_main.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


extern top_window_main *top_win;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QSystemTrayIcon *tray_icon;

    QAction *hide_topwindow;
    QAction *show_topwindow;
    QAction *quit;
    QAction *change_arrangements;
    QAction *set_topwindow_top;

    QMenu *action_menu;

protected:
    void closeEvent(QCloseEvent * event);

private:
    Ui::MainWindow *mainwindow_ui;
};
#endif // MAINWINDOW_H
