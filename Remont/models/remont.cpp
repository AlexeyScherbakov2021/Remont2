#include "remont.h"


void Remont::AddRemont(int idClaim, Items* item, int idPrevReason, QDateTime dateCreate)
{
    RepoMSSQL repo;

    // Добавление в ремонт
    Remont remont;
    remont.idClaim = idClaim;
    remont.idItem = item->id;
    remont.startDate = dateCreate;
    remont.idPrevReason = idPrevReason;
    repo.AddRemont(remont);

    // Добавление в ремонт изделия, если есть
    Items parent = repo.GetItem(item->idParent);
    while(parent.id > 0)
    {
        Remont remontParent = repo.GetRemontForItem(parent.id, idClaim);
        if(remontParent.id == 0)
        {
            // создаем в ремонте, если не было
            remontParent.idClaim = idClaim;
            remontParent.idItem = parent.id;
            remontParent.startDate = dateCreate;
            repo.AddRemont(remontParent);
        }
        parent = repo.GetItem(parent.idParent);
    }
}
