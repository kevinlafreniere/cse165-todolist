
#ifndef CUSTOMCHART_H
#define CUSTOMCHART_H

#include <QWidget>
#include <QtCharts/QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>

class CustomChart
{

public:
    CustomChart(QWidget *parent = nullptr){
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
    };
    ~CustomChart(){
        delete series;
        delete chart;
        delete axisX;
        delete axisY;
        delete chartView;
        delete layout;
    };

    // Update Chart
    void updateChart(int unchecked, int checked){
        QBarSet *set = new QBarSet("User");
        *set << unchecked << checked;
        getSeries()->clear();
        getSeries()->append(set);
    }

    // Getters
    QWidget *getChartWidget(){
        return chartWidget;
    };

    QBarSeries *getSeries(){
        return series;
    };

    QChart *getChart(){
        return chart;
    };

    QBarCategoryAxis *getAxisX(){
        return axisX;
    };

    QValueAxis *getAxisY(){
        return axisY;
    };

    QChartView *getChartView(){
        return chartView;
    };

    QVBoxLayout *getLayout(){
        return layout;
    };

private:
         // add any private members or functions here
    QWidget *chartWidget;
    QBarSeries *series;
    QChart *chart;
    QBarCategoryAxis *axisX;
    QValueAxis *axisY;
    QChartView *chartView;
    QVBoxLayout *layout;

    QStringList categories;
};

#endif // CUSTOMCHART_H
