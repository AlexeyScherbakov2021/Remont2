#ifndef TRACKRECORD_H
#define TRACKRECORD_H

#include <QMap>

template <typename T>
class TrackRecord
{
public:

    bool AddRecord(/*int key,*/ const T &item);
    bool AddRecord(int key, const T &item);
    void DelRecord(/*int key,*/ const T &item);
    void DelRecord(int key, const T &item);

    void getListAdd(QList<T> &list);
    void getListDel(QList<T> &list);

    QMap<int, T> listAdd;
    QMap<int, T> listDel;
    void clear();
};


template<typename T>
inline bool TrackRecord<T>::AddRecord(/*int key,*/ const T &item)
{
    if(listDel.contains(item.id))
        listDel.remove(item.id);
    else
    {
        if(listAdd.contains(item.id))
            return false;
        else
            listAdd.insert(item.id, item);
    }

    return true;
}

template<typename T>
inline bool TrackRecord<T>::AddRecord(int key, const T &item)
{
    if(listDel.contains(key))
        listDel.remove(key);
    else
    {
        if(listAdd.contains(key))
            return false;
        else
            listAdd.insert(key, item);
    }

    return true;
}


template<typename T>
inline void TrackRecord<T>::DelRecord(/*int key,*/ const T &item)
{
    if(listAdd.contains(item.id))
        listAdd.remove(item.id);
    else
        listDel.insert(item.id, item);

}

template<typename T>
inline void TrackRecord<T>::DelRecord(int key, const T &item)
{
    if(listAdd.contains(key))
        listAdd.remove(key);
    else
        listDel.insert(key, item);

}


template<typename T>
inline void TrackRecord<T>::getListAdd(QList<T> &list)
{
    list.clear();
    list.reserve(listAdd.size());

    for(auto &it : listAdd)
        list.push_back(it);
}

template<typename T>
inline void TrackRecord<T>::getListDel(QList<T> &list)
{
    list.clear();
    list.reserve(listDel.size());

    for(auto &it : listDel)
        list.push_back(it);

}

template<typename T>
inline void TrackRecord<T>::clear()
{
    listAdd.clear();
    listDel.clear();
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
