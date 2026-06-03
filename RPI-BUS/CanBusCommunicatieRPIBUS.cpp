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
    // Gebruik socketcan voor Linux/Raspberry Pi
    canDevice = QCanBus::instance()->createDevice("socketcan", interfaceName, &errorString);

    if (!canDevice) {
        qWarning() << "[CAN-BUS] Fout bij aanmaken CAN-device:" << errorString;
        return;
    }

    connect(canDevice, &QCanBusDevice::framesReceived, this, &CanBusCommunicatieRPIBUS::ontvangCanBerichten);

    if (!canDevice->connectDevice()) {
        qWarning() << "[CAN-BUS] Kan niet verbinden met interface" << interfaceName << ":" << canDevice->errorString();
    } else {
        qDebug() << "[CAN-BUS] Succesvol verbonden met" << interfaceName;
    }
}

void CanBusCommunicatieRPIBUS::ontvangCanBerichten() {
    if (!canDevice) return;

    while (canDevice->framesAvailable()) {
        const QCanBusFrame frame = canDevice->readFrame();

        // Voorbeeld protocol interpretatie:
        // Frame ID = STM32 Node ID (bijv. 0x10, 0x11)
        // Byte 0: Sensor Type (0x01 = TEMP, 0x02 = CO2, 0x03 = HUM)
        // Byte 1-4: Float waarde van de sensor

        uint32_t stmId = frame.frameId();
        QByteArray payload = frame.payload();

        if (payload.size() >= 5) {
            char typeByte = payload.at(0);
            QString sensorType;
            if (typeByte == 0x01) sensorType = "TEMP";
            else if (typeByte == 0x02) sensorType = "CO2";
            else if (typeByte == 0x03) sensorType = "HUM";
            else continue; // Onbekend type

            // Converteer de volgende 4 bytes naar een double/float
            float sensorWaarde;
            QDataStream stream(payload.mid(1, 4));
            stream.setByteOrder(QDataStream::LittleEndian); // Pas aan op basis van je STM32 byte order
            stream >> sensorWaarde;

            emit inkomendeSensorData(stmId, sensorType, static_cast<double>(sensorWaarde));
        }
    }
}

void CanBusCommunicatieRPIBUS::verstuurCommandoNaarSTM(uint32_t stmId, const QByteArray &data) {
    if (!canDevice || !canDevice->state() == QCanBusDevice::ConnectedState) return;

    QCanBusFrame frame;
    frame.setFrameId(stmId);
    frame.setPayload(data);
    frame.setFrameType(QCanBusFrame::DataFrame);

    if (!canDevice->writeFrame(frame)) {
        qWarning() << "[CAN-BUS] Fout bij verzenden frame naar STM" << stmId << ":" << canDevice->errorString();
    }
}