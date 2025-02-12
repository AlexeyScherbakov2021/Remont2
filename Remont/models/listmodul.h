#ifndef LISTMODUL_H
#define LISTMODUL_H

#include "listdevice.h"
// #include "modul.h"


class ListModul : public ListDevice
{
public:
    explicit ListModul();

    // void Load() override;
    QVariant getData(int row, int col, int role) const override;

    // bool LinkPlate(int idPlate, int idModul);
    // void LoadChild(Items &modul) override;

    // bool DeleteItem(int id) override;

    void GetHeader(QStringList &headers) override;

    void setData(int row, int col, const QVariant value, int role) const override;
};

#endif // LISTMODUL_H
