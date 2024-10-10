#ifndef TRACKRECORD_H
#define TRACKRECORD_H

#include <QSet>


template <typename T>
class TrackRecord
{
public:

    void AddRecord(T &item);
    void DelRecord(T &item);
    // void setResult();

    QSet<T>& getListAdd() { return listAdd; }
    QSet<T>& getListDel() { return listDel; }
private:
    QSet<T> listAdd;
    QSet<T> listDel;
};







template<typename T>
inline void TrackRecord<T>::AddRecord(T &item)
{
    if(listDel.contains(item))
        listDel.remove(item);
    else
        listAdd.insert(item);
}

template<typename T>
inline void TrackRecord<T>::DelRecord(T &item)
{
    if(listAdd.contains(item))
        listAdd.remove(item);
    else
        listDel.insert(item);
}

// template<typename T>
// inline void TrackRecord<T>::setResult()
// {
//     QSet<T> res = listAdd;
//     res.intersect(listDel);
//     listAdd.subtract(res);
//     listDel.subtract(res);
// }

#endif // TRACKRECORD_H
