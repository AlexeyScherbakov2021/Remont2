#ifndef LISTMODUL_H
#define LISTMODUL_H

#include "listdevice.h"
#include "modul.h"


class ListModul : public ListDevice<Modul>
{
public:
    explicit ListModul();

    // ListDevice interface
public:
    void Load() override;
    QVariant getData(int row, int col) const override;

    bool LinkPlate(int idPlate, int idModul);
    void LoadChild(Modul &modul) override;
    Modul GetItem(int id) override;
};

#endif // LISTMODUL_H
