#include "VeiligheidModule.h"

VeiligheidModule::VeiligheidModule(QObject *parent)
    : QObject(parent)
{
}

void VeiligheidModule::verwerkInkomendeStatus(bool brand, bool overrule, bool ventilator)
{
    emit brandAlarmStatusGewijzigd(brand);
    emit overruleStatusGewijzigd(overrule);
    emit ventilatorStatusGewijzigd(ventilator);
}

void VeiligheidModule::activeerBrandOverrule()
{
    emit overruleStatusGewijzigd(true);
    emit brandAlarmStatusGewijzigd(false);
    emit logBerichtGegenereerd("GEBRUIKER: Brandmelding handmatig gereset via Dashboard.");

    emit stuurNetwerkCommando("COMMAND: ALARM_OVERRULED\n");
}
