#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->start_horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(update_Chart()));
    connect(ui->end_horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(update_Chart()));
    connect(ui->P_horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(update_Chart()));
    connect(ui->I_horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(update_Chart()));
    connect(ui->D_horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(update_Chart()));

    ui->out_label->setText(" 初始温度:"+QString::number(current_temp) +
                           "\n目标温度:"+QString::number(goal_temp) +
                           "\nP:"+QString::number(Kp) +
                           "\nI:"+QString::number(Ki) +
                           "\nD:"+QString::number(Kd));

    chart->legend()->hide();
    chart->addSeries(series);
    chart->addSeries(end_series);
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setTheme(QChart::ChartThemeDark);

    // 创建默认的坐标轴并设置范围
    QValueAxis *xAxis = new QValueAxis;
    xAxis->setTitleText("时间");
    xAxis->setRange(0, 50);
    chart->addAxis(xAxis, Qt::AlignBottom);

    QValueAxis *yAxis = new QValueAxis;
    yAxis->setTitleText("温度");
    // 初始设置一个较大的Y轴范围，可以在update_Chart函数中动态调整
    yAxis->setRange(-100, 200); // 范围取决于你的实际需求
    chart->addAxis(yAxis, Qt::AlignLeft);

    series->attachAxis(xAxis);
    series->attachAxis(yAxis);
    end_series->attachAxis(xAxis);
    end_series->attachAxis(yAxis);

    ui->Chart_View->setChart(chart);
    // 开启抗锯齿，让显示效果更好
    ui->Chart_View->setRenderHint(QPainter::Antialiasing);
    // 显示图表
    ui->Chart_View->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::simulate_pid()
{
    //计算温度差
    double error = goal_temp - current_temp;
    //更新积分项
    integral += error;
    //获得微分项
    double deribative = error - pre_error;
    //计算输出值
    double output = Kp*error + Ki*integral + Kd*deribative;
    //记录偏差
    pre_error = error;
    //将当前温度加上输出值
    current_temp += output;
}

void MainWindow::update_Chart()
{
    current_temp = ui->start_horizontalSlider->value();
    goal_temp = ui->end_horizontalSlider->value();
    Kp = ui->P_horizontalSlider->value() / 100.0;
    Ki = ui->I_horizontalSlider->value() / 1000.0;
    Kd = ui->D_horizontalSlider->value() / 10000.0;

    ui->out_label->setText(" 初始温度:"+QString::number(current_temp) +
                           "\n目标温度:"+QString::number(goal_temp) +
                           "\nP:"+QString::number(Kp) +
                           "\nI:"+QString::number(Ki) +
                           "\nD:"+QString::number(Kd));

    series->clear();
    end_series->clear();
    for(int i=0; i<50; i++){
        series->append(i,current_temp);
        simulate_pid();
        end_series->append(i,goal_temp);
    }
}

