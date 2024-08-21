// MainWindow.cpp
#include "MainWindow.h"
#include <QFileDialog>
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , storage(new Storage())
{
    ui->setupUi(this);
    connect(ui->actionOpenDatabase, &QAction::triggered, this, &MainWindow::onOpenDatabase);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete storage;
}

void MainWindow::onOpenDatabase()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Open Database",
                                                    "",
                                                    "SQLite Database (*.db)");
    if (!fileName.isEmpty()) {
        if (dbManager.openDatabase(fileName)) {
            dbManager.loadData(*storage);
            populateTree();
            populateTable();
        } else {
            // Display an error message if the database could not be opened or if the version is incorrect
            QMessageBox::critical(
                this,
                "Database Error",
                "Failed to open database due to a version mismatch or other issues.");
        }
    }
}

void MainWindow::populateTable()
{
    // Create a model for the QTableView
    QStandardItemModel *tableModel = new QStandardItemModel(this);

    // Define the columns for the table
    QStringList headers = {"Tool ID",
                           "Tool Name",
                           "Sensor ID",
                           "Sensor Name",
                           "Main Mnemonic ID",
                           "Main Mnemonic Name",
                           "Additional Mnemonic ID",
                           "Additional Mnemonic Name"};

    tableModel->setHorizontalHeaderLabels(headers);

    // Retrieve data from Storage
    const std::set<Tool> &tools = storage->getTools();
    const std::set<ToolSensor> &toolSensors = storage->getToolSensors();
    const std::set<Sensor> &sensors = storage->getSensors();
    const std::set<MainMnemonic> &mainMnemonics = storage->getMainMnemonics();
    const std::set<AdditionalMnemonic> &additionalMnemonics = storage->getAdditionalMnemonics();

    // Iterate through the tools
    for (const Tool &tool : tools) {
        bool hasSensor = false;
        bool hasMainMnemonic = false;
        bool hasAdditionalMnemonic = false;

        // Check for related sensors
        for (const ToolSensor &toolSensor : toolSensors) {
            if (toolSensor.tool_id == tool.tool_id) {
                hasSensor = true;
                for (const Sensor &sensor : sensors) {
                    if (sensor.sensor_id == toolSensor.sensor_id) {
                        hasMainMnemonic = false; // Reset for each new sensor

                        // Check for related main mnemonics
                        for (const MainMnemonic &mainMnemonic : mainMnemonics) {
                            if (mainMnemonic.sensor_id == sensor.sensor_id) {
                                hasMainMnemonic = true;
                                hasAdditionalMnemonic = false; // Reset for each new mnemonic

                                // Check for related additional mnemonics
                                for (const AdditionalMnemonic &additionalMnemonic :
                                     additionalMnemonics) {
                                    if (additionalMnemonic.main_mnemonic_id
                                        == mainMnemonic.main_mnemonic_id) {
                                        hasAdditionalMnemonic = true;

                                        // Create a fully populated row
                                        QList<QStandardItem *> rowData;
                                        rowData.append(
                                            new QStandardItem(QString::number(tool.tool_id)));
                                        rowData.append(new QStandardItem(tool.tool_name));
                                        rowData.append(
                                            new QStandardItem(QString::number(sensor.sensor_id)));
                                        rowData.append(new QStandardItem(sensor.sensor_name));
                                        rowData.append(new QStandardItem(
                                            QString::number(mainMnemonic.main_mnemonic_id)));
                                        rowData.append(
                                            new QStandardItem(mainMnemonic.main_mnemonic_name));
                                        rowData.append(new QStandardItem(QString::number(
                                            additionalMnemonic.additional_mnemonic_id)));
                                        rowData.append(new QStandardItem(
                                            additionalMnemonic.additional_mnemonic_name));

                                        tableModel->appendRow(rowData);
                                    }
                                }

                                // If no additional mnemonic was found, create a row without it
                                if (!hasAdditionalMnemonic) {
                                    QList<QStandardItem *> rowData;
                                    rowData.append(new QStandardItem(QString::number(tool.tool_id)));
                                    rowData.append(new QStandardItem(tool.tool_name));
                                    rowData.append(
                                        new QStandardItem(QString::number(sensor.sensor_id)));
                                    rowData.append(new QStandardItem(sensor.sensor_name));
                                    rowData.append(new QStandardItem(
                                        QString::number(mainMnemonic.main_mnemonic_id)));
                                    rowData.append(
                                        new QStandardItem(mainMnemonic.main_mnemonic_name));
                                    rowData.append(new QStandardItem(
                                        "N/A")); // Placeholder for missing additional mnemonic
                                    rowData.append(new QStandardItem("N/A"));

                                    tableModel->appendRow(rowData);
                                }
                            }
                        }

                        // If no main mnemonic was found, create a row without it
                        if (!hasMainMnemonic) {
                            QList<QStandardItem *> rowData;
                            rowData.append(new QStandardItem(QString::number(tool.tool_id)));
                            rowData.append(new QStandardItem(tool.tool_name));
                            rowData.append(new QStandardItem(QString::number(sensor.sensor_id)));
                            rowData.append(new QStandardItem(sensor.sensor_name));
                            rowData.append(
                                new QStandardItem("N/A")); // Placeholder for missing main mnemonic
                            rowData.append(new QStandardItem("N/A"));
                            rowData.append(new QStandardItem("N/A"));
                            rowData.append(new QStandardItem("N/A"));

                            tableModel->appendRow(rowData);
                        }
                    }
                }
            }
        }

        // If no sensor was found, create a row with just the tool data
        if (!hasSensor) {
            QList<QStandardItem *> rowData;
            rowData.append(new QStandardItem(QString::number(tool.tool_id)));
            rowData.append(new QStandardItem(tool.tool_name));
            rowData.append(new QStandardItem("N/A")); // Placeholder for missing sensor
            rowData.append(new QStandardItem("N/A"));
            rowData.append(new QStandardItem("N/A"));
            rowData.append(new QStandardItem("N/A"));
            rowData.append(new QStandardItem("N/A"));
            rowData.append(new QStandardItem("N/A"));

            tableModel->appendRow(rowData);
        }
    }

    // Set the model to the QTableView
    ui->tableView->setModel(tableModel);

    // Optional: Resize the columns to fit the contents
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::populateTree()
{
    QStandardItemModel *treeModel = new QStandardItemModel(ui->treeView);
    treeModel->setHorizontalHeaderLabels(QStringList() << "Name"
                                                       << "Details");

    const std::set<Tool> &tools = storage->getTools();
    const std::set<ToolSensor> &toolSensors = storage->getToolSensors();
    const std::set<Sensor> &sensors = storage->getSensors();
    const std::set<MainMnemonic> &mainMnemonics = storage->getMainMnemonics();
    const std::set<AdditionalMnemonic> &additionalMnemonics = storage->getAdditionalMnemonics();

    for (const Tool &tool : tools) {
        QStandardItem *toolItem = new QStandardItem(tool.tool_name);
        QList<QStandardItem *> toolRow = {toolItem,
                                          new QStandardItem(
                                              QString("Tool ID: %1").arg(tool.tool_id))};
        treeModel->appendRow(toolRow);

        for (const ToolSensor &toolSensor : toolSensors) {
            if (toolSensor.tool_id == tool.tool_id) {
                for (const Sensor &sensor : sensors) {
                    if (sensor.sensor_id == toolSensor.sensor_id) {
                        QStandardItem *sensorItem = new QStandardItem(sensor.sensor_name);
                        QList<QStandardItem *> sensorRow
                            = {sensorItem,
                               new QStandardItem(QString("Sensor ID: %1").arg(sensor.sensor_id))};
                        toolItem->appendRow(sensorRow);

                        for (const MainMnemonic &mainMnemonic : mainMnemonics) {
                            if (mainMnemonic.sensor_id == sensor.sensor_id) {
                                QStandardItem *mnemonicItem = new QStandardItem(
                                    mainMnemonic.main_mnemonic_name);
                                QList<QStandardItem *> mnemonicRow
                                    = {mnemonicItem,
                                       new QStandardItem(QString("Mnemonic ID: %1")
                                                             .arg(mainMnemonic.main_mnemonic_id))};
                                sensorItem->appendRow(mnemonicRow);

                                for (const AdditionalMnemonic &additionalMnemonic :
                                     additionalMnemonics) {
                                    if (additionalMnemonic.main_mnemonic_id
                                            == mainMnemonic.main_mnemonic_id
                                        && additionalMnemonic.additional_mnemonic_id != 0) {
                                        QStandardItem *additionalItem = new QStandardItem(
                                            additionalMnemonic.additional_mnemonic_name);
                                        QList<QStandardItem *> additionalRow = {
                                            additionalItem,
                                            new QStandardItem(
                                                QString("Additional Mnemonic ID: %1, Unit ID: %2")
                                                    .arg(additionalMnemonic.additional_mnemonic_id)
                                                    .arg(additionalMnemonic.unit_id))};
                                        mnemonicItem->appendRow(additionalRow);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    ui->treeView->setModel(treeModel);
    ui->treeView->expandAll();
}
