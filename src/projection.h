#ifndef PROJECTION_H
#define PROJECTION_H

#include <QString>
#include <QList>

typedef enum {
    POLYLINE,
    TEXT,
    CIRCLE,
    UNKNOWN

}PROJECTION_TYPE;

class Projection
{
protected:
    PROJECTION_TYPE *m_type;
public:
    Projection();
    virtual ~Projection();
    virtual QString get_projection_name() = 0;
    virtual bool get_activated() = 0;
    virtual QList<QString> get_coordinateSystemList() = 0;
    virtual PROJECTION_TYPE type() = 0;
};

#endif // PROJECTION_H
