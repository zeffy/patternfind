#pragma once

#include <vector>

namespace patternfind
{
  struct patternbyte
  {
    unsigned char value;
    unsigned char mask;

    patternbyte()
    {
      this->clear();
    }

    patternbyte(const unsigned char value, const unsigned char mask)
    {
      this->value = value;
      this->mask = mask;
    }

    inline bool opaque() const
    {
      return this->mask == 0xffui8;
    }

    inline void clear()
    {
      this->value = 0ui8;
      this->mask = 0xffui8;
    }

    inline bool operator==(const unsigned char &rhs) const
    {
      return (rhs & this->mask) == (this->value & this->mask);
    }
  };

  inline bool operator==(const unsigned char &lhs, const patternbyte &rhs) 
  {
    return (lhs & rhs.mask) == (rhs.value & rhs.mask);
  }

  inline int hexchtoint(char ch)
  {
    if ( ch >= '0' && ch <= '9' )
      return ch - '0';
    else if ( ch >= 'A' && ch <= 'F' )
      return ch - 'A' + 10;
    else if ( ch >= 'a' && ch <= 'f' )
      return ch - 'a' + 10;
    return -1;
  }

  static std::vector<patternbyte> compile(const char *pattern)
  {
    std::vector<patternbyte> v;
    patternbyte b;
    char c;
    int shift = 4;

    while ( c = *pattern++ ) {
      if ( c == '?' || c == '.' )
        b.mask &= ~(0xf << shift);
      else if ( auto num = hexchtoint(c); num != -1 )
        b.value |= (num & 0xf) << shift;
      else
        continue;

      if ( !shift ) {
        v.push_back(b);
        b.clear();
      }
      shift ^= 4;
    }
    if ( !shift ) {
      b.mask &= 0xf0;
      v.push_back(b);
    }
    return v;
  }
}
