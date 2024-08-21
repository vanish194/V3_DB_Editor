// Storage.cpp
#include "Storage.h"

// Методы для добавления данных
void Storage::addTool(const Tool &tool)
{
    tools.insert(tool);
}

void Storage::addProducer(const Producer &producer)
{
    producers.insert(producer);
}

void Storage::addToolDescription(const ToolDescription &toolDescription)
{
    toolDescriptions.insert(toolDescription);
}

void Storage::addToolSensor(const ToolSensor &toolSensor)
{
    toolSensors.insert(toolSensor);
}

void Storage::addSensor(const Sensor &sensor)
{
    sensors.insert(sensor);
}

void Storage::addMethod(const Method &method)
{
    methods.insert(method);
}

void Storage::addMainMnemonic(const MainMnemonic &mainMnemonic)
{
    mainMnemonics.insert(mainMnemonic);
}

void Storage::addAdditionalMnemonic(const AdditionalMnemonic &additionalMnemonic)
{
    additionalMnemonics.insert(additionalMnemonic);
}

void Storage::addUnit(const Unit &unit)
{
    units.insert(unit);
}

void Storage::addCompany(const Company &company)
{
    companies.insert(company);
}

void Storage::addConversionFormula(const ConversionFormula &conversionFormula)
{
    conversionFormulas.insert(conversionFormula);
}

// Методы для получения данных
const std::set<Tool> &Storage::getTools() const
{
    return tools;
}

const std::set<Producer> &Storage::getProducers() const
{
    return producers;
}

const std::set<ToolDescription> &Storage::getToolDescriptions() const
{
    return toolDescriptions;
}

const std::set<ToolSensor> &Storage::getToolSensors() const
{
    return toolSensors;
}

const std::set<Sensor> &Storage::getSensors() const
{
    return sensors;
}

const std::set<Method> &Storage::getMethods() const
{
    return methods;
}

const std::set<MainMnemonic> &Storage::getMainMnemonics() const
{
    return mainMnemonics;
}

const std::set<AdditionalMnemonic> &Storage::getAdditionalMnemonics() const
{
    return additionalMnemonics;
}

const std::set<Unit> &Storage::getUnits() const
{
    return units;
}

const std::set<Company> &Storage::getCompanies() const
{
    return companies;
}

const std::set<ConversionFormula> &Storage::getConversionFormulas() const
{
    return conversionFormulas;
}
