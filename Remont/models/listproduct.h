#ifndef LISTPRODUCT_H
#define LISTPRODUCT_H

#include "listdevice.h"
#include "product.h"


class ListProduct : public ListDevice<Product>
{

        // ListDevice interface
public:

    explicit ListProduct();

    void Load() override;
    QVariant getData(int row, int col) const override;
    void LoadChild(Product &prod) override;
    Product GetItem(int id) override;

    bool DeleteItem(int id) override;
};

#endif // LISTPRODUCT_H
