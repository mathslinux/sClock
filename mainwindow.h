#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLayout>
#include <QToolButton>
#include <QComboBox>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QMediaPlayer>
#include "ui_mainwindow.h"

namespace Ui {
    class MainWindow;
}

#define PLAYING  0
#define STOPED  1

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
    void on_start_btn_clicked();

protected:
    void closeEvent(QCloseEvent *event);
private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *tray;
    int status;
    int wait_time;
    int remain_time;
    QMediaPlayer *myPlayer;
    QTimer *alter_timer;

    int get_status();
    void set_playing();
    void set_stoped();
    void debug(QMediaPlayer::MediaStatus status);
    void play_music();
    void tray_activated(QSystemTrayIcon::ActivationReason reason);
    void _init_time_btn(QComboBox *box, int unit);
    void init_time_alter();
    void init_time_btn();
    void init_func_btn();
    void init_player();
    int get_hour();
    int get_minute();
    int get_second();
    void music_status_changed(QMediaPlayer::MediaStatus status);
    void flush_alter();
};
#endif // MAINWINDOW_H
