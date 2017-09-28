# xtbclient
C++ Client for the xtb trading api.

This library provides all required classes and methods to connect and talk with the xtb trading json socket api.

**The library is in development and not ready for production use!!**

## Broker API Version
Current Version 2.4.15 is supported. http://developers.xstore.pro/documentation/2.4.15

## Style
I followed the style guide of [C++ Best Practice](https://github.com/lefticus/cppbestpractices) by lefticus. 

## Dependencies
- C++14 
- [OpenSSL](http://openssl.org/)
- [RapidJson](http://rapidjson.org/)
- [date](https://github.com/HowardHinnant/date)

# Examples

### Login
```C++
#include <iostream>
#include "Client.h"

using namespace xtbclient;
using namespace std;

int main() {
    // set up client
    Client client(ClientType::DEMO);
    // try to login
    if( client.login("account_id", "super_awesome_password") ) {
      // send request for user data
      UserDataRecord userDataRecord = client.getCurrentUserData();
      cout << "Currency: " << userDataRecord.m_currency << endl;
    } // - if client.sendLogin end
}
```

### Subscribe to tickPrices
```C++
#include <iostream>
#include "Client.h"

using namespace xtbclient;
using namespace std;

// setup callback stream listener
class SimpleListener: public StreamListener {
public:
  void onTickPrices(StreamTickRecord tickRecord) override {
    fprintf(stdout, "%s h: %f; l: %f; ask: %f; bid: %f\n", tickRecord.m_symbol.c_str(), tickRecord.m_high, tickRecord.m_low, tickRecord.m_ask, tickRecord.m_bid);
  }
};

int main() {
  Client client(ClientType::DEMO);
  if( client.login( "account_id", "super_awesome_password") ){
    // set stream client
    Client streamClient(ClientType::DEMO_STREAM);
    streamClient.setStreamSessionId( client.getStreamSessionId() );

    // subscribe to stream tick prices in EURUSD with a delay of 1000ms
    streamClient.subscribeTickPrices("EURUSD", 1000);

    // set stream listener and start listening
    SimpleListener listener;
    streamClient.setStreamListener(&listener);

    while(true){
      // hold on
    }
  }
}
```

More examples will follow.