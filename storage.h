// Storage.h
#ifndef STORAGE_H
#define STORAGE_H

#include <QByteArray>
#include <QString>
#include <set>

// Структуры данных для каждой таблицы
struct Tool
{
    int tool_id;
    QString tool_name;
    int tool_description_id;

    bool operator<(const Tool &other) const { return tool_id < other.tool_id; }
};

struct Producer
{
    int producer_id;
    QString producer_name;

    bool operator<(const Producer &other) const { return producer_id < other.producer_id; }
};

struct ToolDescription
{
    int tool_description_id;
    QString description;
    QString length;
    QString outer_diameter;
    QString inner_diameter;
    QByteArray image;

    bool operator<(const ToolDescription &other) const
    {
        return tool_description_id < other.tool_description_id;
    }
};

struct ToolSensor
{
    int tool_sensor_id;
    int tool_id;
    int sensor_id;
    QString offset;

    bool operator<(const ToolSensor &other) const { return tool_sensor_id < other.tool_sensor_id; }
};

struct Sensor
{
    int sensor_id;
    QString sensor_name;
    QString sensor_description;
    int method_id;

    bool operator<(const Sensor &other) const { return sensor_id < other.sensor_id; }
};

struct Method
{
    int method_id;
    QString method_name;

    bool operator<(const Method &other) const { return method_id < other.method_id; }
};

struct MainMnemonic
{
    int main_mnemonic_id;
    QString main_mnemonic_name;
    QString main_mnemonic_description;
    int sensor_id;
    int unit_id;

    bool operator<(const MainMnemonic &other) const
    {
        return main_mnemonic_id < other.main_mnemonic_id;
    }
};

struct AdditionalMnemonic
{
    int additional_mnemonic_id;
    QString additional_mnemonic_name;
    int main_mnemonic_id;
    int unit_id;
    int company_id;

    bool operator<(const AdditionalMnemonic &other) const
    {
        return additional_mnemonic_id < other.additional_mnemonic_id;
    }
};

struct Unit
{
    int unit_id;
    QString unit_name;

    bool operator<(const Unit &other) const { return unit_id < other.unit_id; }
};

struct Company
{
    int company_id;
    QString company_name;

    bool operator<(const Company &other) const { return company_id < other.company_id; }
};

struct ConversionFormula
{
    int formula_id;
    int initial_unit_id;
    int derived_unit_id;
    QString formula;

    bool operator<(const ConversionFormula &other) const { return formula_id < other.formula_id; }
};

class Storage
{
public:
    // Методы для добавления данных
    void addTool(const Tool &tool);
    void addProducer(const Producer &producer);
    void addToolDescription(const ToolDescription &toolDescription);
    void addToolSensor(const ToolSensor &toolSensor);
    void addSensor(const Sensor &sensor);
    void addMethod(const Method &method);
    void addMainMnemonic(const MainMnemonic &mainMnemonic);
    void addAdditionalMnemonic(const AdditionalMnemonic &additionalMnemonic);
    void addUnit(const Unit &unit);
    void addCompany(const Company &company);
    void addConversionFormula(const ConversionFormula &conversionFormula);

    // Методы для получения данных
    const std::set<Tool> &getTools() const;
    const std::set<Producer> &getProducers() const;
    const std::set<ToolDescription> &getToolDescriptions() const;
    const std::set<ToolSensor> &getToolSensors() const;
    const std::set<Sensor> &getSensors() const;
    const std::set<Method> &getMethods() const;
    const std::set<MainMnemonic> &getMainMnemonics() const;
    const std::set<AdditionalMnemonic> &getAdditionalMnemonics() const;
    const std::set<Unit> &getUnits() const;
    const std::set<Company> &getCompanies() const;
    const std::set<ConversionFormula> &getConversionFormulas() const;

private:
    // Контейнеры для хранения данных
    std::set<Tool> tools;
    std::set<Producer> producers;
    std::set<ToolDescription> toolDescriptions;
    std::set<ToolSensor> toolSensors;
    std::set<Sensor> sensors;
    std::set<Method> methods;
    std::set<MainMnemonic> mainMnemonics;
    std::set<AdditionalMnemonic> additionalMnemonics;
    std::set<Unit> units;
    std::set<Company> companies;
    std::set<ConversionFormula> conversionFormulas;
};

#endif // STORAGE_H
