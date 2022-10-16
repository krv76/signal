#include "mainwindow.h"

#include "ui_mainwindow.h"

#include "qcustomplot.h"

#include <QTimer>

#include <memory>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ptimer = new QTimer();
    connect(ptimer, SIGNAL(timeout()), this, SLOT(on_timer()));
    ptimer->start(frameExposeTime);
}

MainWindow::~MainWindow()
{
    delete ui;
}

std::pair<QVector<double>, QVector<double>> gen_points(const QString input)
{
    QVector<double> resx, resy;
    auto from = input.cbegin();
    for (auto to = from; to != input.cend(); ++to)
    {
        if ((*to) == ' ')
        {
            const double number = QStringView(from, to).toString().toDouble();
            if (resx.length() == resy.length())
            {
                resx << number;
            }
            else
            {
                resy << number;
            }
            from = to + 1;
        }
    }
    return {resx, resy};
}

void MainWindow::on_timer()
{
    requestAndRedraw();
}

void MainWindow::requestAndRedraw()
{
    const auto request_result = requestData();
    if (request_result == "")
    {
        return;
    }
    const auto parse_result = parseTimeAndData(request_result);
    const auto time = parse_result.first;
    const auto data = parse_result.second;
    const auto points = gen_points(data);
    if (points.first.length() == 0)
        return;

    drawGraph(points.first, points.second, time);
    ui->textEdit->setPlainText("Draw successfully");
}

void MainWindow::drawGraph(const QVector<double> x, const QVector<double> y,
                           const QString hLabel)
{
    const int penWidth = ui->penWidth->value();
    const double vSize = ui->vSizeSelector->value();

    auto& customPlot = ui->graphic;

    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    customPlot->graph(0)->setPen(QPen(color, penWidth));

    customPlot->xAxis->setLabel(hLabel);
    customPlot->xAxis->setRange(x[0], x.back());

    customPlot->yAxis->setLabel("value");
    customPlot->yAxis->setRange(-vSize, vSize);

    customPlot->replot();
}

std::pair<QString, QString> MainWindow::parseTimeAndData(
    const QString stringToParse)
{
    QRegExp parser("now (.*) data (.*)");
    const int pos = parser.indexIn(stringToParse);
    if (pos == -1)
    {
        ui->textEdit->setPlainText(stringToParse + " : can't parse");
        return {{}, {}};
    }

    const auto time = parser.capturedTexts()[1];
    const auto data = parser.capturedTexts()[2];
    return {time, data};
}

QString MainWindow::requestData()
{
    std::unique_ptr<QTcpSocket> mpSocket(new QTcpSocket(this));
    mpSocket->connectToHost("127.0.0.1", 6000);
    if (!mpSocket->waitForConnected())
    {
        ui->textEdit->setPlainText("Can't connect");
        return {};
    }

    if (!mpSocket->waitForReadyRead())
    {
        ui->textEdit->setPlainText("Can't greeting");
        return {};
    }
    mpSocket->read(100000);

    mpSocket->write("from -");
    mpSocket->write(
        QString::number(ui->timeFrameBox->value()).toStdString().c_str());
    mpSocket->write(" to 0 count ");
    mpSocket->write(
        QString::number(ui->pointsCount->value()).toStdString().c_str());

    if (!mpSocket->waitForReadyRead())
    {
        ui->textEdit->setPlainText("Can't read request result");
        return {};
    }

    const auto request_result = QString(mpSocket->read(100000));
    return request_result;
}

void MainWindow::on_getColor_clicked()
{
    color = QColorDialog::getColor();
}
