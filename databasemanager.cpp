// DatabaseManager.cpp
#include "DatabaseManager.h"
#include <QSqlError>

DatabaseManager::DatabaseManager()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
}

DatabaseManager::~DatabaseManager()
{
    if (db.isOpen()) {
        db.close();
    }
}

bool DatabaseManager::openDatabase(const QString &dbPath)
{
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << "Error: connection with database failed -" << db.lastError().text();
        return false;
    }

    qDebug() << "Database: connection ok";

    // Check the database version
    int currentVersion = getDatabaseVersion();
    int expectedVersion = 1; // Set this to the version your application expects

    if (currentVersion < 0) {
        qDebug() << "Error: Unable to determine database version.";
        return false;
    }

    if (currentVersion != expectedVersion) {
        qDebug() << "Error: Database version mismatch. Expected version" << expectedVersion
                 << "but found version" << currentVersion;
        return false;
    }

    return true;
}

void DatabaseManager::loadData(Storage &storage)
{
    QString sqlQuery = R"(
    SELECT
        tools.tool_id,
        tools.tool_name,
        tools.tool_description_id,
        producers.producer_id,
        producers.producer_name,
        tool_descriptions.tool_description_id,
        tool_descriptions.description,
        tool_descriptions.length,
        tool_descriptions.outer_diameter,
        tool_descriptions.inner_diameter,
        tool_descriptions.image,
        tools_sensors.tool_sensor_id,
        tools_sensors.offset,
        tools_sensors.sensor_id,
        sensors.sensor_id,
        sensors.sensor_name,
        sensors.sensor_description,
        sensors.method_id,
        methods.method_id,
        methods.method_name,
        main_mnemonics.main_mnemonic_id,
        main_mnemonics.main_mnemonic_name,
        main_mnemonics.main_mnemonic_description,
        main_mnemonics.sensor_id,
        units_for_main.unit_id AS main_unit_id,
        units_for_main.unit_name AS main_unit_name,
        additional_mnemonics.additional_mnemonic_id,
        additional_mnemonics.additional_mnemonic_name,
        additional_mnemonics.main_mnemonic_id,
        additional_mnemonics.unit_id AS additional_unit_id,
        units_for_additional.unit_name AS additional_unit_name,
        additional_mnemonics.company_id,
        companies.company_id,
        companies.company_name,
        conversion_formulas.formula_id,
        conversion_formulas.formula,
        conversion_formulas.initial_unit_id,
        conversion_formulas.derived_unit_id
    FROM
        tools
        LEFT JOIN tools_sensors ON tools.tool_id = tools_sensors.tool_id
        LEFT JOIN sensors ON tools_sensors.sensor_id = sensors.sensor_id
        LEFT JOIN methods ON sensors.method_id = methods.method_id
        LEFT JOIN main_mnemonics ON sensors.sensor_id = main_mnemonics.sensor_id
        LEFT JOIN units AS units_for_main ON main_mnemonics.main_mnemonic_id = units_for_main.unit_id
        LEFT JOIN additional_mnemonics ON main_mnemonics.main_mnemonic_id = additional_mnemonics.main_mnemonic_id
        LEFT JOIN units AS units_for_additional ON additional_mnemonics.unit_id = units_for_additional.unit_id
        LEFT JOIN companies ON additional_mnemonics.company_id = companies.company_id
        LEFT JOIN tool_descriptions ON tools.tool_description_id = tool_descriptions.tool_description_id
        LEFT JOIN producers ON tool_descriptions.producer_id = producers.producer_id
        LEFT JOIN conversion_formulas ON units_for_additional.unit_id = conversion_formulas.derived_unit_id
        AND units_for_main.unit_id = conversion_formulas.initial_unit_id
    )";

    QSqlQuery query = executeQuery(sqlQuery);
    while (query.next()) {
        // Tool
        if (!query.isNull("tool_id")) {
            Tool tool;
            tool.tool_id = query.value("tool_id").toInt();
            tool.tool_name = query.value("tool_name").toString();
            tool.tool_description_id = query.value("tool_description_id").toInt();
            storage.addTool(tool);
        }

        // Producer
        if (!query.isNull("producer_id")) {
            Producer producer;
            producer.producer_id = query.value("producer_id").toInt();
            producer.producer_name = query.value("producer_name").toString();
            storage.addProducer(producer);
        }

        // ToolDescription
        if (!query.isNull("tool_description_id")) {
            ToolDescription toolDescription;
            toolDescription.tool_description_id = query.value("tool_description_id").toInt();
            toolDescription.description = query.value("description").toString();
            toolDescription.length = query.value("length").toString();
            toolDescription.outer_diameter = query.value("outer_diameter").toString();
            toolDescription.inner_diameter = query.value("inner_diameter").toString();
            toolDescription.image = query.value("image").toByteArray();
            storage.addToolDescription(toolDescription);
        }

        // ToolSensor
        if (!query.isNull("tool_sensor_id")) {
            ToolSensor toolSensor;
            toolSensor.tool_sensor_id = query.value("tool_sensor_id").toInt();
            toolSensor.tool_id = query.value("tool_id").toInt();
            toolSensor.sensor_id = query.value("sensor_id").toInt();
            toolSensor.offset = query.value("offset").toString();
            storage.addToolSensor(toolSensor);
        }

        // Sensor
        if (!query.isNull("sensor_id")) {
            Sensor sensor;
            sensor.sensor_id = query.value("sensor_id").toInt();
            sensor.sensor_name = query.value("sensor_name").toString();
            sensor.sensor_description = query.value("sensor_description").toString();
            sensor.method_id = query.value("method_id").toInt();
            storage.addSensor(sensor);
        }

        // Method
        if (!query.isNull("method_id")) {
            Method method;
            method.method_id = query.value("method_id").toInt();
            method.method_name = query.value("method_name").toString();
            storage.addMethod(method);
        }

        // MainMnemonic
        if (!query.isNull("main_mnemonic_id")) {
            MainMnemonic mainMnemonic;
            mainMnemonic.main_mnemonic_id = query.value("main_mnemonic_id").toInt();
            mainMnemonic.main_mnemonic_name = query.value("main_mnemonic_name").toString();
            mainMnemonic.main_mnemonic_description = query.value("main_mnemonic_description")
                                                         .toString();
            mainMnemonic.sensor_id = query.value("sensor_id").toInt();
            mainMnemonic.unit_id = query.value("main_unit_id").toInt();
            storage.addMainMnemonic(mainMnemonic);
        }

        // AdditionalMnemonic
        if (!query.isNull("additional_mnemonic_id")) {
            AdditionalMnemonic additionalMnemonic;
            additionalMnemonic.additional_mnemonic_id = query.value("additional_mnemonic_id").toInt();
            additionalMnemonic.additional_mnemonic_name = query.value("additional_mnemonic_name")
                                                              .toString();
            additionalMnemonic.main_mnemonic_id = query.value("main_mnemonic_id").toInt();
            additionalMnemonic.unit_id = query.value("additional_unit_id").toInt();
            additionalMnemonic.company_id = query.value("company_id").toInt();
            storage.addAdditionalMnemonic(additionalMnemonic);
        }

        // Unit
        if (!query.isNull("main_unit_id")) {
            Unit unit;
            unit.unit_id = query.value("main_unit_id").toInt();
            unit.unit_name = query.value("main_unit_name").toString();
            storage.addUnit(unit);
        }

        // Company
        if (!query.isNull("company_id")) {
            Company company;
            company.company_id = query.value("company_id").toInt();
            company.company_name = query.value("company_name").toString();
            storage.addCompany(company);
        }

        // ConversionFormula
        if (!query.isNull("formula_id")) {
            ConversionFormula conversionFormula;
            conversionFormula.formula_id = query.value("formula_id").toInt();
            conversionFormula.initial_unit_id = query.value("initial_unit_id").toInt();
            conversionFormula.derived_unit_id = query.value("derived_unit_id").toInt();
            conversionFormula.formula = query.value("formula").toString();
            storage.addConversionFormula(conversionFormula);
        }
    }
}

QSqlQuery DatabaseManager::executeQuery(const QString &queryStr)
{
    QSqlQuery query;
    if (!query.exec(queryStr)) {
        qDebug() << "Error: failed to execute query -" << query.lastError().text();
    }
    return query;
}
int DatabaseManager::getDatabaseVersion() const
{
    int version = -1; // Default to an invalid version

    QSqlQuery query("SELECT version FROM db_information ORDER BY version DESC LIMIT 1");

    if (query.exec()) {
        if (query.next()) {
            version = query.value(0).toInt();
        }
    } else {
        qDebug() << "Error retrieving database version:" << query.lastError().text();
    }

    return version;
}
