#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCharts>
#include <QVector>
#include <QMainWindow>
#include <QDebug>
QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void simulate_pid();

public slots:
    void update_Chart();

private:
    double current_temp = 0;
    double goal_temp = 100;
    double Kp = 1;
    double Ki = 1;
    double Kd = 1;
    double pre_error = 0;
    double integral = 0;

    QChart *chart = new QChart();
    QLineSeries *series = new QLineSeries();
    QLineSeries *end_series = new QLineSeries();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
