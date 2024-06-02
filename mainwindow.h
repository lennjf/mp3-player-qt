#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMultimedia>
#include <QtCore>
#include <QtWidgets>
#include <QDebug>



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void listFile(QString mp3dir);
    void playtheaudio();
private slots:
    void on_homebtn_clicked();

    void on_volume_valueChanged(int value);


    void grogress_changed();

    void on_progress_sliderMoved(int position);

    void on_playbtn_clicked();

    void on_pausebtn_clicked();

    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);

    void on_prebtn_clicked();

    void on_postbtn_clicked();

    void onAudioDevicesChanged();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *mp;
    QAudioOutput *ao;
    QTimer *timer;
    QSettings *settings;
    QStringList *musicList;
    int mp3index;
    QString *mp3dir;
    int volumenum;

    QMediaDevices *device;

};
#endif // MAINWINDOW_H
