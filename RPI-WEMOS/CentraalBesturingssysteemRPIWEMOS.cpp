#include "CentraalBesturingssysteemRPIWEMOS.h"

#include "IRuneVerwerker.h"
#include "RgbModule.h"
#include "TafelModule.h"
#include "VeiligheidModule.h"

CentraalBesturingssysteemRPIWEMOS::CentraalBesturingssysteemRPIWEMOS(QObject *parent)
    : QObject(parent)
{
    m_rgb        = new RgbModule(this);
    m_tafel      = new TafelModule(this);
    m_veiligheid = new VeiligheidModule(this);

    // Modules die Bifrost-runes verwerken, registreren voor de dispatch.
    m_verwerkers << m_rgb << m_tafel;

    // --- Module-signalen bundelen naar de coordinator-signalen ---------------
    // Veiligheid
    connect(m_veiligheid, &VeiligheidModule::ventilatorStatusGewijzigd,
            this, &CentraalBesturingssysteemRPIWEMOS::ventilatorStatusGewijzigd);
    connect(m_veiligheid, &VeiligheidModule::brandAlarmStatusGewijzigd,
            this, &CentraalBesturingssysteemRPIWEMOS::brandAlarmStatusGewijzigd);
    connect(m_veiligheid, &VeiligheidModule::overruleStatusGewijzigd,
            this, &CentraalBesturingssysteemRPIWEMOS::overruleStatusGewijzigd);
    connect(m_veiligheid, &VeiligheidModule::stuurNetwerkCommando,
            this, &CentraalBesturingssysteemRPIWEMOS::stuurNetwerkCommando);
    connect(m_veiligheid, &VeiligheidModule::logBerichtGegenereerd,
            this, &CentraalBesturingssysteemRPIWEMOS::logBerichtGegenereerd);

    // Tafels
    connect(m_tafel, &TafelModule::stuurBifrostBericht,
            this, &CentraalBesturingssysteemRPIWEMOS::stuurBifrostBericht);
    connect(m_tafel, &TafelModule::tafelStatusGewijzigd,
            this, &CentraalBesturingssysteemRPIWEMOS::tafelStatusGewijzigd);

    // RGB
    connect(m_rgb, &RgbModule::stuurBifrostBericht,
            this, &CentraalBesturingssysteemRPIWEMOS::stuurBifrostBericht);
    connect(m_rgb, &RgbModule::rgbStatusGewijzigd,
            this, &CentraalBesturingssysteemRPIWEMOS::rgbStatusGewijzigd);
    connect(m_rgb, &RgbModule::bewegingStatusGewijzigd,
            this, &CentraalBesturingssysteemRPIWEMOS::bewegingStatusGewijzigd);
}

void CentraalBesturingssysteemRPIWEMOS::verwerkInkomendeStatus(bool brand, bool overrule, bool ventilator)
{
    m_veiligheid->verwerkInkomendeStatus(brand, overrule, ventilator);
}

void CentraalBesturingssysteemRPIWEMOS::activeerBrandOverrule()
{
    m_veiligheid->activeerBrandOverrule();
}

void CentraalBesturingssysteemRPIWEMOS::verwerkBifrostRune(const QString &topic, const QString &payload)
{
    // Route de Rune naar de eerste module die het topic aankan.
    for (IRuneVerwerker *verwerker : m_verwerkers) {
        if (verwerker->kanVerwerken(topic)) {
            verwerker->verwerkRune(topic, payload);
            return;
        }
    }
}

void CentraalBesturingssysteemRPIWEMOS::resetTafel(int id)
{
    m_tafel->resetTafel(id);
}

void CentraalBesturingssysteemRPIWEMOS::zetRgbKleur(const QString &kleurNaam)
{
    m_rgb->zetRgbKleur(kleurNaam);
}

void CentraalBesturingssysteemRPIWEMOS::zetRgbUit()
{
    m_rgb->zetRgbUit();
}

void CentraalBesturingssysteemRPIWEMOS::zetRgbAutoModus(bool aan)
{
    m_rgb->zetRgbAutoModus(aan);
}

void CentraalBesturingssysteemRPIWEMOS::zetRgbKleurKeuze(const QString &kleurNaam)
{
    m_rgb->zetRgbKleurKeuze(kleurNaam);
}
