#include "CanBusCommunicatieRPIBUS.h"
#include <QDataStream>

CanBusCommunicatieRPIBUS::CanBusCommunicatieRPIBUS(QObject *parent) : QObject(parent) {}

CanBusCommunicatieRPIBUS::~CanBusCommunicatieRPIBUS() {
    if (canDevice) {
        canDevice->disconnectDevice();
        delete canDevice;
    }
}

void CanBusCommunicatieRPIBUS::start(const QString &interfaceName) {
    QString errorString;
    canDevice = QCanBus::instance()->createDevice("socketcan", interfaceName, &errorString);

    if (!canDevice) {
        qWarning() << "[CAN-BUS] Fout bij aanmaken CAN-device:" << errorString;
        return;
    }

    connect(canDevice, &QCanBusDevice::framesReceived, this, &CanBusCommunicatieRPIBUS::ontvangCanBerichten);

    if (!canDevice->connectDevice()) {
        qWarning() << "[CAN-BUS] Kan niet verbinden:" << canDevice->errorString();
    } else {
        qDebug() << "[CAN-BUS] Succesvol verbonden met" << interfaceName;
    }
}

void CanBusCommunicatieRPIBUS::ontvangCanBerichten() {
    if (!canDevice) return;

    while (canDevice->framesAvailable()) {
        const QCanBusFrame frame = canDevice->readFrame();
        uint32_t stmId = frame.frameId();
        QByteArray payload = frame.payload();

        if (payload.size() == 4) {
            QString sensorType;
            if (stmId == 0x100) sensorType = "CO2";
            else if (stmId == 0x101) sensorType = "TEMP";
            else if (stmId == 0x102) sensorType = "HUM";
            else continue; 

            float sensorWaarde;
            QDataStream stream(payload);
            stream.setByteOrder(QDataStream::LittleEndian);
            stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
            stream >> sensorWaarde;

            emit inkomendeSensorData(stmId, sensorType, static_cast<double>(sensorWaarde));
        } 
        else if (payload.size() == 1 && stmId == 0x300 && payload.at(0) == 0x01) {
            qDebug() << " [CAN-BUS] Noodstop vrijgegeven vanaf STM32 fysieke knop!";
            emit noodstopVrijgegeven();
        } 
        else {
            qDebug() << "[CAN-BUS] Genegeerd: Onbekend ID of lengte (" << payload.size() << "bytes)";
        }
    }
}

void CanBusCommunicatieRPIBUS::verstuurCommandoNaarSTM(uint32_t stmId, const QByteArray &data) {
    if (!canDevice || canDevice->state() != QCanBusDevice::ConnectedState) return;
    QCanBusFrame frame;
    frame.setFrameId(stmId);
    frame.setPayload(data);
    frame.setFrameType(QCanBusFrame::DataFrame);
    canDevice->writeFrame(frame);
}
