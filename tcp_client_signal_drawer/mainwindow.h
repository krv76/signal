#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qcustomplot.h>

#include <QMainWindow>
#include <QTcpSocket>
#include <QTimer>

#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

const int frameExposeTime = 30;  // ms

class MainWindow : public QMainWindow
{
    Q_OBJECT

   public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

   private slots:
    void on_timer();
    void on_getColor_clicked();

   private:
    Ui::MainWindow* ui;
    void drawGraph(const QVector<double> x, const QVector<double> y,
                   const QString hLabel);
    QString requestData();
    std::pair<QString, QString> parseTimeAndData(const QString got);
    void requestAndRedraw();
    QTimer* ptimer;
    QColor color = Qt::green;
};
#endif  // MAINWINDOW_H
