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

        uint32_t stmId = frame.frameId();
        QByteArray payload = frame.payload();

        // 1. Print het onbewerkte bericht
        qDebug() << "[CAN-BUS] RAW Frame Ontvangen - ID:" << Qt::hex << stmId
                 << "| Payload:" << payload.toHex()
                 << "| Lengte:" << payload.size();

        // 2. STM32 verstuurt exact 4 bytes per id (alleen de float)
        if (payload.size() == 4) {
            QString sensorType;

            // Bepaal het type sensor op basis van het STM32 CAN ID
            if (stmId == 0x100) sensorType = "CO2";
            else if (stmId == 0x101) sensorType = "TEMP";
            else if (stmId == 0x102) sensorType = "HUM";
            else {
                qDebug() << "[CAN-BUS] Genegeerd: Onbekend CAN ID:" << Qt::hex << stmId;
                continue; // Geen relevant id, ga door
            }

            // Converteer de 4 bytes direct naar een float
            float sensorWaarde;
            QDataStream stream(payload);
            stream.setByteOrder(QDataStream::LittleEndian); // STM32 ARM processors zijn Little Endian
            stream.setFloatingPointPrecision(QDataStream::SinglePrecision); // Zorg ervoor dat hij 32-bit float pakt
            stream >> sensorWaarde;

            // 3. Print de succesvol geparseerde data
            qDebug() << "[CAN-BUS] Geparseerde Data - ID:" << Qt::hex << stmId
                     << "| Type:" << sensorType
                     << "| Waarde:" << sensorWaarde;

            emit inkomendeSensorData(stmId, sensorType, static_cast<double>(sensorWaarde));
        } else {
            qDebug() << "[CAN-BUS] Genegeerd: Payload heeft een onverwachte lengte (is" << payload.size() << "bytes, verwacht 4)";
        }
    }
}

void CanBusCommunicatieRPIBUS::verstuurCommandoNaarSTM(uint32_t stmId, const QByteArray &data) {
    if (!canDevice || canDevice->state() != QCanBusDevice::ConnectedState) return;

    QCanBusFrame frame;
    frame.setFrameId(stmId);
    frame.setPayload(data);
    frame.setFrameType(QCanBusFrame::DataFrame);

    if (!canDevice->writeFrame(frame)) {
        qWarning() << "[CAN-BUS] Fout bij verzenden frame naar STM" << stmId << ":" << canDevice->errorString();
    }
}
