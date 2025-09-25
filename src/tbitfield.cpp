// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "../include/tbitfield.h"
#include<stdexcept>
using namespace std;

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
  if ( len <= 0)
    throw std::invalid_argument("jopa3");
  
  BitLen=len;
  MemLen = ( BitLen + sizeof(TELEM)* 8 - 1 ) / (sizeof(TELEM)* 8);
  pMem = new TELEM [MemLen];
  for (int i = 0; i < MemLen; i++)
    pMem[i] = 0;

}
// конструктор копирования
TBitField::TBitField(const TBitField &bf) : BitLen(bf.BitLen), MemLen(bf.MemLen)
{
  pMem = new TELEM[MemLen];
  for ( int i = 0; i<MemLen; ++i)
    pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
  if (pMem)
  {
    delete[] pMem;
    pMem = nullptr;
    BitLen = 0;
    MemLen = 0;  
  }
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
  if ( (n< 0) || (n>= BitLen))
    throw std::out_of_range("jopa1"); 
  
  return n/ (sizeof (TELEM)*8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
  if ( (n< 0) || (n>=BitLen)){
    throw std::out_of_range("jopa2");
  }
  return 1 << n % (sizeof(TELEM)*8);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
  if ( (n < 0) || (n >= BitLen))
    throw std::out_of_range("jopa4");
  int index = GetMemIndex(n);
  TELEM mask = GetMemMask(n);
  pMem[index] |= mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
  if ( (n < 0) || (n >= BitLen))
    throw std::out_of_range("jopa5");
  int index = GetMemIndex(n);
  TELEM mask = GetMemMask(n);
  pMem[index] &= ~mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  if ( (n < 0) || (n >= BitLen) )
    throw std::out_of_range("jopa6");

  int index = GetMemIndex(n);
  TELEM mask = GetMemMask(n);
  return (pMem[index] & mask) != 0;  
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
  if (*this == bf)
      return *this;


  delete[] pMem;
      
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  
  pMem = new TELEM[MemLen];
  
  for (int i = 0; i < MemLen; i++)
      pMem[i] = bf.pMem[i];
  
  return *this;
}




int TBitField::operator==(const TBitField &bf) const // сравнение
{
  for (int i =0; i<BitLen; ++i)
  {
    if (bf.GetBit(i) != GetBit(i))
      return 0;
  }
  return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
  int maxlen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
  
  TBitField res = TBitField(maxlen);

  for (int i =0; i<BitLen; ++i)
  {
    if (GetBit(i))
      res.SetBit(i);
  }

  for (int i =0; i<bf.BitLen; ++i)
  {
    if (bf.GetBit(i))
      res.SetBit(i);
  }
  return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int maxlen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    int minlen = (BitLen < bf.BitLen) ? BitLen : bf.BitLen;
    
    TBitField res = TBitField(maxlen);
    for (int i =0; i < minlen; ++i)
    {
      if (GetBit(i) && bf.GetBit(i))
        res.SetBit(i);
    }
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
  TBitField res = TBitField(BitLen);
  for (int i =0; i< BitLen; ++i)
  {
    if (!GetBit(i))
      res.SetBit(i);
  }
  return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
  for (int i = 0; i<bf.BitLen; ++i)
  {
    char bit;
    istr >> bit;

    if (bit == '1')
      bf.SetBit(i);
    else if (bit == '0')
      bf.ClrBit(i);
    else
      break;

  }
  return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
  for (int i =0; i<bf.BitLen; ++i)
    ostr << ( bf.GetBit(i) ? '1' : '0');
  
  return ostr;
}
