#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>

#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <QJsonObject>
#include <QJsonDocument>

void MainWindow::refreshIpDetails()
{
    ui->lblMessage->setPlainText("Loading...");

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    QUrl url("http://wtfismyip.com/json");
    QNetworkRequest request(url);

    // etc

    QObject::connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));

    manager->get(request);
}

void MainWindow::replyFinished(QNetworkReply * networkReply)
{
    QJsonDocument doc(QJsonDocument::fromJson(networkReply->readAll()));

    QJsonObject json = doc.object();

    QString message =
            "Public IP  : " + json["YourFuckingIPAddress"].toString() + "\n" +
            "Location   : " + json["YourFuckingLocation"].toString() + "\n" +
            "ISP        : " + json["YourFuckingISP"].toString();

    ui->lblMessage->setPlainText(message);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnStart_clicked()
{
    ui->lblMessage->setPlainText("Starting...");

    QProcess startKerioProcess;
    startKerioProcess.execute("gksudo /etc/init.d/kerio-kvc start");

    ui->lblMessage->setPlainText("Started");

    refreshIpDetails();
}

void MainWindow::on_btnStop_clicked()
{
    ui->lblMessage->setPlainText("Stopping...");

    QProcess stopKerioProcess;
    stopKerioProcess.execute("gksudo /etc/init.d/kerio-kvc stop");

    ui->lblMessage->setPlainText("Stopped");

    refreshIpDetails();
}

void MainWindow::on_pushButton_clicked()
{
    refreshIpDetails();
}
