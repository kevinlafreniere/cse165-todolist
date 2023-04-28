
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QStandardPaths>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFile file(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "\\toDoFile.txt");

    if(!file.open(QIODevice::ReadWrite)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    // read the file for each list item, line by line
    while(!in.atEnd()){
        QListWidgetItem* item = new QListWidgetItem(in.readLine(), ui->listWidget);
        ui->listWidget->addItem(item);
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
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        out<<ui->listWidget->item(i)->text()<<"\n";
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

    ui->listWidget->addItem(item);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    ui->txtTask->clear();
    ui->txtTask->setFocus();
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

