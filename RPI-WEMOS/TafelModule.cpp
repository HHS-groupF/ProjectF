#include "TafelModule.h"
#include <QStringList>

TafelModule::TafelModule(QObject *parent)
    : QObject(parent)
{
}

bool TafelModule::kanVerwerken(const QString &topic) const
{
    // tafel/<id>/status
    const QStringList delen = topic.split('/');
    return delen.size() == 3 && delen[0] == "tafel" && delen[2] == "status";
}

void TafelModule::verwerkRune(const QString &topic, const QString &payload)
{
    const QStringList delen = topic.split('/');
    if (delen.size() != 3) return;

    const int id = delen[1].toInt();
    const bool hulpNodig = (payload == "HELP");
    if (hulpNodig || payload == "OK") {
        emit tafelStatusGewijzigd(id, hulpNodig);
    }
}

void TafelModule::resetTafel(int id)
{
    emit stuurBifrostBericht("tafel/" + QString::number(id) + "/reset", "RESET");
}
