

#pragma once

#include <iostream>

inline auto& cout = std::cout;
inline auto& cerr = std::cerr;

using streambuf = std::streambuf;
using ostream = std::ostream;

template <class _Elem, class _Traits>
std::basic_ostream<_Elem, _Traits>& endl(std::basic_ostream<_Elem, _Traits>& _Ostr)
{
    return std::endl(_Ostr);
}

template <class _Elem, class _Traits>
std::basic_ostream<_Elem, _Traits>& flush(std::basic_ostream<_Elem, _Traits>& _Ostr)
{
    return std::flush(_Ostr);
}

#endif
