# xtbclient
C++ Client for the xtb trading api.

This library provides all required classes and methods to connect and talk with the xtb trading json socket api.

## Broker API Version
Current Version 2.4.15 is supported. http://developers.xstore.pro/documentation/2.4.15

## Style
I followed the style guide of [C++ Best Practice](https://github.com/lefticus/cppbestpractices) by lefticus. 

## Dependencies
- C++14 
- [OpenSSL](https://github.com/openssl/openssl) use github and configure with shared option
- [RapidJson](https://github.com/Tencent/rapidjson/)
- [date](https://github.com/HowardHinnant/date)

## Build
### OpenSSL
```bash
$ cd /tmp
$ git clone https://github.com/openssl/openssl
$ cd openssl
$ ./config shared
$ make
$ make test
$ make install
```

### rapidjson
```bash
$ cd /tmp
$ git clone https://github.com/Tencent/rapidjson
$ mkdir rapidjson/build
$ cd rapidjson/build
$ ccmake ../
$ make
$ make test
$ make install
```

### date
```bash
$ cd /tmp
$ git clone https://github.com/HowardHinnant/date
$ cd date
$ cp -R include/date /usr/local/include
```

### xtbclient
```bash
$ cd /tmp
$ git clone https://github.com/ArneGockeln/xtbclient
$ mkdir xtbclient/build
$ cd xtbclient/build
$ ccmake ../
$ make
$ make install
```

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
