#ifndef CENTRAALBESTURINGSSYSTEEMRPIBUS_H
#define CENTRAALBESTURINGSSYSTEEMRPIBUS_H

#include <QObject>
#include <QString>
#include <QByteArray>

class CentraalBesturingssysteemRPIBUS : public QObject
{
    Q_OBJECT
public:
    explicit CentraalBesturingssysteemRPIBUS(QObject *parent = nullptr);

signals:
    void stuurNetwerkData(const QString &data);
    void stuurCanCommando(uint32_t stmId, const QByteArray &data);

public slots:
    void verwerkCanSensorData(uint32_t stmId, const QString &sensorType, double waarde);
    void verwerkCommando(const QString &commando);
    void verwerkStmNoodstopReset();

private:
    double huidigTemp;
    double huidigCO2;
    double huidigLucht;
    bool isVentilatorAan;
    bool isBrandAlarmActief;
    bool isOverruleActief;

    void controleerLimieten();
    void verzendSysteemStatus();
};

#endif // CENTRAALBESTURINGSSYSTEEMRPIBUS_H
