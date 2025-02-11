#ifndef TOP_WINDOW_MAIN_H
#define TOP_WINDOW_MAIN_H

#include <QWidget>
#include <QCloseEvent>
#include <QResizeEvent>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

#include <QFile>
#include <QStandardPaths>

#include <QDateTime>
#include <QTimer>

#include <QDebug>

namespace Ui {
class top_window_main;
}

class top_window_main : public QWidget
{
    Q_OBJECT

public:
    explicit top_window_main(QWidget *parent = nullptr);
    ~top_window_main();

    qreal record_word_size;
    int record_lineedit_height;

    QTimer *clock;

    void update_widget_status();
    void adjust_word_size();

protected:
    void closeEvent(QCloseEvent *event) ;
    void resizeEvent(QResizeEvent *event) ;

private:
    Ui::top_window_main *ui;
};

#endif // TOP_WINDOW_MAIN_H
