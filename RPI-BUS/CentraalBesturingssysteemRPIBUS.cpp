#include "CentraalBesturingssysteemRPIBUS.h"
#include "SysteemConfig.h"
#include <QByteArray>

CentraalBesturingssysteemRPIBUS::CentraalBesturingssysteemRPIBUS(QObject *parent)
    : QObject(parent)
{
    // Veilige startwaarden instellen (kamertemperatuur)
    huidigTemp = 20.0;
    huidigCO2 = 400.0;
    huidigLucht = 45.0;
    isVentilatorAan = false;
    isBrandAlarmActief = false;
    isOverruleActief = false;
}

void CentraalBesturingssysteemRPIBUS::verwerkCanSensorData(uint32_t stmId, const QString &sensorType, double waarde) {
    // 1. Update de interne administratie op basis van de binnengekomen bus-data
    if (sensorType == "TEMP") {
        huidigTemp = waarde;
    } else if (sensorType == "CO2") {
        huidigCO2 = waarde;
    } else if (sensorType == "HUM") {
        huidigLucht = waarde;
    }

    // 2. Stuur de live-data als eigen Bifrost-regel naar de andere Pi (geen JSON-library):
    //    "SENSOR <nodeId> <sensorId> <waarde>"
    QString bericht = QString("SENSOR %1 %2 %3\n")
                          .arg(static_cast<int>(stmId))
                          .arg(sensorType)
                          .arg(waarde);
    emit stuurNetwerkData(bericht);

    // 3. Evalueer direct de limieten met de nieuwste meetwaarden
    controleerLimieten();
}

void CentraalBesturingssysteemRPIBUS::verwerkCommando(const QString &commando) {
    if (commando.contains("ALARM_OVERRULED")) {
        if (isBrandAlarmActief) {
            isBrandAlarmActief = false;
            isOverruleActief = true;
            controleerLimieten(); // Bereken direct de nieuwe staat van de ventilator
        }
    }
}

void CentraalBesturingssysteemRPIBUS::controleerLimieten() {
    bool brandGevaar = (huidigTemp > Config::TEMP_BRAND || huidigCO2 > Config::CO2_BRAND);

    // --- 1. BRANDALARM VERGRENDELING (LATCH) ---
    if (brandGevaar) {
        if (!isOverruleActief) {
            isBrandAlarmActief = true;
        }
    } else {
        // Zodra de waarden weer veilig zijn (geen brandgevaar), 
        // zet het alarm én de eventuele overrule direct weer uit.
        isBrandAlarmActief = false;
        isOverruleActief = false;
    }

    // --- 2. NORMALE WAARSCHUWINGEN ---
    bool teWarm = (huidigTemp > Config::TEMP_WAARSCHUWING);
    bool teVeelCO2 = (huidigCO2 > Config::CO2_WAARSCHUWING);
    bool teVochtig = (huidigLucht > Config::HUM_WAARSCHUWING);

    // --- 3. VENTILATOR LOGICA & BUS COMMANDO ---
    bool vorigeVentilatorStaat = isVentilatorAan;

    if (isBrandAlarmActief) {
        isVentilatorAan = false; // Absolute blokkade bij actieve brand
    } else {
        isVentilatorAan = (teWarm || teVeelCO2 || teVochtig);
    }

    // Controleer of de status van de ventilator daadwerkelijk is veranderd
    if (isVentilatorAan != vorigeVentilatorStaat) {
        QByteArray canPayload;

        if (isVentilatorAan) {
            canPayload.append(static_cast<char>(0x01)); // 0x01 staat voor: Ventilator/LED AAN
        } else {
            canPayload.append(static_cast<char>(0x00)); // 0x00 staat voor: Ventilator/LED UIT
        }

        // Verstuur het fysieke aan/uit commando over de CAN-bus naar de specifieke STM32
        emit stuurCanCommando(STM32_VENTILATOR_ID, canPayload);
    }

    // Netwerkstatus updaten voor de andere Pi
    verzendSysteemStatus();
}

void CentraalBesturingssysteemRPIBUS::verzendSysteemStatus() {
    // Eigen Bifrost-regel (geen JSON-library): "STATUS <brand> <overrule> <ventilator>" met 1/0.
    QString bericht = QString("STATUS %1 %2 %3\n")
                          .arg(isBrandAlarmActief ? 1 : 0)
                          .arg(isOverruleActief ? 1 : 0)
                          .arg(isVentilatorAan ? 1 : 0);
    emit stuurNetwerkData(bericht);
}
