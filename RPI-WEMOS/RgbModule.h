#ifndef RGBMODULE_H
#define RGBMODULE_H

#include <QObject>
#include <QString>
#include <QTimer>

#include "IRuneVerwerker.h"

// ============================================================================
//  RgbModule — RGB-sfeerverlichting.
// ----------------------------------------------------------------------------
//  Volgt (optioneel) de bewegingssensor: lamp aan bij beweging, en na een
//  instelbare uitloop (Config::RGB_UIT_VERTRAGING) weer uit. Of handmatig
//  bediend. Verwerkt het Bifrost-topic "sensor/beweging" en stuurt zelf
//  "sensor/rgb/set" terug naar de Wemos.
// ============================================================================
class RgbModule : public QObject, public IRuneVerwerker {
    Q_OBJECT
public:
    explicit RgbModule(QObject *parent = nullptr);

    // IRuneVerwerker
    bool kanVerwerken(const QString &topic) const override;
    void verwerkRune(const QString &topic, const QString &payload) override;

public slots:
    void zetRgbKleur(const QString &kleurNaam);
    void zetRgbUit();
    void zetRgbAutoModus(bool aan);                  // checkbox: RGB volgt beweging
    void zetRgbKleurKeuze(const QString &kleurNaam); // onthoudt de combobox-keuze

signals:
    void stuurBifrostBericht(const QString &topic, const QString &payload);
    void rgbStatusGewijzigd(const QString &rgbWaarde); // "r,g,b" of "UIT"
    void bewegingStatusGewijzigd(bool beweging);

private:
    bool kleurNaarWaarde(const QString &naam, QString &rgbWaarde) const;

    bool rgbAutoModus = true;        // standaard: RGB volgt de bewegingssensor
    bool laatsteBeweging = false;    // huidige bewegingsstatus
    QString rgbKleurKeuze = "Wit";   // laatst gekozen kleur in de combobox
    QTimer *rgbUitTimer = nullptr;   // uitloop-vertraging na "geen beweging"
};

#endif // RGBMODULE_H
