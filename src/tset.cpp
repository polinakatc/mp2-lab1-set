// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "../include/tset.h"
#include<stdexcept>

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : BitField(mp)
{
    if (mp < 0)
        throw out_of_range("jopa7");
    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField), MaxPower(s.MaxPower)
{
}

// конструктор преобразования типа из битового поля
TSet::TSet(const TBitField &bf) : BitField(bf), MaxPower(bf.GetLength())
{
}
// jоператор преобразования в битовое поле
TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if (Elem < 0 || Elem >= MaxPower)
        throw out_of_range("jopa10");
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if ( Elem< 0 || Elem >= MaxPower)
        throw invalid_argument("jopa8");
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if ( Elem< 0 || Elem >= MaxPower)
        throw invalid_argument("jopa9");
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    if (*this != s)
    {
        MaxPower = s.MaxPower;
        BitField = s.BitField;
    }
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    return (BitField == s.BitField) && (MaxPower == s.MaxPower);
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    int maxpower = (MaxPower > s.MaxPower) ? MaxPower : s.MaxPower;
    TSet res = TSet (maxpower);
    res.BitField = BitField | s.BitField;
    return res;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if ( Elem< 0 || Elem >= MaxPower)
        throw invalid_argument("jopa11");
    TSet result(*this);
    result.InsElem(Elem);
    return result;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if ( Elem< 0 || Elem >= MaxPower)
        throw invalid_argument("jopa12");
    TSet result(*this);
    result.DelElem(Elem);
    return result;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    int maxpower = (MaxPower > s.MaxPower) ? MaxPower : s.MaxPower;
    TSet res = TSet (maxpower);
    res.BitField = BitField & s.BitField;
    return res;
    
}

TSet TSet::operator~(void) // дополнение
{
    TSet result(MaxPower);
    result.BitField = ~BitField;
    return result;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    int t, n;
    istr >> n;
    for (int i=0; i<n; ++i)
    {
        istr >> t;
        s.InsElem(t);
    }
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    for (int i=0; i<s.MaxPower; ++i)
    {
        if (s.IsMember(i))
            ostr << i;
    }
    return ostr;
}