
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "CustomChart.h"
#include <QMainWindow>

#include <QtCharts/QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnAdd_clicked();

    void on_btnRemove_clicked();

    void on_btnRemoveAll_clicked();

    void on_btnChart_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    QListWidget *original;

    CustomChart *customChart;
};

#endif // MAINWINDOW_H
