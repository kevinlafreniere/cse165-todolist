
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

    QBarSet *set0 = new QBarSet("User");

    *set0 << 10 << 15;

    series = new QBarSeries();
    series->append(set0);

    chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Your Todo List Progress");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    categories << "Incomplete" << "Complete";
    axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);

    axisY = new QValueAxis();
    axisY->setRange(0,15);
    axisY->setTickCount(4);
    axisY->setLabelFormat("%d");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chartView = new QChartView(chart);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartWidget = new QWidget();
    layout = new QVBoxLayout();
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

        if(in.readLine().contains("0")){
            item->setCheckState(Qt::Unchecked);
        }
        else{
            item->setCheckState(Qt::Checked);
        }

        item->setFlags(item->flags() | Qt::ItemIsEditable);

        ui->listWidget->addItem(item);
        original->addItem(item->clone());
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
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        out<<ui->listWidget->item(i)->text()<<"\n";
        if(ui->listWidget->item(i)->checkState() == Qt::Unchecked){
            out<<"0\n";
        }
        else{
            out<<"1\n";
        }
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


    item->setFlags(item->flags() | Qt::ItemIsEditable);
    item->isSelected();
    ui->txtTask->clear();
    ui->txtTask->setFocus();

    ui->listWidget->addItem(item);
    original->addItem(item->clone());

    if(ui->comboBox->currentIndex() != 0)
        MainWindow::on_comboBox_currentIndexChanged(ui->comboBox->currentIndex());
    ui->listWidget->setCurrentItem(item);
}


void MainWindow::on_btnRemove_clicked()
{
    QListWidgetItem* item = ui->listWidget->takeItem(ui->listWidget->currentRow());

    //TODO: delete item from original list
    //original->removeItemWidget(item);
    delete original->itemWidget(item);
    //if(item->checkState() == Qt::Checked)
        delete item;
}


void MainWindow::on_btnRemoveAll_clicked()
{
    ui->listWidget->clear();
    original->clear();
}

//UNUSED NOW
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
//        QBarSet *set0 = new QBarSet("Incomplete");
//        QBarSet *set1 = new QBarSet("Complete");

//        *set0 << 100;
//        *set1 << 30;

//        set0->

//        series->clear();

//        series->append(set0);
//        series->append(set1);

//        chart->removeAllSeries();
//        chart->addSeries(series);
//        chart->createDefaultAxes();
        int checkedCount = 0;
        int uncheckedCount = 0;
        for (int var = 0; var < ui->listWidget->count(); ++var) {
            if(ui->listWidget->item(var)->checkState() == Qt::Checked){
                checkedCount++;
            }
            else{
                uncheckedCount++;
            }
        }


        if(checkedCount > uncheckedCount){
            axisY->setRange(0,checkedCount);
        }
        else{
            axisY->setRange(0,uncheckedCount);
        }

        QBarSet *set0 = new QBarSet("User");
        *set0 << uncheckedCount << checkedCount;
        series->clear();
        series->append(set0);

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
        QString current;
        if(ui->listWidget->currentItem() != nullptr)
            current = ui->listWidget->currentItem()->text();


        for (int var = 0; var < original->count(); ++var) {
            // Find the item by its text
            QString targetText = original->item(var)->text();
            QList<QListWidgetItem *> foundItems = ui->listWidget->findItems(targetText, Qt::MatchExactly);

            // Change the check state of the first found item
            if (!foundItems.isEmpty()) {
                QListWidgetItem *item = foundItems.first();
                if(item->checkState() == Qt::Unchecked){
                    original->item(var)->setCheckState(Qt::Unchecked);
                }
                else{
                    original->item(var)->setCheckState(Qt::Checked);
                }
            }
        }
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
