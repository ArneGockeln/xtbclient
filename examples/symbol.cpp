/*!
 *  Copyright(c)2017, Arne Gockeln
 *  http://www.arnegockeln.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//
// This file demonstrates how to fetch symbol information for EURUSD
//

#include <iostream>
#include <xtbclient/Client.h>

// here are my account credentials defined
// the file is not part of the repository
#include "../Credentials.h"

using namespace xtbclient;

int main() {
  // set client to examples
  Client client(ClientType::DEMO);
  // login
  if( client.login( XTBACCOUNTID, XTBPASSWORD ) ){
    // get symbol record
    SymbolRecord symbolRecord = client.getSymbol( "EURUSD" );
    // output
    printf("bid: %f, ask: %f, low: %f, high: %f\n", symbolRecord.m_bid, symbolRecord.m_ask,
            symbolRecord.m_low, symbolRecord.m_high );
  }
}