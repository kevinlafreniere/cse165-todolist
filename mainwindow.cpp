
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QStandardPaths>
#include <QMessageBox>
#include <QCheckBox>
#include <QAction>
#include <QListWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QBarSet *set0 = new QBarSet("Incomplete");
    QBarSet *set1 = new QBarSet("Complete");

    *set0 << 10;
    *set1 << 5;

    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->append(set1);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("BarChart test");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    categories << "First" << "second";
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();

    QChartView *chartView = new QChartView(chart);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(chartView);
    chartWidget->setLayout(layout);

    // Add the chart widget to the stacked widget
    ui->stackedWidget->addWidget(chartWidget);
    //ui->stackedWidget->setCurrentWidget(chartWidget);

    QFile file(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "\\toDoFile.txt");

    if(!file.open(QIODevice::ReadWrite)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    original = new QListWidget();
    // read the file for each list item, line by line
    while(!in.atEnd()){
        QListWidgetItem* item = new QListWidgetItem(in.readLine(), ui->listWidget);
        item->setCheckState(Qt::Unchecked);
        ui->listWidget->addItem(item);
        original->addItem(item->clone());
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    }

    file.close();
}

MainWindow::~MainWindow()
{
    delete ui;

    QFile file(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "\\toDoFile.txt");

    // open file for overwriting (Truncate 'clears' old data)
    if(!file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream out(&file);

    // save each list item in a new line
    for (int i = 0; i < original->count(); ++i) {
        out<<original->item(i)->text()<<"\n";
    }

    file.close();
}



void MainWindow::on_btnAdd_clicked()
{
    // exit func if text is empty
    if(ui->txtTask->text().isEmpty()){
        ui->txtTask->setFocus();
        return;
    }

    QListWidgetItem* item = new QListWidgetItem(ui->txtTask->text(), ui->listWidget);
    item->setCheckState(Qt::Unchecked);

    ui->listWidget->addItem(item);
    original->addItem(item->clone());
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    item->isSelected();
    ui->txtTask->clear();
    ui->txtTask->setFocus();

    MainWindow::on_comboBox_currentIndexChanged(ui->comboBox->currentIndex());
    ui->listWidget->setCurrentItem(item);
}


void MainWindow::on_btnRemove_clicked()
{
    QListWidgetItem* item = ui->listWidget->takeItem(ui->listWidget->currentRow());
    delete item;
}


void MainWindow::on_btnRemoveAll_clicked()
{
    ui->listWidget->clear();
}


void MainWindow::on_actionSort_toggled(bool arg1)
{
    if(arg1){
        ui->listWidget->sortItems(Qt::AscendingOrder);
    }
    else{
        ui->listWidget->clear();
        for (int var = 0; var < original->count(); ++var) {
            ui->listWidget->addItem(original->item(var)->clone());
        }
    }
}


void MainWindow::on_btnChart_clicked()
{
    if(ui->stackedWidget->currentIndex() == 0){
        ui->stackedWidget->setCurrentWidget(chartWidget);
        ui->btnChart->setText("Return");
    }
    else{
        ui->stackedWidget->setCurrentIndex(0);
        ui->btnChart->setText("View Chart");
    }
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if(index == 0){
        QString current = ui->listWidget->currentItem()->text();
        ui->listWidget->clear();
        for (int var = 0; var < original->count(); ++var) {
            ui->listWidget->addItem(original->item(var)->clone());
            if(ui->listWidget->item(var)->text().compare(current) == 0){
                ui->listWidget->setCurrentItem(ui->listWidget->item(var));
            }
        }
    }
    else if(index == 1){
        ui->listWidget->sortItems(Qt::AscendingOrder);
    }
    else{
        ui->listWidget->sortItems(Qt::DescendingOrder);
    }
}

