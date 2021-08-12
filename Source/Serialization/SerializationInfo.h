#pragma once

#include <map>
#include <string>

namespace tg
{

class SerializationInfo final
{
/**@section Method */
public:
    template <typename _Value>
    void AddValue(const std::string& name, const _Value& value);
    
/**@section Variable */
private:
    std::map<std::string, std::string> m_variableTable;
};

template <typename _Value>
void SerializationInfo::AddValue(const std::string& name, const _Value& value)
{
    m_variableTable.insert(name, value);
}

}
