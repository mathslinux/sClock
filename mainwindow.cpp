#include <QDebug>
#include <QUrl>
#include <QTimer>
#include "mainwindow.h"

/**
- 点击开始
  - 查询音乐状态
    - 如果已经开始，不让点击。
    - 否则开始音乐，将图标换位停止，
  - 如果音乐放完了，将图标换位开始。
- 点击停止
  - 查询音乐状态
    - 如果已经停止不让点击，否则停止音乐。
    - 否则将图标换位开始音乐，停止音乐。 
 */

void MainWindow::debug(QMediaPlayer::MediaStatus status)
{
    qDebug() << status;
}

void MainWindow::play_music()
{
    myPlayer->play();
}

int MainWindow::get_hour()
{
    return ui->hour_btn->currentText().toInt();
}

int MainWindow::get_minute()
{
    return ui->minute_btn->currentText().toInt();
}

int MainWindow::get_second()
{
    return ui->second_btn->currentText().toInt();
}

void MainWindow::on_start_btn_clicked()
{
    if (get_status() == PLAYING) {
        // stop music
        myPlayer->stop();
        set_stoped();
    } else {
        this->wait_time = (3600 * get_hour() + 60 * get_minute() + get_second()) * 1000;
        QTimer::singleShot(this->wait_time, this, &MainWindow::play_music);
        set_playing();
    }
}

void MainWindow::tray_activated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        if (isHidden()) {
            show();
        }
        break;
    case QSystemTrayIcon::Context:
        // TODO:
        printf("unimplement\n");
        break;
    default:
        break;
    }
}

void MainWindow::init_time_alter()
{
    ui->timealter->setText("00:00:00");
    this->alter_timer = new QTimer(this);
    QObject::connect(this->alter_timer, &QTimer::timeout, this, &MainWindow::flush_alter);
}

void MainWindow::init_time_btn()
{
    _init_time_btn(ui->hour_btn, 24);
    _init_time_btn(ui->minute_btn, 60);
    _init_time_btn(ui->second_btn, 60);
}

void MainWindow::_init_time_btn(QComboBox *box, int unit)
{
    for (int i = 0; i < unit; ++i) {
        char buf[4];
        snprintf(buf, sizeof(buf), "%02d", i);
        box->addItem(buf);
    }
}

int MainWindow::get_status()
{
    return status;
}

void MainWindow::flush_alter()
{
    char text[32];
    int hour, minute, second;
    hour = remain_time / 3600;
    minute = (remain_time % 3600) / 60;
    second = ((remain_time % 3600) % 60);
    // this->wait_time = (3600 * get_hour() + 60 * get_minute() + get_second()) * 1000;
    snprintf(text, sizeof(text), "%02d:%02d:%02d", hour, minute, second);
    this->ui->timealter->setText(text);
    if (remain_time > 0 ) {
        remain_time--;
    }
}

void MainWindow::set_playing()
{
    status = PLAYING;
    ui->start_btn->setIcon(QIcon(":/res/res/stop.svg"));
    remain_time = wait_time / 1000; // transter to second
    this->alter_timer->start(1000);
}

void MainWindow::set_stoped()
{
    status = STOPED;
    ui->timealter->setText("00:00:00");
    this->alter_timer->stop();
    ui->start_btn->setIcon(QIcon(":/res/res/start.svg"));
}

void MainWindow::init_func_btn()
{
    ui->scene_btn->setIcon(QIcon(":/res/res/scene.svg"));
    QObject::connect(ui->start_btn, &QToolButton::clicked, this, &MainWindow::on_start_btn_clicked);
    set_stoped();
    ui->interval_btn->setIcon(QIcon(":/res/res/interval.svg"));
}

void MainWindow::init_player()
{
    myPlayer = new QMediaPlayer;
    myPlayer->setMedia(QUrl("qrc:/res/res/clock.wav"));
    myPlayer->setVolume(100);
}

MainWindow::MainWindow() :
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tray = new QSystemTrayIcon(this);

    tray->setIcon(QIcon(":/res/res/clock.svg"));
    tray->setToolTip("Simple Clock By dunrong");
    QObject::connect(tray, &QSystemTrayIcon::activated, this, &MainWindow::tray_activated);
    tray->show();

    init_time_alter();
    init_time_btn();
    init_func_btn();
    init_player();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}
