#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private :
    struct Images
    {
        QString fileName;
        QImage imageData;
    };
    std::vector<Images> images;
    QString path = "C:\\";
    bool image_to_fit_window = false;


    /// возвращает индекс вектора images, для найденного имени
    int find_by_name(const QString &);

    /// функция сортировки изображений по размеру диагонали
    void combobox_quick_sort(const int &left,const int &right);

    /// размер диагонали изображения (т. Пифагора)
    double diag_images(const QImage &image);

    /// поиск дубликатов в массиве изображений images
    bool duplicate_search(const std::vector<Images> &v,const QImage &value);

    /// заполнение combobox отсортированным массивом изображений
    void insert_images_into_combobox();

private slots:
    void on_pushButton_clicked();
    void on_comboBox_activated(int index);

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
