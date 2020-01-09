#ifndef PROJECTIONS_H
#define PROJECTIONS_H

#include <QList>
#include <QString>

#include "projection.h"

class Projections
{
protected:
    //! list of projection in group
    QList<Projection*> m_list;
    QString m_groupname;
public:
    Projections();
    ~Projections();

    //! add to projection
    int add(Projection * projection);
    //! change or remove projection
    int remove(int index);
    int clear();
    //! count
    int count();
    //! get projection by index
    Projection *get(int index);
    //! get projection by name
    int get(Projection *projection);

    QList<Projection*> items();


};

#endif // PROJECTIONS_H
