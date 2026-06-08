#include "CentraalBesturingssysteemRPIWEMOS.h"
#include "SysteemConfig.h"
#include <QStringList>

CentraalBesturingssysteemRPIWEMOS::CentraalBesturingssysteemRPIWEMOS(QObject *parent)
    : QObject(parent)
{
}

void CentraalBesturingssysteemRPIWEMOS::verwerkInkomendeStatus(bool brand, bool overrule, bool ventilator)
{
    emit brandAlarmStatusGewijzigd(brand);
    emit overruleStatusGewijzigd(overrule);
    emit ventilatorStatusGewijzigd(ventilator);
}

void CentraalBesturingssysteemRPIWEMOS::activeerBrandOverrule()
{
    emit overruleStatusGewijzigd(true);
    emit brandAlarmStatusGewijzigd(false);
    emit logBerichtGegenereerd("GEBRUIKER: Brandmelding handmatig gereset via Dashboard.");

    emit stuurNetwerkCommando("COMMAND: ALARM_OVERRULED\n");
}

// ===========================================================================
//  Tafel- en RGB-logica (Wemos-runes via Heimdall)
// ===========================================================================
void CentraalBesturingssysteemRPIWEMOS::verwerkBifrostRune(const QString &topic, const QString &payload)
{
    QStringList delen = topic.split('/');

    // tafel/<id>/status  ->  HELP / OK
    if (delen.size() == 3 && delen[0] == "tafel" && delen[2] == "status") {
        int id = delen[1].toInt();
        bool hulpNodig = (payload == "HELP");
        if (hulpNodig || payload == "OK") {
            emit tafelStatusGewijzigd(id, hulpNodig);
        }
    }
    // sensor/beweging  ->  JA / NEE  (stuurt in automatische modus de RGB)
    else if (topic == "sensor/beweging") {
        bool beweging = (payload == "JA");
        emit bewegingStatusGewijzigd(beweging);

        if (rgbAutoModus) {
            if (beweging) zetRgbKleur(rgbKleurKeuze);
            else          zetRgbUit();
        }
    }
}

void CentraalBesturingssysteemRPIWEMOS::resetTafel(int id)
{
    emit stuurBifrostBericht("tafel/" + QString::number(id) + "/reset", "RESET");
}

void CentraalBesturingssysteemRPIWEMOS::zetRgbKleur(const QString &kleurNaam)
{
    QString rgbWaarde;
    if (!kleurNaarWaarde(kleurNaam, rgbWaarde)) return;
    emit stuurBifrostBericht("sensor/rgb/set", rgbWaarde);
    emit rgbStatusGewijzigd(rgbWaarde);
}

void CentraalBesturingssysteemRPIWEMOS::zetRgbUit()
{
    emit stuurBifrostBericht("sensor/rgb/set", "UIT");
    emit rgbStatusGewijzigd("UIT");
}

void CentraalBesturingssysteemRPIWEMOS::zetRgbAutoModus(bool aan)
{
    rgbAutoModus = aan;
}

void CentraalBesturingssysteemRPIWEMOS::zetRgbKleurKeuze(const QString &kleurNaam)
{
    rgbKleurKeuze = kleurNaam;
    // In automatische modus: pas de gekozen kleur meteen toe (live), zodat je je
    // keuze direct ziet en niet pas bij de volgende beweging.
    if (rgbAutoModus) zetRgbKleur(rgbKleurKeuze);
}

bool CentraalBesturingssysteemRPIWEMOS::kleurNaarWaarde(const QString &naam, QString &rgbWaarde) const
{
    if      (naam == "Wit")   rgbWaarde = Config::RGB_WIT;
    else if (naam == "Warm")  rgbWaarde = Config::RGB_WARM;
    else if (naam == "Rood")  rgbWaarde = Config::RGB_ROOD;
    else if (naam == "Blauw") rgbWaarde = Config::RGB_BLAUW;
    else if (naam == "Groen") rgbWaarde = Config::RGB_GROEN;
    else return false;
    return true;
}
