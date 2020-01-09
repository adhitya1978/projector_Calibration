#include "parameters.h"

Parameters::Parameters()
{
}
Parameters::~Parameters()
{
}

int Parameters::add(Parameter *parameter)
{
    m_parameters.append(parameter);
    return 1;
}

int Parameters::clear()
{
     m_parameters.clear();
     return 1;
}

int Parameters::count()
{
   return m_parameters.count();
}

Parameter *Parameters::get(int index)
{
    return m_parameters.at(index);
}

int Parameters::get(Parameter *parameter)
{
    int pos = -1;
    Q_FOREACH(Parameter *_temp, m_parameters){
        if(QString::compare(_temp->get_parameter_name().toUpper(), parameter->get_parameter_name().toUpper())== 0){
            pos++;
            break;
        }
        pos++;
    }
    return pos;
}

QList<Parameter *> Parameters::items()
{
    return m_parameters;
}

void Parameters::remove(int pos)
{
    m_parameters.removeAt(pos);
}


