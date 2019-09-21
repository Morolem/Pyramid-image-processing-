#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QFileDialog>
#include <QImage>
#include <QtDebug>
#include <QWidget>

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

void MainWindow::on_pushButton_clicked()
{
    QString fileName = nullptr;
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                      "/home",
                                                      tr("Images (*.png *.xpm *.jpg)"));    
    if(fileName != nullptr) {
        QImage image;
        image.load(fileName);

//         auto diag = sqrt(image.width()*image.width() + image.height() * image.height());
//         qDebug() << diag;
//        image = image.scaled(ui->label->width(),ui->label->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        auto image2 = image.scaled(image.width()/1,image.height()/1,Qt::KeepAspectRatio);
//         auto image2 = image;
//         image2 = image.scaled(image.width(),image.height(),Qt::KeepAspectRatio);
        //image2 = image2.scaled(ui->label->width(),ui->label->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        //image.save("./",nullptr,65);
        ui->label->setPixmap(QPixmap::fromImage(image2));
//        ui->scrollArea->setWidget());
        ui->doubleSpinBox->setValue(0.00);
    }
}
