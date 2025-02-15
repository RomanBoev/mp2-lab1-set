// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0)
        throw "Input error: len < 0";
	
    BitLen = len;
    MemLen = len / (8 * sizeof(int)) + 1;
    pMem = new unsigned int [MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = 0;
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new unsigned int[MemLen];
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    BitLen = 0;
    MemLen = 0;
    delete[] pMem;
    pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0)
        throw ("Input error: n < 0");
    return (n / (sizeof(unsigned int) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0)
        throw ("Input error: n < 0");

    const int i = n % (sizeof(unsigned int) * 8);
    unsigned int mask = 1 << i;
    return mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n > BitLen)
    {
        throw ("Input error: n < 0 or n > BitLen");
    }

    const unsigned int     i = GetMemIndex(n)
						,  k = GetMemMask(n);
    pMem[i] |= k;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n > BitLen)
    {
        throw ("Input error: n < 0 or n > BitLen");
    }
	
    const unsigned int     i = GetMemIndex(n)
						,  k = GetMemMask(n);
    pMem[i] &= ~k;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0 || n > BitLen)
	{
		throw ("Input error: n < 0 or n > BitLen");
	}

    const unsigned int     i = GetMemIndex(n)
						,  k = GetMemMask(n);
    return (pMem[i] & k);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf)
        return *this;
	
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    delete[] pMem;
	
    pMem = new unsigned int[MemLen];
	for(int i = 0; i < MemLen; i++)
	{
        pMem[i] = bf.pMem[i];
	}
    return *this;
}

bool TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return false;
    if (this == &bf)
        return true;
    for (int i = 0; i < MemLen; i++)
    {
        if(pMem[i] != bf.pMem[i])
            return false;
    }
    return true;
}

bool TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int len = BitLen;
    if (bf.BitLen > len)
        len = bf.BitLen;
	
    TBitField tmp(len);
	for(int i = 0; i < MemLen; i++)
	{
        tmp.pMem[i] = pMem[i];
	}
	for(int i = 0; i < bf.MemLen; i++)
	{
        tmp.pMem[i] |= bf.pMem[i];
	}
    return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int len = BitLen;
    if (bf.BitLen > len)
        len = bf.BitLen;

    TBitField tmp(len);
    for (int i = 0; i < MemLen; i++)
    {
        tmp.pMem[i] = pMem[i];
    }
    for (int i = 0; i < bf.MemLen; i++)
    {
        tmp.pMem[i] &= bf.pMem[i];
    }
    return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField tmp(*this);
	for(int i = 0; i < BitLen; ++i)
	{
        if (tmp.GetBit(i))
            tmp.ClrBit(i);
        else
            tmp.SetBit(i);
	}
    return tmp;
}

// ввод/вывод

istream &operator>>(istream &in, TBitField &bf) // ввод
{
    int  i = 0;
    char c;
	do
	{
        in >> c;
    } while (c != ' ');

	while(true)
	{
        in >> c;
        if (c == '0')
            bf.ClrBit(i++);
        else if (c == '1')
            bf.SetBit(i++);
        else break;
	}
	
    return in;
}

ostream &operator<<(ostream &out, const TBitField &bf) // вывод
{
	for(int i = 0; i < bf.GetLength(); ++i)
	{
        out << (bf.GetBit(i) ? '1' : '0');
	}
    return out;
}
