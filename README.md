# xtbclient
C++ Client for the xtb trading api.

This library provides all required classes and methods to connect and talk with the xtb trading json socket api.

**The library is in development and not ready for production use!!**

## Broker API Version
Current Version 2.4.15 is supported. http://developers.xstore.pro/documentation/2.4.15

## Style
I followed the style guide of [C++ Best Practice](https://github.com/lefticus/cppbestpractices) by lefticus. 

## Dependencies
- C++17 
- [OpenSSL](http://openssl.org/)
- [RapidJson](http://rapidjson.org/)
- [date](https://github.com/HowardHinnant/date)

# Examples

### Login
```C++
using namespace xtbclient;
using namespace std;

// set up client
Client client(ClientType::DEMO);
// try to login
if( client.sendLogin("account_id", "super_awesome_password") ) {
  // send request for user data
  UserDataRecord userDataRecord = client.getCurrentUserData();
  cout << "Currency: " << userDataRecord.m_currency << endl;
} // - if client.sendLogin end
```

More examples will follow.