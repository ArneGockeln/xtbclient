# xtbclient
C++ Client for the xtb trading api.

This library provides all required classes and methods to connect and talk with the xtb trading json socket api.

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
#include <xtbclient/Client.h>

using namespace xtbclient;

int main() {
  // set client to demos
  Client client(ClientType::DEMO);
  // login
  if(client.login("user_id", "super_awesome_password")){
    // get symbol record
    SymbolRecord symbolRecord = client.getSymbol("EURUSD");
    // output
    fprintf(stdout, "bid: %f, ask: %f, low: %f, high: %f\n", symbolRecord.m_bid, symbolRecord.m_ask,
            symbolRecord.m_low, symbolRecord.m_high);
  }
}
```

Find more examples in the folder examples/