#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SysteemConfig.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QListWidget>
#include <QPen>
#include <QColor>
#include <QDebug>
#include <QMqttTopicFilter>
#include <QMqttTopicName>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupGrafieken();
    timerSindsStart.start();

    socketComm = new SocketCommunicatieRPIWEMOS(this);

    connect(socketComm, &SocketCommunicatieRPIWEMOS::nieuwLogBericht, this, [this](QString bericht) {
        QString tijd = QDateTime::currentDateTime().toString("hh:mm:ss");
        ui->textBrowser_Logboek->append(tijd + " - " + bericht);
    });

    if (socketComm->verbind()) {
        ui->textBrowser_Logboek->append("Netwerk backend gestart. Luistert naar poort " + QString::number(Config::POORT_WEMOS_DATA) + "...");
    }

    mqttClient = new QMqttClient(this);
    mqttClient->setHostname(Config::MQTT_BROKER);
    mqttClient->setPort(Config::MQTT_PORT);

    connect(mqttClient, &QMqttClient::connected, this, [this]() {
        ui->textBrowser_Logboek->append("MQTT verbonden met broker.");
        mqttClient->subscribe(QMqttTopicFilter("tafel/+/status"));
        mqttClient->subscribe(QMqttTopicFilter("sensor/beweging"));
    });

    connect(mqttClient, &QMqttClient::messageReceived, this, [this](const QByteArray &bericht, const QMqttTopicName &topic) {
        QString topicStr = topic.name();
        QString payload  = QString::fromUtf8(bericht);
        QStringList delen = topicStr.split('/');

        if (delen.size() == 3 && delen[0] == "tafel" && delen[2] == "status") {
            QString tafelId = delen[1];
            QString tijd = QDateTime::currentDateTime().toString("hh:mm:ss");
            bool hulpNodig = (payload == "HELP");

            if (hulpNodig) {
                ui->textBrowser_Logboek->append("<b>" + tijd + " - [TAFEL] Tafel " + tafelId + " vraagt hulp!</b>");
                beheerWaarschuwing("Tafel " + tafelId + " vraagt hulp!", true);
            } else if (payload == "OK") {
                ui->textBrowser_Logboek->append(tijd + " - [TAFEL] Tafel " + tafelId + " is geholpen.");
                beheerWaarschuwing("Tafel " + tafelId + " vraagt hulp!", false);
            }

            // Update de status-LED voor tafel 1
            if (tafelId == "1") {
                if (hulpNodig)
                    ui->label_Status_Drukknop_1->setStyleSheet("background-color: orange; border-radius: 45px;");
                else
                    ui->label_Status_Drukknop_1->setStyleSheet("background-color: green; border-radius: 45px;");
            }
        }

        else if (topicStr == "sensor/beweging") {
            bool beweging = (payload == "JA");
            QString aanStijl = "background-color: green; border-radius: 45px;";
            QString uitStijl = "background-color: rgb(170, 0, 0); border-radius: 45px;";
            ui->label_Status_Bewegingssensor->setStyleSheet(beweging ? aanStijl : uitStijl);
            ui->label_Status_Verlichting_LED->setStyleSheet(beweging ? aanStijl : uitStijl);
        }
    });

    connect(mqttClient, &QMqttClient::disconnected, this, [this]() {
        ui->textBrowser_Logboek->append("MQTT verbinding verbroken. Opnieuw verbinden...");
        mqttClient->connectToHost();
    });

    mqttClient->connectToHost();

    centraalSysteem = new CentraalBesturingssysteemRPIWEMOS(this);

    // --- UI Kleuren en Status koppelen ---
    connect(centraalSysteem, &CentraalBesturingssysteemRPIWEMOS::ventilatorStatusGewijzigd, this, [this](bool aan) {
        if (aan) ui->label_Status_Ventilator->setStyleSheet("background-color: green; border-radius: 45px;");
        else ui->label_Status_Ventilator->setStyleSheet("background-color: rgb(170, 0, 0); border-radius: 45px;");
    });

    connect(centraalSysteem, &CentraalBesturingssysteemRPIWEMOS::brandAlarmStatusGewijzigd, this, [this](bool actief) {
        if (actief) ui->label_Status_Branddetectie->setStyleSheet("background-color: green; border-radius: 45px;");
        else ui->label_Status_Branddetectie->setStyleSheet("background-color: rgb(170, 0, 0); border-radius: 45px;");
    });

    connect(centraalSysteem, &CentraalBesturingssysteemRPIWEMOS::overruleStatusGewijzigd, this, [this](bool actief) {
        if (actief) ui->label_Status_Brandoverruleknop->setStyleSheet("background-color: green; border-radius: 45px;");
        else ui->label_Status_Brandoverruleknop->setStyleSheet("background-color: rgb(170, 0, 0); border-radius: 45px;");
    });

    // --- Actieve waarschuwingen en logs ---
    connect(centraalSysteem, &CentraalBesturingssysteemRPIWEMOS::logBerichtGegenereerd, this, [this](QString bericht) {
        QString tijd = QDateTime::currentDateTime().toString("hh:mm:ss");
        ui->textBrowser_Logboek->append("<b>" + tijd + " - " + bericht + "</b>");
    });

    connect(centraalSysteem, &CentraalBesturingssysteemRPIWEMOS::stuurNetwerkCommando, socketComm, &SocketCommunicatieRPIWEMOS::verzendData);

    uiTimer = new QTimer(this);
    connect(uiTimer, &QTimer::timeout, this, &MainWindow::updateScherm);
    uiTimer->start(Config::UI_TIMER_INTERVAL);
}

MainWindow::~MainWindow()
{
    uiTimer->stop();
    delete ui;
}

void MainWindow::on_pushButton_Brandoverruleknop_clicked()
{
    centraalSysteem->activeerBrandOverrule();
}

void MainWindow::beheerWaarschuwing(const QString &msg, bool actief)
{
    QList<QListWidgetItem*> items = ui->listWidget_Actief->findItems(msg, Qt::MatchExactly);

    if (actief && items.isEmpty()) {
        ui->listWidget_Actief->addItem(msg);
    } else if (!actief && !items.isEmpty()) {
        for (QListWidgetItem* item : items) {
            delete ui->listWidget_Actief->takeItem(ui->listWidget_Actief->row(item));
        }
    }
}

void MainWindow::updateScherm()
{
    if (socketComm->checkConnectieStatus()) {
        ui->label_Status_Socketverbinding->setStyleSheet("background-color: green; border-radius: 45px;");
    } else {
        ui->label_Status_Socketverbinding->setStyleSheet("background-color: rgb(170, 0, 0); border-radius: 45px;");
    }

    QString inkomend = socketComm->ontvangData();

    if (!inkomend.isEmpty()) {
        QStringList berichten = inkomend.split('\n', Qt::SkipEmptyParts);

        for (const QString& ruwBericht : berichten) {
            QString schoonBericht = ruwBericht.trimmed();
            if (schoonBericht.isEmpty()) continue;

            QString tijd = QDateTime::currentDateTime().toString("hh:mm:ss");
            ui->textBrowser_Logboek->append(tijd + " - Inkomend: " + schoonBericht);

            QJsonParseError parseFout;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(schoonBericht.toUtf8(), &parseFout);

            if (parseFout.error == QJsonParseError::NoError) {
                QJsonObject jsonObj = jsonDoc.object();

                // STATUS UPDATES
                if (jsonObj["type"].toString() == "status") {
                    centraalSysteem->verwerkInkomendeStatus(jsonObj);

                    bool brand = jsonObj["brandAlarm"].toBool();
                    bool overrule = jsonObj["overrule"].toBool();

                    beheerWaarschuwing("🔥 NOODGEVAL: BRAND gedetecteerd! -> Actie: Ventilator geforceerd UIT", brand);
                    beheerWaarschuwing("⚠️ Brandalarm Handmatig Genegeerd -> Actie: Ventilator vrijgegeven", overrule && !brand);
                }

                // SENSOR DATA
                else if (jsonObj["type"].toString() == "sensor") {
                    QString id = jsonObj["sensorId"].toString();

                    double waarde = jsonObj["waarde"].toDouble();
                    double secondenGeleden = timerSindsStart.elapsed() / 1000.0;
                    double windowSize = 60.0;

                    if (id == "CO2") {
                        ui->lcdNumber_CO2->display(waarde);
                        seriesCO2->append(secondenGeleden, waarde);

                        if (secondenGeleden > windowSize) asX_CO2->setRange(secondenGeleden - windowSize, secondenGeleden);
                        if (waarde > asY_CO2->max()) asY_CO2->setMax(waarde + 200);
                        if (waarde < asY_CO2->min()) asY_CO2->setMin(qMax(0.0, waarde - 100));

                        QString msg = "⚠️ CO2-niveau te hoog (> " + QString::number(Config::CO2_WAARSCHUWING) + " PPM) -> Actie: Ventilator AAN";
                        beheerWaarschuwing(msg, waarde > Config::CO2_WAARSCHUWING && !jsonObj["brandAlarm"].toBool());
                    }
                    else if (id == "TEMP") {
                        ui->lcdNumber_Temperatuur->display(waarde);
                        seriesTemp->append(secondenGeleden, waarde);

                        if (secondenGeleden > windowSize) asX_Temp->setRange(secondenGeleden - windowSize, secondenGeleden);
                        if (waarde > asY_Temp->max()) asY_Temp->setMax(waarde + 5);
                        if (waarde < asY_Temp->min()) asY_Temp->setMin(waarde - 5);

                        QString msg = "⚠️ Temperatuur te hoog (> " + QString::number(Config::TEMP_WAARSCHUWING) + "°C) -> Actie: Ventilator AAN";
                        beheerWaarschuwing(msg, waarde > Config::TEMP_WAARSCHUWING && !jsonObj["brandAlarm"].toBool());
                    }
                    else if (id == "HUM") {
                        ui->lcdNumber_Luchtvochtigheid->display(waarde);
                        seriesLucht->append(secondenGeleden, waarde);

                        if (secondenGeleden > windowSize) asX_Lucht->setRange(secondenGeleden - windowSize, secondenGeleden);
                        if (waarde > asY_Lucht->max()) asY_Lucht->setMax(qMin(100.0, waarde + 10));

                        QString msg = "⚠️ Luchtvochtigheid te hoog (> " + QString::number(Config::HUM_WAARSCHUWING) + "%) -> Actie: Ventilator AAN";
                        beheerWaarschuwing(msg, waarde > Config::HUM_WAARSCHUWING && !jsonObj["brandAlarm"].toBool());
                    }
                }
            } else {
                qDebug() << "JSON Fout:" << parseFout.errorString() << "in bericht:" << schoonBericht;
            }
        }
    }
}

void MainWindow::on_pushButton_RGB_Set_clicked()
{
    QString kleurNaam = ui->comboBox_RGB_Kleur->currentText();
    QString kleurWaarde;
    QColor labelKleur;

    if      (kleurNaam == "Wit")   { kleurWaarde = Config::RGB_WIT;   labelKleur = QColor(255, 255, 255); }
    else if (kleurNaam == "Warm")  { kleurWaarde = Config::RGB_WARM;  labelKleur = QColor(255, 120, 20);  }
    else if (kleurNaam == "Rood")  { kleurWaarde = Config::RGB_ROOD;  labelKleur = QColor(255, 0, 0);     }
    else if (kleurNaam == "Blauw") { kleurWaarde = Config::RGB_BLAUW; labelKleur = QColor(0, 0, 255);     }
    else if (kleurNaam == "Groen") { kleurWaarde = Config::RGB_GROEN; labelKleur = QColor(0, 255, 0);     }

    mqttClient->publish(QMqttTopicName("sensor/rgb/set"), kleurWaarde.toUtf8());

    ui->label_Status_Verlichting_RGB->setStyleSheet(
        QString("background-color: rgb(%1,%2,%3); border-radius: 45px;")
            .arg(labelKleur.red()).arg(labelKleur.green()).arg(labelKleur.blue()));

    QString tijd = QDateTime::currentDateTime().toString("hh:mm:ss");
    ui->textBrowser_Logboek->append(tijd + " - Sfeerlicht ingesteld op: " + kleurNaam);
}

void MainWindow::on_pushButton_RGB_Uit_clicked()
{
    mqttClient->publish(QMqttTopicName("sensor/rgb/set"), "UIT");

    ui->label_Status_Verlichting_RGB->setStyleSheet("background-color: rgb(170, 0, 0); border-radius: 45px;");
    ui->label_Status_Verlichting_LED->setStyleSheet("background-color: rgb(170, 0, 0); border-radius: 45px;");

    QString tijd = QDateTime::currentDateTime().toString("hh:mm:ss");
    ui->textBrowser_Logboek->append(tijd + " - Sfeerlicht uitgeschakeld.");
}

void MainWindow::on_pushButton_Reset_Tafel_clicked()
{
    int tafelId = ui->spinBox_Tafel_ID->value();
    mqttClient->publish(QMqttTopicName("tafel/" + QString::number(tafelId) + "/reset"), "RESET");

    QString tijd = QDateTime::currentDateTime().toString("hh:mm:ss");
    ui->textBrowser_Logboek->append(tijd + " - Reset gestuurd naar tafel " + QString::number(tafelId) + ".");
    // label_Status_Drukknop_1 wordt bijgewerkt via de inkomende MQTT "OK" van de Wemos
}

void MainWindow::on_pushButton_Stuur_Menu_clicked()
{
    QString nieuweTekst = ui->lineEdit_Lichtkrant_Nieuw->text();
    mqttClient->publish(QMqttTopicName("wemos/lichtkrant"), ("MENU:" + nieuweTekst).toUtf8());

    QString tijd = QDateTime::currentDateTime().toString("hh:mm:ss");
    ui->textBrowser_Logboek->append(tijd + " - Menu loop ingesteld op: " + nieuweTekst);
    ui->label_Lichtkrant_Huidig->setText(nieuweTekst);
    ui->lineEdit_Lichtkrant_Nieuw->clear();
}

void MainWindow::on_pushButton_Update_Lichtkrant_clicked()
{
    QString nieuweTekst = ui->lineEdit_Lichtkrant_Nieuw->text();

    // Stuur naar Wemos lichtkrant via MQTT
    mqttClient->publish(QMqttTopicName("wemos/lichtkrant"), ("MSG:" + nieuweTekst).toUtf8());

    // Stuur ook naar RPI-BUS via TCP (voor toekomstig gebruik)
    socketComm->verzendData("LICHTKRANT:" + nieuweTekst + "\n");

    QString tijd = QDateTime::currentDateTime().toString("hh:mm:ss");
    ui->textBrowser_Logboek->append(tijd + " - Lichtkrant ingesteld op: " + nieuweTekst);
    ui->label_Lichtkrant_Huidig->setText(nieuweTekst);
    ui->lineEdit_Lichtkrant_Nieuw->clear();
}

void MainWindow::setupGrafieken()
{
    QPen penWaarschuwing(QColor(255, 170, 0));
    penWaarschuwing.setWidth(2);
    penWaarschuwing.setStyle(Qt::DashLine);

    QPen penBrand(Qt::red);
    penBrand.setWidth(2);
    penBrand.setStyle(Qt::DashLine);

    int startTijdvenster = 60;

    // --- TEMPERATUUR ---
    seriesTemp = new QLineSeries();
    seriesWaarschuwingTemp = new QLineSeries();
    seriesWaarschuwingTemp->setPen(penWaarschuwing);
    seriesWaarschuwingTemp->append(0, Config::TEMP_WAARSCHUWING);
    seriesWaarschuwingTemp->append(999999, Config::TEMP_WAARSCHUWING);

    seriesBrandTemp = new QLineSeries();
    seriesBrandTemp->setPen(penBrand);
    seriesBrandTemp->append(0, Config::TEMP_BRAND);
    seriesBrandTemp->append(999999, Config::TEMP_BRAND);

    chartTemp = new QChart();
    chartTemp->addSeries(seriesTemp);
    chartTemp->addSeries(seriesWaarschuwingTemp);
    chartTemp->addSeries(seriesBrandTemp);
    chartTemp->legend()->hide();
    chartTemp->setTitle("Temperatuur (°C)");

    asX_Temp = new QValueAxis();
    asX_Temp->setRange(0, startTijdvenster);
    asY_Temp = new QValueAxis();
    asY_Temp->setRange(10, Config::TEMP_BRAND + 5);

    chartTemp->addAxis(asX_Temp, Qt::AlignBottom);
    chartTemp->addAxis(asY_Temp, Qt::AlignLeft);

    seriesTemp->attachAxis(asX_Temp);
    seriesTemp->attachAxis(asY_Temp);
    seriesWaarschuwingTemp->attachAxis(asX_Temp);
    seriesWaarschuwingTemp->attachAxis(asY_Temp);
    seriesBrandTemp->attachAxis(asX_Temp);
    seriesBrandTemp->attachAxis(asY_Temp);

    // --- CO2 ---
    seriesCO2 = new QLineSeries();
    seriesWaarschuwingCO2 = new QLineSeries();
    seriesWaarschuwingCO2->setPen(penWaarschuwing);
    seriesWaarschuwingCO2->append(0, Config::CO2_WAARSCHUWING);
    seriesWaarschuwingCO2->append(999999, Config::CO2_WAARSCHUWING);

    seriesBrandCO2 = new QLineSeries();
    seriesBrandCO2->setPen(penBrand);
    seriesBrandCO2->append(0, Config::CO2_BRAND);
    seriesBrandCO2->append(999999, Config::CO2_BRAND);

    chartCO2 = new QChart();
    chartCO2->addSeries(seriesCO2);
    chartCO2->addSeries(seriesWaarschuwingCO2);
    chartCO2->addSeries(seriesBrandCO2);
    chartCO2->legend()->hide();
    chartCO2->setTitle("CO2 (PPM)");

    asX_CO2 = new QValueAxis();
    asX_CO2->setRange(0, startTijdvenster);
    asY_CO2 = new QValueAxis();
    asY_CO2->setRange(300, Config::CO2_BRAND + 100);

    chartCO2->addAxis(asX_CO2, Qt::AlignBottom);
    chartCO2->addAxis(asY_CO2, Qt::AlignLeft);

    seriesCO2->attachAxis(asX_CO2);
    seriesCO2->attachAxis(asY_CO2);
    seriesWaarschuwingCO2->attachAxis(asX_CO2);
    seriesWaarschuwingCO2->attachAxis(asY_CO2);
    seriesBrandCO2->attachAxis(asX_CO2);
    seriesBrandCO2->attachAxis(asY_CO2);

    // --- LUCHTVOCHTIGHEID ---
    seriesLucht = new QLineSeries();
    seriesWaarschuwingLucht = new QLineSeries();
    seriesWaarschuwingLucht->setPen(penWaarschuwing);
    seriesWaarschuwingLucht->append(0, Config::HUM_WAARSCHUWING);
    seriesWaarschuwingLucht->append(999999, Config::HUM_WAARSCHUWING);

    chartLucht = new QChart();
    chartLucht->addSeries(seriesLucht);
    chartLucht->addSeries(seriesWaarschuwingLucht);
    chartLucht->legend()->hide();
    chartLucht->setTitle("Luchtvochtigheid (%)");

    asX_Lucht = new QValueAxis();
    asX_Lucht->setRange(0, startTijdvenster);
    asY_Lucht = new QValueAxis();
    asY_Lucht->setRange(0, 100);

    chartLucht->addAxis(asX_Lucht, Qt::AlignBottom);
    chartLucht->addAxis(asY_Lucht, Qt::AlignLeft);

    seriesLucht->attachAxis(asX_Lucht);
    seriesLucht->attachAxis(asY_Lucht);
    seriesWaarschuwingLucht->attachAxis(asX_Lucht);
    seriesWaarschuwingLucht->attachAxis(asY_Lucht);

    chartViewTemp = new QChartView(chartTemp, ui->centralwidget);
    chartViewTemp->setGeometry(ui->graphicsView_Temperatuur->geometry());
    chartViewTemp->show();
    ui->graphicsView_Temperatuur->hide();

    chartViewCO2 = new QChartView(chartCO2, ui->centralwidget);
    chartViewCO2->setGeometry(ui->graphicsView_CO2->geometry());
    chartViewCO2->show();
    ui->graphicsView_CO2->hide();

    chartViewLucht = new QChartView(chartLucht, ui->centralwidget);
    chartViewLucht->setGeometry(ui->graphicsView_Luchtvochtigheid->geometry());
    chartViewLucht->show();
    ui->graphicsView_Luchtvochtigheid->hide();
}