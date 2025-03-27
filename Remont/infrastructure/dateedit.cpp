#include "dateedit.h"

DateEdit::DateEdit(QWidget *parent) : QDateEdit(parent)
{

}

DateEdit::DateEdit(QDate date, QWidget *parent) : QDateEdit(date, parent)
{

}


QDateTime DateEdit::dateTimeFromText(const QString &text) const
{
    QDateTime date = QDateTime::fromString(text, displayFormat());
    return date;
}

QString DateEdit::textFromDateTime(const QDateTime &dt) const
{
    if(dt.date() <= nullDate())
        return specialValueText();

    return dt.toString(displayFormat());
}


QValidator::State DateEdit::validate(QString &input, int &/*pos*/) const
{
    QValidator::State state;
    QDate date = QDate::fromString(input, displayFormat());
    if(date.isValid())
        state = QValidator::State::Acceptable;
    else
        state = QValidator::Acceptable;

    return state;
}

bool DateEdit::isNull()
{
    return date() <= nullDate();
}
