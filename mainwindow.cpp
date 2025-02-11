#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mainwindow_ui(new Ui::MainWindow)
{
    mainwindow_ui->setupUi(this);
    action_menu = new QMenu();
    hide_topwindow = new QAction("隐藏值日表^-^",this);
    show_topwindow = new QAction("显示值日表^-^",this);
    quit = new QAction("退出^-^",this);
    change_arrangements = new QAction("编辑值日表^-^",this);
    set_topwindow_top = new QAction("置顶值日表^-^",this);
    tray_icon = new QSystemTrayIcon();
//设置tablewidget部分可编辑
    for(int i = 0;i<mainwindow_ui->tableWidget->rowCount();i++)
    {
        mainwindow_ui->tableWidget->item(i,0)->setFlags(Qt::ItemIsEnabled);
    }
    for(int i = 0;i<mainwindow_ui->tableWidget->columnCount();i++)
    {
        mainwindow_ui->tableWidget->item(0,i)->setFlags(Qt::ItemIsEnabled);
    }
    for(int i = 0;i<mainwindow_ui->tableWidget_2->rowCount();i++)
    {
        mainwindow_ui->tableWidget_2->item(i,0)->setFlags(Qt::ItemIsEnabled);
    }
    for(int i = 0;i<mainwindow_ui->tableWidget_2->columnCount();i++)
    {
        mainwindow_ui->tableWidget_2->item(0,i)->setFlags(Qt::ItemIsEnabled);
    }
//top_win初始化
    top_win = new top_window_main();
    top_win->setWindowIcon(this->windowIcon());
    QObject::connect(top_win->clock,&QTimer::timeout,this,[=](){tray_icon->showMessage("值日!!!","快值日!!!");});
//所有action事件
    QObject::connect(hide_topwindow,&QAction::triggered,this,[=](){top_win->hide();});
    QObject::connect(show_topwindow,&QAction::triggered,this,[=](){top_win->show();});
    QObject::connect(change_arrangements,&QAction::triggered,this,[=]()
    {
        QFile read_file(qApp->applicationDirPath()+"/cleaning_arrangement.json");
        if(read_file.open(QFile::ReadOnly))
        {
            QJsonParseError read_error;
            QJsonDocument doc_read = QJsonDocument::fromJson(read_file.readAll(),&read_error);
            qDebug()<<read_error.error;
            qDebug()<<doc_read.toJson();
            if(!doc_read.isNull()&&read_error.error == QJsonParseError::NoError)
            {
                qDebug()<<"read_no_error";
                QJsonObject root_object = doc_read.object();
                QJsonArray odd_week_array = root_object.value("odd_week").toArray();
                QJsonArray even_week_array = root_object.value("even_week").toArray();

                for(int i = 1;i<mainwindow_ui->tableWidget->columnCount();i++)
                {
                    for(int j = 1;j<mainwindow_ui->tableWidget->rowCount();j++)
                    {
                        QJsonObject one_day_object = odd_week_array[i-1].toObject();
                        QTableWidgetItem *one_item = new QTableWidgetItem(one_day_object.value(mainwindow_ui->tableWidget->item(j,0)->text()).toString());
                        mainwindow_ui->tableWidget->setItem(j,i,one_item);
                    }
                }
                for(int i = 1;i<mainwindow_ui->tableWidget_2->columnCount();i++)
                {
                    for(int j = 1;j<mainwindow_ui->tableWidget_2->rowCount();j++)
                    {
                        QJsonObject one_day_object = even_week_array[i-1].toObject();
                        QTableWidgetItem *one_item = new QTableWidgetItem(one_day_object.value(mainwindow_ui->tableWidget_2->item(j,0)->text()).toString());
                        mainwindow_ui->tableWidget_2->setItem(j,i,one_item);
                    }
                }
            }
        }
        read_file.close();

        this->show();
    });

    set_topwindow_top->setCheckable(true);
    set_topwindow_top->setChecked(false);
    QObject::connect(set_topwindow_top,&QAction::toggled,top_win,[=]()
    {
        if(set_topwindow_top->isChecked())
        {
            top_win->setWindowFlag(Qt::WindowStaysOnTopHint,true);
        }
        else
        {
            top_win->setWindowFlag(Qt::WindowStaysOnTopHint,false);
        }
        top_win->show();
    });
    QObject::connect(quit,&QAction::triggered,qApp,&QApplication::quit);

    action_menu->addAction(hide_topwindow);
    action_menu->addAction(show_topwindow);
    action_menu->addAction(set_topwindow_top);
    action_menu->addSeparator();
    action_menu->addAction(change_arrangements);
    action_menu->addSeparator();
    action_menu->addAction(quit);

    tray_icon->setIcon(QIcon(":/new/logo/resources/logo.png"));
    tray_icon->setToolTip("值日表安排^-^");
    tray_icon->setContextMenu(action_menu);
    tray_icon->show();
    this->close();

    //mainwindow ui 事件
    QObject::connect(mainwindow_ui->pushButton,&QPushButton::clicked,this,[=]()
    {
        QJsonDocument doc;
        QJsonObject root_object;
        QJsonArray first_week_array;
        QJsonArray second_week_array;
        for(int i = 1;i<mainwindow_ui->tableWidget->columnCount();i++)
        {
            QJsonObject one_day;
            for(int j = 1;j<mainwindow_ui->tableWidget->rowCount();j++)
            {
                //qDebug()<<mainwindow_ui->tableWidget->item(j,0)->text()<<" "<<mainwindow_ui->tableWidget->item(j,i)->text();
                if(mainwindow_ui->tableWidget->item(j,i) == nullptr || mainwindow_ui->tableWidget->item(j,i)->text().isEmpty())
                {
                    qDebug()<<"length == 0";
                    one_day.insert(mainwindow_ui->tableWidget->item(j,0)->text()," ");
                    continue;
                }
                one_day.insert(mainwindow_ui->tableWidget->item(j,0)->text(),mainwindow_ui->tableWidget->item(j,i)->text());
            }
            first_week_array.append(one_day);
        }
        root_object.insert("odd_week",first_week_array);
        for(int i = 1;i<mainwindow_ui->tableWidget_2->columnCount();i++)
        {
            QJsonObject one_day;
            for(int j = 1;j<mainwindow_ui->tableWidget_2->rowCount();j++)
            {
                if(mainwindow_ui->tableWidget_2->item(j,i) == nullptr || mainwindow_ui->tableWidget_2->item(j,i)->text().isEmpty())
                {
                    one_day.insert(mainwindow_ui->tableWidget_2->item(j,0)->text()," ");
                    continue;
                }
                one_day.insert(mainwindow_ui->tableWidget_2->item(j,0)->text(),mainwindow_ui->tableWidget_2->item(j,i)->text());
            }
            second_week_array.append(one_day);
        }
        root_object.insert("even_week",second_week_array);
        doc.setObject(root_object);

        qDebug()<<doc.toJson();

        QFile file(qApp->applicationDirPath()+"/cleaning_arrangement.json");
        if(file.open(QFile::WriteOnly|QFile::Truncate))
        {
            file.write(doc.toJson());
        }
        file.close();

        this->hide();
        tray_icon->showMessage("Saved","保存好啦>-<\0");

        top_win->update_widget_status();
    });

    top_win->update_widget_status();
    top_win->show();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();
}

MainWindow::~MainWindow()
{
    delete mainwindow_ui;
}

