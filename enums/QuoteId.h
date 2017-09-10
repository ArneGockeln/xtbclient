/*
 * QuoteId.h
 * SymbolRecord
 *  Created on: 10.06.2017
 *      Author: arnegockeln
 */

#ifndef SRC_XTB_RECORD_QUOTEID_H_
#define SRC_XTB_RECORD_QUOTEID_H_

namespace xtbclient {

enum QUOTEID {
	QD_FIXED = 1,
	QD_FLOAT = 2,
	QD_DEPTH = 3,
	QD_CROSS = 4
};

}

#endif /* SRC_XTB_RECORD_QUOTEID_H_ */
