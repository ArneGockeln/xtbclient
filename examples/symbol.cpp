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
    fprintf( stdout, "bid: %f, ask: %f, low: %f, high: %f\n", symbolRecord.m_bid, symbolRecord.m_ask,
            symbolRecord.m_low, symbolRecord.m_high );
  }
}