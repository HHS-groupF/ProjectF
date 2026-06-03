#include "CentraalBesturingssysteemRPIWEMOS.h"

CentraalBesturingssysteemRPIWEMOS::CentraalBesturingssysteemRPIWEMOS(QObject *parent)
    : QObject(parent)
{
}

void CentraalBesturingssysteemRPIWEMOS::verwerkInkomendeStatus(const QJsonObject &jsonObj)
{
    // 1. Verwerk eerst de overrule status
    if (jsonObj.contains("overrule")) {
        bool overruleBinnen = jsonObj["overrule"].toBool();
        emit overruleStatusGewijzigd(overruleBinnen);

        // Zodra de hardware bevestigt dat de overrule succesvol is verwerkt,
        // kunnen we onze tijdelijke frontend-blokkade opheffen.
        if (overruleBinnen == true) {
            handmatigeOverruleActief = false;
        }
    }

    // 2. Verwerk de brandalarm status
    if (jsonObj.contains("brandAlarm")) {
        bool brandBinnen = jsonObj["brandAlarm"].toBool();

        // Als de hardware zegt dat er sowieso gï¿½ï¿½n brand meer is,
        // resetten we de blokkade (de situatie is weer veilig).
        if (!brandBinnen) {
            handmatigeOverruleActief = false;
        }

        // De slimme check: We negeren het brandalarm als de hardware
        // nog naijlt terwijl wij zojuist de overrule hebben ingedrukt.
        if (handmatigeOverruleActief && brandBinnen) {
            // Doe niets: we wachten tot de hardware is bijgewerkt
        } else {
            emit brandAlarmStatusGewijzigd(brandBinnen);
        }
    }

    // 3. Verwerk de ventilator status
    if (jsonObj.contains("ventilator")) {
        emit ventilatorStatusGewijzigd(jsonObj["ventilator"].toBool());
    }
}

void CentraalBesturingssysteemRPIWEMOS::activeerBrandOverrule()
{
    // Zet de lokale blokkade AAN zodat we inkomende brandAlarmen even negeren
    handmatigeOverruleActief = true;

    emit overruleStatusGewijzigd(true);
    emit brandAlarmStatusGewijzigd(false);
    emit logBerichtGegenereerd("GEBRUIKER: Brandmelding handmatig gereset via Dashboard.");

    emit stuurNetwerkCommando("COMMAND: ALARM_OVERRULED\n");
}
