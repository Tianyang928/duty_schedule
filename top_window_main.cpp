#include "top_window_main.h"
#include "ui_top_window_main.h"

top_window_main::top_window_main(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::top_window_main)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->move(1920-this->width(),990-this->height());

    record_word_size = ui->lineEdit->font().pointSizeF();
    record_lineedit_height = ui->lineEdit->height();

    QTime current_time = QTime::currentTime();
    QTime trigger_time;

    trigger_time.setHMS(20,20,0);

    int seconds_wait = current_time.secsTo(trigger_time);

    clock = new QTimer();
    clock->setSingleShot(true);
    clock->setTimerType(Qt::VeryCoarseTimer);
    QObject::connect(clock,&QTimer::timeout,this,[=]()
    {
        this->resize(1280,720);
        this->move(1920/2-1280/2,1080/2-720/2);
        clock->start(24*60*60*1000);
    });

    clock->start(seconds_wait*1000);

}

top_window_main::~top_window_main()
{
    delete ui;
}

void top_window_main::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();
}

void top_window_main::update_widget_status()
{
    QDateTime current_time = QDateTime::currentDateTime();
    int week_index = current_time.date().weekNumber();
    qDebug()<<week_index;
    bool is_odd_week;
    if(week_index%2 == 0)
    {
        is_odd_week = false;
    }
    else
    {
        is_odd_week = true;
    }
    QString day_type = current_time.toString("ddd");
//test
    //day_type = "\345\221\250\344\270\200";

    qDebug()<<day_type;
    QStringList week_type_list = {"\345\221\250\344\270\200"
                                  ,"\345\221\250\344\272\214"
                                  ,"\345\221\250\344\270\211"
                                  ,"\345\221\250\345\233\233"
                                  ,"\345\221\250\344\272\224"};
    int day_index = week_type_list.indexOf(day_type);
    if(day_index == -1)
    {
        return;
    }

    ui->dateEdit->setDate(current_time.date());
    qDebug()<<"set_date"<<current_time.date().toString();
    QFile read_one_day(qApp->applicationDirPath()+"/cleaning_arrangement.json");
    if(read_one_day.open(QFile::ReadOnly))
    {
        QJsonParseError read_error;
        QJsonDocument doc_one_day = QJsonDocument::fromJson(read_one_day.readAll(),&read_error);
        if(!doc_one_day.isNull() && read_error.error == QJsonParseError::NoError)
        {
            QJsonObject root_object = doc_one_day.object();
            QJsonArray week_type_array;
            if(is_odd_week)
            {
                week_type_array = root_object.value("odd_week").toArray();
            }
            else
            {
                week_type_array = root_object.value("even_week").toArray();
            }
            QJsonObject one_day_object = week_type_array[day_index].toObject();

            ui->lineEdit_2->setText("\346\211\253\345\234\260\347\224\262:"+one_day_object.value("\346\211\253\345\234\260\347\224\262").toString());
            ui->lineEdit_3->setText("\346\211\253\345\234\260\344\271\231:"+one_day_object.value("\346\211\253\345\234\260\344\271\231").toString());
            ui->lineEdit_4->setText("\346\216\222\346\241\214\344\270\211\345\205\263:"+one_day_object.value("\346\216\222\346\241\214\344\270\211\345\205\263").toString());
            ui->lineEdit_5->setText("\351\273\221\346\235\277:"+one_day_object.value("\351\273\221\346\235\277").toString());
            ui->lineEdit_6->setText("\346\213\226\345\234\260:"+one_day_object.value("\346\213\226\345\234\260").toString());
            ui->lineEdit_7->setText("\351\223\201\347\224\267\347\224\262:"+one_day_object.value("\351\223\201\347\224\267\347\224\262").toString());
            ui->lineEdit_8->setText("\351\223\201\347\224\267\344\271\231:"+one_day_object.value("\351\223\201\347\224\267\344\271\231").toString());
        }
    }
    read_one_day.close();

    adjust_word_size();
}

void top_window_main::resizeEvent(QResizeEvent *event)
{
    adjust_word_size();
}

void top_window_main::adjust_word_size()
{
    qDebug()<<"record_word_size"<<record_word_size;
    qDebug()<<"record_lineedit_height"<<record_lineedit_height;
    int new_lineedit_height = ui->lineEdit->height();
    qreal new_word_size = new_lineedit_height/record_lineedit_height*record_word_size;

    qDebug()<<"new_word_size"<<new_word_size;

    QFont new_font = this->font();
    new_font.setPointSizeF(new_word_size);

    ui->lineEdit->setFont(new_font);
    ui->lineEdit_2->setFont(new_font);
    ui->lineEdit_3->setFont(new_font);
    ui->lineEdit_4->setFont(new_font);
    ui->lineEdit_5->setFont(new_font);
    ui->lineEdit_6->setFont(new_font);
    ui->lineEdit_7->setFont(new_font);
    ui->lineEdit_8->setFont(new_font);
}
