#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QElapsedTimer>

#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>

#include "SocketCommunicatieRPIWEMOS.h"
#include "CentraalBesturingssysteemRPIWEMOS.h"

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
    void updateScherm();
    void on_pushButton_Update_Lichtkrant_clicked();
    void on_pushButton_Brandoverruleknop_clicked();
private:
    Ui::MainWindow *ui;

    SocketCommunicatieRPIWEMOS *socketComm;
    CentraalBesturingssysteemRPIWEMOS *centraalSysteem;

    QTimer *uiTimer;
    QElapsedTimer timerSindsStart;

    QLineSeries *seriesTemp;
    QLineSeries *seriesCO2;
    QLineSeries *seriesLucht;

    QLineSeries *seriesWaarschuwingTemp;
    QLineSeries *seriesWaarschuwingCO2;
    QLineSeries *seriesWaarschuwingLucht;

    QLineSeries *seriesBrandTemp;
    QLineSeries *seriesBrandCO2;

    QChart *chartTemp;
    QChart *chartCO2;
    QChart *chartLucht;

    QChartView *chartViewTemp;
    QChartView *chartViewCO2;
    QChartView *chartViewLucht;

    QValueAxis *asX_Temp;
    QValueAxis *asY_Temp;
    QValueAxis *asX_CO2;
    QValueAxis *asY_CO2;
    QValueAxis *asX_Lucht;
    QValueAxis *asY_Lucht;

    void setupGrafieken();
    void beheerWaarschuwing(const QString &msg, bool actief);
};
#endif // MAINWINDOW_H