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
// This file demonstrates how to subscribe to tick price stream
//

#include <iostream>
#include <xtbclient/Client.h>

// here are my account credentials defined
// the file is not part of the repository
#include "../Credentials.h"

using namespace xtbclient;

/*!
 * The stream listener
 */
class SimpleListener: public StreamListener {
public:
  void onTickPrices(StreamTickRecord tickRecord) override {
    printf("TickRecord: ask=%f, bid=%f, askVolume=%d, bidVolume=%d, high=%f, low=%f\n",
            tickRecord.m_ask, tickRecord.m_bid,
            tickRecord.m_askVolume, tickRecord.m_bidVolume,
            tickRecord.m_high, tickRecord.m_low);
  }
};

int main() {
  // set client to examples
  Client client(ClientType::DEMO);
  // login
  if( client.login( XTBACCOUNTID, XTBPASSWORD ) ){
    // set stream client
    Client streamClient(ClientType::DEMO_STREAM);
    // set stream session id
    streamClient.setStreamSessionId( client.getStreamSessionId() );
    // subscribe to tick prices
    streamClient.subscribeTickPrices( "EURUSD" );
    // set stream listener and start listening
    SimpleListener listener;
    streamClient.setStreamListener( &listener );
  }
}