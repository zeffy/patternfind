#pragma once
#include <vector>

namespace catgirls
{
  namespace
  {
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
  }

  struct patternbyte
  {
    unsigned char value;
    unsigned char mask;

    patternbyte()
    {
      this->value = 0;
      this->mask = 0;
    }

    patternbyte(const unsigned char value, const unsigned char mask = 0xff)
    {
      this->value = value;
      this->mask = mask;
    }

    inline bool opaque() const
    {
      return this->mask == 0xff;
    }

    inline bool operator==(const unsigned char &rhs) const
    {
      return (rhs & this->mask) == (this->value & this->mask);
    }

    friend inline bool operator==(const unsigned char &lhs, const patternbyte &rhs)
    {
      return (lhs & rhs.mask) == (rhs.value & rhs.mask);
    }
  };

  static std::vector<catgirls::patternbyte> compile(const char *pattern)
  {
    std::vector<catgirls::patternbyte> v;
    catgirls::patternbyte b;
    char c;
    int shift = 4;

    while ( c = *pattern++ ) {
      if ( c == '?' || c == '.' )
        b.mask &= ~(0xf << shift);
      else {
        const int num = hexchtoint(c);
        if ( num < 0 )
          continue;
        b.value |= (num & 0xf) << shift;
      }

      if ( !shift ) {
        v.push_back(b);
        b.value = 0;
        b.mask = 0xff;
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
