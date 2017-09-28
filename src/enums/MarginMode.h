/*
 * MarginMode.h
 * SymbolRecord
 *  Created on: 10.06.2017
 *      Author: arnegockeln
 */

#ifndef SRC_XTB_RECORD_MARGINMODE_H_
#define SRC_XTB_RECORD_MARGINMODE_H_

namespace xtbclient {

enum class MARGIN_MODE {
	MM_FOREX = 101,
	MM_CFG_LEVERAGED = 102,
  MM_CFD = 103
};

}

#endif /* SRC_XTB_RECORD_MARGINMODE_H_ */
