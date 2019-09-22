#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QImage>
#include <QDir>
#include <QFileDialog>
#include <vector>
#include <QDebug>
#include <algorithm>

int MainWindow::find_by_name(const QString &name)
{
    for(uint i=0; i<images.size(); ++i)
        if(images[i].fileName == name) return i;
    return -1;
}

bool MainWindow::duplicate_search(const std::vector<Images> &v,const QImage &value)
{    
    for(uint i = 0; i<v.size(); ++i)
        if(value == v[i].imageData) return false;
    return true;
}

double MainWindow::diag_images(const QImage &image)
{
    return sqrt(image.width()*image.width() + image.height()*image.height());
}


void MainWindow::combobox_quick_sort(const int &left,const int &right)
{
    int l = left, r = right;
    int index_piv = (l+r) / 2;
    while(l <= r)
    {
        while(diag_images(images[l].imageData) < diag_images(images[index_piv].imageData)) ++l;
        while(diag_images(images[r].imageData) > diag_images(images[index_piv].imageData)) --r;
        if(l <= r){
            std::swap(images[l++],images[r--]);
        }
    }
    if(l < right) combobox_quick_sort(l,right);
    if(r > left) combobox_quick_sort(left,r);
}

void MainWindow::insert_images_into_combobox()
{
    ui->comboBox->clear();
    combobox_quick_sort(0,images.size()-1);
    for(int i=0; i<images.size(); i++){
        ui->comboBox->addItem(images[i].fileName);
    }
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

void MainWindow::on_pushButton_clicked()
{    

    QString fileName = nullptr; // имя файла
    // возвращается полный путь и имя файла
    fileName = QFileDialog::getOpenFileName(this, tr("Выбор файла"),
                                                      path,
                                                      tr("Images (*.png *.jpg)"));
    if(fileName != nullptr) { // если файл был выбран
        QImage image;
        image.load(fileName); // загружаем

        if(duplicate_search(images,image)) // если файл не дубликат
        {
            Images tmp;
            tmp.fileName = fileName;
            tmp.imageData = image;
            images.push_back(tmp); // пушим его в массив файлов

            insert_images_into_combobox(); // сортируем файлы по размеру диагонали и заносим в комбобокс
            ui->comboBox->setCurrentIndex(find_by_name(fileName)); // combobox устанавливаем на выбраный файл
            image_to_fit_window = false; // оригинальное разрешение изображения
            on_doubleSpinBox_valueChanged(0); // выводим файл (по умолчанию)
            ui->doubleSpinBox->setValue(0.00); // 0 слой изображения

        }

    }
}


void MainWindow::on_comboBox_activated(int index)
{
    if(index < images.size() && !images.empty())
            ui->label->setPixmap(QPixmap::fromImage(images[index].imageData));
}


void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    auto index = ui->comboBox->currentIndex();
    auto width = images[index].imageData.width() / pow(2,arg1); // уменьшаем ширину в 2^(уровень пирамиды)
    auto height = images[index].imageData.height() / pow(2,arg1); // также уменьшаем высоту
    QImage img = images[index].imageData.scaled(width,height,Qt::KeepAspectRatio); // скалируем по новой ширине и высоте


    if(image_to_fit_window == true) { // если нужна пирамида изображений по размеру окна
        // сжимаем до размера окна
        width = ui->scrollArea->width(); // ширина окна
        height = ui->scrollArea->height(); // высота окна
        img = img.scaled(width,height,Qt::KeepAspectRatio); // растягиваем изображение до размера оригинала
        // уменьшаем в 2^(уровень пирамиды)
        width = img.width() / pow(2,arg1); // уменьшаем ширину в 2^(уровень пирамиды)
        height = img.height() / pow(2,arg1); // также уменьшаем высоту
        ui->label_2->setText(QString::number(int(width)) + 'x' + QString::number(int(height))); // вывод разрешения

        img = img.scaled(width,height,Qt::KeepAspectRatio); // растягиваем изображение до размера оригинала
        // растягиваем до размера окна
        width = ui->scrollArea->width(); // ширина окна
        height = ui->scrollArea->height(); // высота окна
        img = img.scaled(width,height,Qt::KeepAspectRatio); // растягиваем изображение до размера оригинала
        ui->label->setPixmap(QPixmap::fromImage(img)); // выводим
    }
    else {
        ui->label_2->setText(QString::number(int(width)) + 'x' + QString::number(int(height))); // вывод разрешения
        width = images[index].imageData.width(); // изначальная ширина
        height = images[index].imageData.height(); // высота
        img = img.scaled(width,height,Qt::KeepAspectRatio); // растягиваем изображение до размера оригинала
        ui->label->setPixmap(QPixmap::fromImage(img)); // выводим

    }
}

void MainWindow::on_pushButton_3_clicked()
{
    image_to_fit_window = true;
    auto index = ui->comboBox->currentIndex(); // получаем индекс выбраного изображения
    auto width = ui->scrollArea->width(); // ширина окна
    auto height = ui->scrollArea->height(); // высота окна
    QImage img = images[index].imageData.scaled(width,height,Qt::KeepAspectRatio); // скалируем изображение по размерам окна
    ui->label->setPixmap(QPixmap::fromImage(img)); // выводим
    ui->doubleSpinBox->setValue(0.00);
    ui->label_2->setText(QString::number(int(width)) + 'x' + QString::number(int(height))); // вывод разрешения
}

void MainWindow::on_pushButton_2_clicked()
{
    image_to_fit_window = false;
    auto width = images[ui->comboBox->currentIndex()].imageData.width();
    auto height = images[ui->comboBox->currentIndex()].imageData.height();
    ui->label_2->setText(QString::number(int(width)) + 'x' + QString::number(int(height))); // вывод разрешения
    ui->label->setPixmap(QPixmap::fromImage(images[ui->comboBox->currentIndex()].imageData));
    ui->doubleSpinBox->setValue(0.00);
}
