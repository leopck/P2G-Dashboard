#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <persistence1d.hpp>

#include <QFont>
#include <QValueAxis>

const size_t CHART_TITLE_FONT_SIZE = 20;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initializeTimeDataPlot();
    initializeRangeDataPlot();
    initializeTargetDataPlot();

    ui->range_data->chart()->layout()->setContentsMargins(0, 0, 0, 0);
    ui->range_data->chart()->setBackgroundRoundness(0);

    ui->time_data->chart()->layout()->setContentsMargins(0, 0, 0, 0);
    ui->time_data->chart()->setBackgroundRoundness(0);

    ui->target_data->chart()->layout()->setContentsMargins(0, 0, 0, 0);
    ui->target_data->chart()->setBackgroundRoundness(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit closed();
    event->accept();
}

void MainWindow::initializeTimeDataPlot()
{
    m_time_data_series_re_rx1 = new QLineSeries();
    m_time_data_series_im_rx1 = new QLineSeries();
    m_time_data_series_re_rx1->setName("Inphase component antenna 1");
    m_time_data_series_im_rx1->setName("Quadratur component antenna 1");

    m_time_data_series_re_rx2 = new QLineSeries();
    m_time_data_series_im_rx2 = new QLineSeries();
    m_time_data_series_re_rx2->setName("Inphase component antenna 2");
    m_time_data_series_im_rx2->setName("Quadratur component antenna 2");

    QChart *chart = new QChart();
    chart->setTheme(QChart::ChartThemeBlueCerulean);
    chart->setTitle("Time domain");
    chart->addSeries(m_time_data_series_re_rx1);
    chart->addSeries(m_time_data_series_im_rx1);
    chart->addSeries(m_time_data_series_re_rx2);
    chart->addSeries(m_time_data_series_im_rx2);

    QValueAxis *axisX = new QValueAxis;
    axisX->setTitleText("Samples");
    axisX->setRange(0, 63);
    axisX->setLabelFormat("%d");
    chart->addAxis(axisX, Qt::AlignBottom);
    m_time_data_series_re_rx1->attachAxis(axisX);
    m_time_data_series_im_rx1->attachAxis(axisX);
    m_time_data_series_re_rx2->attachAxis(axisX);
    m_time_data_series_im_rx2->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setTitleText("Amplitude");
    axisY->setRange(0, 1.2);
    axisY->setLabelFormat("%.1f");
    chart->addAxis(axisY, Qt::AlignLeft);
    m_time_data_series_re_rx1->attachAxis(axisY);
    m_time_data_series_im_rx1->attachAxis(axisY);
    m_time_data_series_re_rx2->attachAxis(axisY);
    m_time_data_series_im_rx2->attachAxis(axisY);

    QFont font;
    font.setPixelSize(CHART_TITLE_FONT_SIZE);
    chart->setTitleFont(font);

    ui->time_data->setChart(chart);
    ui->time_data->setRenderHint(QPainter::Antialiasing);
    ui->time_data->show();
}

void MainWindow::initializeRangeDataPlot()
{
    m_range_data_series_upper_rx1 = new QLineSeries();
    m_range_data_series_rx1 = new QAreaSeries(m_range_data_series_upper_rx1);
    m_range_data_series_rx1->setName("Antenna 1");

    m_range_data_series_upper_rx2 = new QLineSeries();
    m_range_data_series_rx2 = new QAreaSeries(m_range_data_series_upper_rx2);
    m_range_data_series_rx2->setName("Antenna 2");

    m_range_data_maximum_rx1 = new QScatterSeries();
    m_range_data_maximum_rx1->setName("All extrema");
    m_range_data_maximum_rx1->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    m_range_data_maximum_rx1->setMarkerSize(12);
    m_range_data_maximum_rx1->setBrush(Qt::red);
    m_range_data_maximum_rx1->setBorderColor(Qt::white);
    m_range_data_maximum_rx1->setPointLabelsVisible(true);
    m_range_data_maximum_rx1->setPointLabelsColor(Qt::white);
    m_range_data_maximum_rx1->setPointLabelsFormat("@xPoint m");

    QChart *chart = new QChart();
    chart->setTheme(QChart::ChartThemeBlueCerulean);
    chart->addSeries(m_range_data_series_rx1);
    chart->addSeries(m_range_data_series_rx2);
    chart->addSeries(m_range_data_maximum_rx1);
    chart->setTitle("Range spectrum");

    QValueAxis *axisX = new QValueAxis;
    axisX->setTitleText("Range [m]");
    axisX->setRange(0, 10);
    axisX->setLabelFormat("%.1f");
    chart->addAxis(axisX, Qt::AlignBottom);
    m_range_data_series_rx1->attachAxis(axisX);
    m_range_data_series_rx2->attachAxis(axisX);
    m_range_data_maximum_rx1->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setTitleText("Magnitude");
    axisY->setRange(0, 1.0);
    axisY->setLabelFormat("%.1f");
    chart->addAxis(axisY, Qt::AlignLeft);
    m_range_data_series_rx1->attachAxis(axisY);
    m_range_data_series_rx2->attachAxis(axisY);
    m_range_data_maximum_rx1->attachAxis(axisY);

    QFont font;
    font.setPixelSize(CHART_TITLE_FONT_SIZE);
    chart->setTitleFont(font);

    ui->range_data->setChart(chart);
    ui->range_data->setRenderHint(QPainter::Antialiasing);
    ui->range_data->show();
}

void MainWindow::initializeTargetDataPlot()
{
    m_target_data_series = new QScatterSeries();
    m_target_data_series->setName("Detected targets");
    m_target_data_series->setMarkerSize(12);
    m_target_data_series->setColor(Qt::red);
    m_target_data_series->setBorderColor(Qt::white);

    QPolarChart *chart = new QPolarChart();
    chart->setTheme(QChart::ChartThemeBlueCerulean);
    chart->addSeries(m_target_data_series);    
    chart->setTitle("Polar plot of targets");

    QValueAxis *angularAxis = new QValueAxis();
    angularAxis->setRange(-180, 180);
    angularAxis->setTickAnchor(180);
    angularAxis->setTickCount(9);
    angularAxis->setLabelFormat("%d" + QString::fromLatin1("°"));
    chart->addAxis(angularAxis, QPolarChart::PolarOrientationAngular);
    m_target_data_series->attachAxis(angularAxis);

    QValueAxis *radialAxis = new QValueAxis();
    radialAxis->setRange(0, 10);
    radialAxis->setTickCount(6);
    radialAxis->setLabelFormat("%d [m]");
    chart->addAxis(radialAxis, QPolarChart::PolarOrientationRadial);
    m_target_data_series->attachAxis(radialAxis);

    QFont font;
    font.setPixelSize(CHART_TITLE_FONT_SIZE);
    chart->setTitleFont(font);

    ui->target_data->setChart(chart);
    ui->target_data->setRenderHint(QPainter::Antialiasing);
    ui->target_data->show();
}

void MainWindow::updateTimeData(QList<QPointF> const & re_rx1, QList<QPointF> const & im_rx1,
                                 QList<QPointF> const & re_rx2, QList<QPointF> const & im_rx2)
{
    m_time_data_series_re_rx1->clear();
    m_time_data_series_im_rx1->clear();
    m_time_data_series_re_rx2->clear();
    m_time_data_series_im_rx2->clear();

    m_time_data_series_re_rx1->append(re_rx1);
    m_time_data_series_im_rx1->append(im_rx1);
    m_time_data_series_re_rx2->append(re_rx2);
    m_time_data_series_im_rx2->append(im_rx2);
}

void MainWindow::updateRangeData(QList<QPointF> const & rx1, QList<QPointF> const & rx2)
{
    m_range_data_series_upper_rx1->clear();
    m_range_data_series_upper_rx2->clear();
    m_range_data_series_upper_rx1->append(rx1);
    m_range_data_series_upper_rx2->append(rx2);

    calculateRangeMaxima(rx1);
}

void MainWindow::updateTargetData(const QVector<Target_Info_t> &data)
{
    m_target_data_series->clear();

    for (auto & e: data)
        m_target_data_series->append(QPointF(e.azimuth, e.radius / 100));
}

void MainWindow::calculateRangeMaxima(QList<QPointF> const & rx1)
{
    std::vector<float> data;

    for (int i = 0; i < rx1.size(); i++)
        data.push_back(rx1[i].y());

    p1d::Persistence1D p;
    p.RunPersistence(data);

    std::vector<p1d::TPairedExtrema> extrema;
    p.GetPairedExtrema(extrema, 0.01);

    auto maximum = 0.0;
    m_range_data_maximum_rx1->clear();

    for(std::vector<p1d::TPairedExtrema>::iterator it = extrema.begin(); it != extrema.end(); it++)
    {
        auto x = rx1[(*it).MaxIndex].x();
        auto y = rx1[(*it).MaxIndex].y();

        m_range_data_maximum_rx1->append(QPointF(x, y));

        if (y > maximum)
            maximum = y;
    }
    static_cast<QValueAxis*>(ui->range_data->chart()->axes(Qt::Vertical).back())->setMax(maximum + 0.2);
}

