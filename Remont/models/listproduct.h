#ifndef LISTPRODUCT_H
#define LISTPRODUCT_H

#include "listdevice.h"
// #include "product.h"


class ListProduct : public ListDevice
{

        // ListDevice interface
public:

    explicit ListProduct();

    void Load() override;
    QVariant getData(int row, int col, int role) const override;
    void LoadChild(Items &prod) override;
    // Product GetItem(int id) override;

    bool DeleteItem(int id) override;

    // ListDevice interface
public:
    void GetHeader(QStringList &headers) override;

    // ListDevice interface
public:
    void setData(int row, int col, const QVariant value, int role) const override;
};

#endif // LISTPRODUCT_H
