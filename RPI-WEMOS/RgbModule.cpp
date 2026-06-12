#include "RgbModule.h"
#include "SysteemConfig.h"

RgbModule::RgbModule(QObject *parent)
    : QObject(parent)
{
    // Timer die de RGB-sfeerlamp uitzet nadat de beweging is gestopt.
    // De vertraging is instelbaar via Config::RGB_UIT_VERTRAGING.
    rgbUitTimer = new QTimer(this);
    rgbUitTimer->setSingleShot(true);
    connect(rgbUitTimer, &QTimer::timeout, this, &RgbModule::zetRgbUit);
}

bool RgbModule::kanVerwerken(const QString &topic) const
{
    return topic == "sensor/beweging";
}

void RgbModule::verwerkRune(const QString &topic, const QString &payload)
{
    if (topic != "sensor/beweging") return;

    bool beweging = (payload == "JA");
    laatsteBeweging = beweging;
    emit bewegingStatusGewijzigd(beweging);

    if (rgbAutoModus) {
        if (beweging) {
            // Beweging: lamp meteen aan en een lopende uit-timer annuleren.
            rgbUitTimer->stop();
            zetRgbKleur(rgbKleurKeuze);
        } else if (Config::RGB_UIT_VERTRAGING > 0) {
            // Geen beweging: lamp pas na de ingestelde vertraging uitzetten.
            rgbUitTimer->start(Config::RGB_UIT_VERTRAGING);
        } else {
            // Vertraging 0: direct uit.
            zetRgbUit();
        }
    }
}

void RgbModule::zetRgbKleur(const QString &kleurNaam)
{
    QString rgbWaarde;
    if (!kleurNaarWaarde(kleurNaam, rgbWaarde)) return;
    emit stuurBifrostBericht("sensor/rgb/set", rgbWaarde);
    emit rgbStatusGewijzigd(rgbWaarde);
}

void RgbModule::zetRgbUit()
{
    emit stuurBifrostBericht("sensor/rgb/set", "UIT");
    emit rgbStatusGewijzigd("UIT");
}

void RgbModule::zetRgbAutoModus(bool aan)
{
    rgbAutoModus = aan;
}

void RgbModule::zetRgbKleurKeuze(const QString &kleurNaam)
{
    rgbKleurKeuze = kleurNaam;
    // In automatische modus blijft de lamp de bewegingssensor volgen: pas de
    // nieuwe kleur alleen live toe als er op dit moment beweging is (lamp al
    // aan). Zonder beweging onthouden we de kleur alleen voor de volgende
    // beweging; de lamp gaat dus niet ongewenst constant aan door kleurwissel.
    if (rgbAutoModus && laatsteBeweging) zetRgbKleur(rgbKleurKeuze);
}

bool RgbModule::kleurNaarWaarde(const QString &naam, QString &rgbWaarde) const
{
    if      (naam == "Wit")   rgbWaarde = Config::RGB_WIT;
    else if (naam == "Warm")  rgbWaarde = Config::RGB_WARM;
    else if (naam == "Rood")  rgbWaarde = Config::RGB_ROOD;
    else if (naam == "Blauw") rgbWaarde = Config::RGB_BLAUW;
    else if (naam == "Groen") rgbWaarde = Config::RGB_GROEN;
    else return false;
    return true;
}
