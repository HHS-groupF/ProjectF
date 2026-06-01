#ifndef CENTRAALBESTURINGSSYSTEEMRPIBUS_H
#define CENTRAALBESTURINGSSYSTEEMRPIBUS_H

#include <QObject>
#include <QTimer>
#include <QString>

class CentraalBesturingssysteemRPIBUS : public QObject {
    Q_OBJECT
public:
    explicit CentraalBesturingssysteemRPIBUS(QObject *parent = nullptr);
public slots:
    void verwerkCommando(const QString &commando);
signals:
    void stuurNetwerkData(const QString &data);
private slots:
    void genereerEnVerwerkSensorData();
private:
    QTimer *sensorTimer;
    double huidigTemp = 0.0;
    double huidigCO2 = 0.0;
    double huidigLucht = 0.0;
    bool isVentilatorAan = false;
    bool isBrandAlarmActief = false;
    bool isOverruleActief = false;
    bool wasTeWarm = false;
    bool wasTeVeelCO2 = false;
    bool wasTeVochtig = false;
    void controleerLimieten();
    void verzendSysteemStatus();
};

#endif // CENTRAALBESTURINGSSYSTEEMRPIBUS_H