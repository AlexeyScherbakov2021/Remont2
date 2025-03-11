#ifndef DATEEDIT_H
#define DATEEDIT_H

#include <QDateEdit>
#include <QWidget>

class DateEdit : public QDateEdit
{
    Q_OBJECT
    Q_PROPERTY(QDate nullDate READ nullDate WRITE setNullDate )


public:
    DateEdit(QWidget *parent);
    DateEdit(QDate date, QWidget *parent);

    QDate nullDate() const { return _nullDate; }
    QValidator::State validate(QString &input, int &pos) const override;

public Q_SLOTS:
    void setNullDate(QDate date) { _nullDate = date;}
    bool isNull();

private:
    QDate _nullDate;

    // QDateTimeEdit interface
protected:
    QDateTime dateTimeFromText(const QString &text) const override;
    QString textFromDateTime(const QDateTime &dt) const override;

};

#endif // DATEEDIT_H
