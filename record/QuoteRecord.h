//
// Created by Arne Gockeln on 15.07.17.
//

#ifndef XTBLIB_QUOTERECORD_H
#define XTBLIB_QUOTERECORD_H

#include <cstdint>
#include "../enums/Weekdays.h"

namespace xtblib {
 struct QuoteRecord {
   Weekdays m_day;
   uint64_t m_fromT;
   uint64_t m_toT;
 };
}

#endif //XTBLIB_QUOTERECORD_H
