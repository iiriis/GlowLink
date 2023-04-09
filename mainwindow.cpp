
#include "ui_mainwindow.h"
#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
        , ui(new Ui::MainWindow)
        , socket(nullptr)
    {
        ui->setupUi(this);

        QString temp;
        socket = new QTcpSocket(this);
        connect(socket, &QTcpSocket::readyRead, this, &MainWindow::on_readyRead);
        socket->connectToHost(IP_addr, 80);
        if (socket->waitForConnected()) {
            qDebug ("Connected to server");

        } else {
            qDebug  ("Error");
        }

        connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_connectButton_clicked);
    }

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectButton_clicked()
{
    qDebug ("dat");

    QColorDialog colorDialog(this);
    QColor color = colorDialog.getColor();

    int redValue = color.red();
    int greenValue = color.green();
    int blueValue = color.blue();

    QString colorString = QString::number(redValue) + " " + QString::number(greenValue) + " " + QString::number(blueValue);

    QByteArray data = colorString.toUtf8();

    socket->write(data);
    socket->flush();
}

void MainWindow::on_readyRead()
{
    QByteArray data = socket->readAll();
    qDebug (data);
}
