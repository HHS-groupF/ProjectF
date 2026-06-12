#include "BusProtocolParser.h"
#include <QStringList>
#include <QRegularExpression>

BusProtocolParser::BusProtocolParser(QObject *parent)
    : QObject(parent)
{
}

void BusProtocolParser::verwerk(const QString &ruweData)
{
    if (!ruweData.isEmpty()) buffer += ruweData;

    // Verwerk alle volledige regels die momenteel in de buffer zitten.
    while (buffer.contains('\n')) {
        const int pos = buffer.indexOf('\n');
        const QString ruw = buffer.left(pos);
        buffer.remove(0, pos + 1); // verwijder de verwerkte regel incl. de '\n'

        const QString schoon = ruw.trimmed();
        if (schoon.isEmpty()) continue;

        // --- HEARTBEAT ---
        if (schoon == "HEARTBEAT") {
            emit heartbeatOntvangen();
            continue;
        }

        // --- HARDWARE LOG (bv. fysieke STM32 brandoverrule-meldingen) ---
        if (schoon.startsWith("LOG ")) {
            emit hardwareLog(schoon.mid(4)); // haal "LOG " ervan af
            continue;
        }

        // Elke overige regel komt ook in het logboek.
        emit algemeenBerichtOntvangen(schoon);

        // --- Parser: splits op witruimte (spaties en tabs) ---
        const QStringList delen = schoon.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (delen.isEmpty()) continue;
        const QString type = delen[0];

        // STATUS <brand> <overrule> <ventilator>
        if (type == "STATUS" && delen.size() >= 4) {
            emit gebouwStatus(delen[1] == "1", delen[2] == "1", delen[3] == "1");
        }
        // SENSOR <nodeId> <sensorId> <waarde>
        else if (type == "SENSOR" && delen.size() >= 4) {
            emit sensorMeting(delen[2], delen[3].toDouble());
        }
    }
}
