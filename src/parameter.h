#ifndef PARAMETER_H
#define PARAMETER_H

#include <QString>

typedef enum{
    NAME,
    CROSS_MODEL,
    DISTANCE,
    POINT_MODEL,
    STATUS

} PARAMETER_TYPE;


class Parameter
{
protected:
    PARAMETER_TYPE m_type;

public:
    //! simple constructor
    Parameter(){}
    virtual ~Parameter();
    virtual QString get_parameter_name() = 0;
    virtual PARAMETER_TYPE type() = 0;
};

#endif // PARAMETER_H
