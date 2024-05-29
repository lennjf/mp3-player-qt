#include "mainwindow.h"
#include "ui_mainwindow.h"

bool isDir(const QString &path) {
    QDir dir(path);
    return dir.exists();
}

bool isAudioFile(const QString &path) {
    QString str(path);
    QFileInfo fileInfo(str);
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(fileInfo.filePath());
    if(mime.name().toLower().startsWith("audio")){
        return true;
    }else{
        return false;
    }
}

void MainWindow::listFile(QString mp3dir){
    QDir dir(mp3dir);
    QStringList list = dir.entryList();
    foreach (QString str, list.toList()) {
        if(str != "." && str != ".."){
            str = mp3dir + QDir::separator() + str;
            if(isDir(str)){
                if (str != "store"){
                    listFile(str);
                }
            }else {
                if(isAudioFile(str)){
                    musicList->append(str);
                }
            }
        }
    }
}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->playbtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pausebtn->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    ui->prebtn->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->postbtn->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    ui->homebtn->setIcon(style()->standardIcon(QStyle::SP_DirHomeIcon));

    mp = new QMediaPlayer(this);
    ao = new QAudioOutput(this);
    ao->setVolume(0.3);
    mp->setAudioOutput(ao);
    //connect(mp, &QMediaPlayer::positionChanged, this, &MediaExample::positionChanged);
    ui->volume->setMinimum(0);
    ui->volume->setMaximum(100);
    ui->volume->setValue(30);

    musicList = new QStringList();
    mp3index = 0;


    settings = new QSettings("niko-robin-player");
    //settings->setValue("cnm","cnnn");
    mp3dir = new QString(settings->value("mp3dir").toString());
    if(mp3dir->isEmpty()){
        ui->ed->setText("pls choose a mp3 resource folder to kick off.");
    }else{
        //qInfo()<<mp3dir << "---------dir";
        listFile(*mp3dir);
        ui->ed->setText(QString::number(musicList->size()) + " audio files loaded");
    }
    if(settings->value("mp3index").toString().isEmpty()){
        mp3index = 0;
    }else {
        mp3index = settings->value("mp3index").toString().toInt();
    }
    if(settings->value("volumenum").toString().isEmpty()){
        volumenum = 30;
    }else {
        volumenum = settings->value("volumenum").toString().toInt();
    }


    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::grogress_changed);
    timer->start(2000);

    connect(mp, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::onMediaStatusChanged);




}

MainWindow::~MainWindow()
{
    settings->setValue("mp3index", mp3index);
    settings->setValue("volumenum", volumenum);
    delete ui;
    delete musicList;
    delete mp3dir;
    delete settings;
}

void MainWindow::playtheaudio(){
    QString filename =  musicList->at(mp3index);
    mp->setSource(QUrl::fromLocalFile(filename));
    ao->setVolume((float)volumenum/100);
    ui->volume->setValue(volumenum);
    mp->play();
    QFileInfo fileinfo(filename);
    ui->ed->setText(fileinfo.fileName() + "  ...  " + QString::number(mp3index) + " of " + QString::number(musicList->size()) + "  ...");
}


void MainWindow::on_homebtn_clicked()
{
    QString dir = QFileDialog::getExistingDirectoryUrl(this, "select a dir").toString().replace("file://", "");
    settings->setValue("mp3dir", dir);
    settings->setValue("mp3index", mp3index);
    settings->setValue("volumenum", volumenum);
    musicList->clear();
    listFile(dir);
    mp3dir = &dir;
    // foreach (QString ml, musicList->toList()) {
    //     qInfo() << "musiclist =++++++++ " << ml ;
    // }
    playtheaudio();
}


void MainWindow::on_volume_valueChanged(int value)
{
    //ui->ed->setText( QString::number(value));
    ao->setVolume((float)value/100);
    //qInfo() << "volume changed: " << value;
    volumenum = value;
}



void MainWindow::grogress_changed()
{
    if(mp->hasAudio()){
        ui->progress->setValue(mp->position()*100/mp->duration());
    }
}





void MainWindow::on_progress_sliderMoved(int position)
{
    //ui->ed->setText( QString::number(position));
    mp->pause();
    mp->setPosition((long)mp->duration()*position/100);
    mp->play();
    //qInfo() << mp->duration() << position << mp->duration()*position/100 << mp->position();

}


void MainWindow::on_playbtn_clicked()
{
    if(mp->hasAudio()){
        mp->play();
    }else if(musicList->size()>0){
        playtheaudio();
    }

}


void MainWindow::on_pausebtn_clicked()
{
    if(mp->hasAudio()){
        mp->pause();
    }

}

void MainWindow::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) {

        //qDebug() << "MP3 has finished playing " << musicList->size();
        if(mp3index < (musicList->size() - 1)){
            mp3index += 1;
        }else {
            mp3index = 1;
        }
        playtheaudio();
    }
}






void MainWindow::on_prebtn_clicked()
{
    if(mp3index > 0 ){
        mp3index -= 1;
    }else {
        mp3index = musicList->size() - 1;
    }
    playtheaudio();
}


void MainWindow::on_postbtn_clicked()
{
    if(mp3index < (musicList->size() - 1)){
        mp3index += 1;
    }else {
        mp3index = 1;
    }
    playtheaudio();
}

