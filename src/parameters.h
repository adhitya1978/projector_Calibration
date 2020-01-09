#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <QList>
#include "parameter.h"

class Parameters
{
protected:
    QList<Parameter*> m_parameters;
public:
    Parameters();
    ~Parameters();


    int add(Parameter *parameter);
    int clear();
    int count();
    Parameter *get(int index);
    int get(Parameter *parameter);
    QList<Parameter*> items();
    void remove(int pos);

};

#endif // PARAMETERS_H
