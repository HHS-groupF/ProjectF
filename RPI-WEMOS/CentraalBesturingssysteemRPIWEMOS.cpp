#include "CentraalBesturingssysteemRPIWEMOS.h"

CentraalBesturingssysteemRPIWEMOS::CentraalBesturingssysteemRPIWEMOS(QObject *parent)
    : QObject(parent)
{
}

void CentraalBesturingssysteemRPIWEMOS::verwerkInkomendeStatus(const QJsonObject &jsonObj)
{
    if (jsonObj.contains("brandAlarm")) {
        emit brandAlarmStatusGewijzigd(jsonObj["brandAlarm"].toBool());
    }
    if (jsonObj.contains("overrule")) {
        emit overruleStatusGewijzigd(jsonObj["overrule"].toBool());
    }
    if (jsonObj.contains("ventilator")) {
        emit ventilatorStatusGewijzigd(jsonObj["ventilator"].toBool());
    }
}

void CentraalBesturingssysteemRPIWEMOS::activeerBrandOverrule()
{
    emit overruleStatusGewijzigd(true);
    emit brandAlarmStatusGewijzigd(false);
    emit logBerichtGegenereerd("GEBRUIKER: Brandmelding handmatig gereset via Dashboard.");

    emit stuurNetwerkCommando("COMMAND: ALARM_OVERRULED\n");
}