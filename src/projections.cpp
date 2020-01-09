#include "projections.h"
#include <QDebug>

Projections::Projections()
{
    //todo add new projection
}

Projections::~Projections()
{
}

int Projections::add(Projection *projection)
{
    this->m_list.append(projection);
    return 1;
}

int Projections::remove(int index)
{
    m_list.removeAt(index);
    return 1;
}

int Projections::clear()
{
    m_list.clear();
}

int Projections::count()
{
    return m_list.count();
}

Projection *Projections::get(int index)
{
    return m_list.at(index);
}

int Projections::get(Projection *projection)
{
    int pos = -1;
    Q_FOREACH(Projection *_temp, m_list){
        if(QString::compare(_temp->get_projection_name().toUpper(), projection->get_projection_name().toUpper())== 0){
            pos++;
            break;
        }
        pos++;
    }
    return pos;
}

QList<Projection *> Projections::items()
{
    return m_list;
}
