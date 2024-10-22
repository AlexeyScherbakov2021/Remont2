#ifndef TRACKRECORD_H
#define TRACKRECORD_H

#include <QMap>
// #include <QSet>


template <typename T>
class TrackRecord
{
public:

    // void AddRecord(T &item);
    // void DelRecord(T &item);
    // QSet<T>& getListAdd() { return listAdd; }
    // QSet<T>& getListDel() { return listDel; }

    bool AddRecord(int key, const T &item);
    void DelRecord(int key, const T &item);

    void getListAdd(QList<T> &list);
    void getListDel(QList<T> &list);

// private:

    // QSet<T> listAdd;
    // QSet<T> listDel;
    QMap<int, T> listAdd;
    QMap<int, T> listDel;
};




// template<typename T>
// inline void TrackRecord<T>::AddRecord(T &item)
// {
//     if(listDel.contains(item))
//         listDel.remove(item);
//     else
//         listAdd.insert(item);
// }

// template<typename T>
// inline void TrackRecord<T>::DelRecord(T &item)
// {
//     if(listAdd.contains(item))
//         listAdd.remove(item);
//     else
//         listDel.insert(item);
// }

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


// template<typename T>
// inline void TrackRecord<T>::setResult()
// {
//     QSet<T> res = listAdd;
//     res.intersect(listDel);
//     listAdd.subtract(res);
//     listDel.subtract(res);
// }

#endif // TRACKRECORD_H
