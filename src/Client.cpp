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

#include "Client.h"
#include "RequestFactory.h"
#include "record/StreamNewsRecord.h"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <netdb.h>
#include <math.h>

namespace xtbclient {

  Client::Client(ClientType t_clientType) {
    setClientType(t_clientType);
    initClient();
  }

  /*!
   * Create ssl context
   *
   * @return SSL_CTX*
   */
  SSL_CTX *Client::createContext() {
    SSL_CTX *ctx;

    ctx = SSL_CTX_new( SSLv23_client_method() );
    if(ctx == nullptr){
      printf("Err: Unable to create SSL context.\n");
      ERR_print_errors_fp(stderr);
      exit(EXIT_FAILURE);
    }

    return ctx;
  }

  /*!
   * Initialize client
   */
  void Client::initClient() {
    int server_socket;
    int server_port = 0;
    std::string server_host;
    int y = 1;

    server_host = "xapia.x-station.eu";

    switch(m_clientType){
      case ClientType::LOCAL:
        server_port = 15000;
        server_host = "localhost";
        break;
      case ClientType::LOCAL_STREAM:
        server_port = 15001;
        server_host = "localhost";
        break;
      case ClientType::DEMO:
        server_port = 5124;
        break;
      case ClientType::DEMO_STREAM:
        server_port = 5125;
        break;
      case ClientType::REAL:
        server_port = 5112;
        break;
      case ClientType::REAL_STREAM:
        server_port = 5113;
        break;
    }

    struct sockaddr_in server_address = { 0 };
    struct hostent* he;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);

    if((he = gethostbyname(server_host.data())) == nullptr){
      herror(server_host.data());
      exit(EXIT_FAILURE);
    }

    bcopy(he->h_addr_list[0], &server_address.sin_addr, he->h_length);

    socklen_t server_address_len = sizeof(server_address);

    if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
      printf("Err: Unable to create socket.\n");
      exit(EXIT_FAILURE);
    }

    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&y, sizeof(y));
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, (const char*)&y, sizeof(y));

    if(connect(server_socket, (struct sockaddr *) &server_address, server_address_len) < 0){
      printf("Err: Unable to connect.\n");
      close(server_socket);
      abort();
    }

    if( m_flag_verbose ){
      printf("Connected to %s:%d\n", server_host.c_str(), server_port);
    }

    // openssl init
    OpenSSL_add_all_algorithms();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();
    SSL_load_error_strings();

    if(SSL_library_init() < 0){
      printf("Err: Could not initialize the OpenSSL library!\n");
      exit(EXIT_FAILURE);
    }

    // create context
    SSL_CTX* ssl_ctx = createContext();
    SSL* ssl = SSL_new(ssl_ctx);
    if(ssl == nullptr){
      printf("Err: SSL_new failed.\n");
      exit(EXIT_FAILURE);
    }

    SSL_set_fd(ssl, server_socket);
    if(SSL_connect(ssl) == -1){
      ERR_print_errors_fp(stderr);
      printf("Err: SSL_connect failed.\n");
      close(server_socket);
      SSL_CTX_free(ssl_ctx);
      exit(EXIT_FAILURE);
    }

    setSSL(ssl);
  }

  /*!
   * Mutex write to ssl
   *
   * @param SSL* t_ssl
   * @param const char* t_data
   * @param const int t_data_len
   */
  void Client::ssl_write(SSL* t_ssl, const char* t_data, const int t_data_len) {
    if(t_ssl == nullptr){
      printf("Err: SSL is nullptr\n");
      exit(EXIT_FAILURE);
    }

    std::unique_lock<std::mutex> unique_lock(m_ssl_mutex);
    int write_error = SSL_write(t_ssl, t_data, t_data_len);
    unique_lock.unlock();

    switch(SSL_get_error(t_ssl, write_error)){
      case SSL_ERROR_NONE:
        if(t_data_len != write_error){
          printf("Err: SSL incomplete write.\n");
          exit(EXIT_FAILURE);
        }
        break;
      default:
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
  }

  /*!
   * Write to ssl fd and return response
   *
   * @param std::string json request
   * @return std::string response
   */
  std::string Client::sendRequest(std::string t_json) {

    SSL *ssl = getSSL();

    std::string response;

    if(ssl != nullptr && t_json.length() > 0){

      // debug requests
      if( m_debug_json_requests ){
        printf("Req: %s\n", t_json.c_str());
      }

      // mutex write to ssl
      ssl_write(ssl, t_json.c_str(), static_cast<int>(t_json.length()));

      response = getResponse();

      // debug response
      if( m_debug_json_response ){
        printf("Resp: %s\n", response.c_str());
      }
    }

    // return response
    return response;
  }

  /*!
   * Write to ssl fd without response
   *
   * @param std::string t_json
   */
  void Client::sendRequestWithoutResponse(std::string t_json){
    SSL *ssl = getSSL();

    if( ssl != nullptr && !t_json.empty() ){

      if( m_debug_json_requests ){
        printf("Req: %s\n", t_json.c_str());
      }

      // mutex write to ssl
      ssl_write(ssl, t_json.c_str(), static_cast<int>(t_json.length()));
    }
  }

  /*!
   * Check if response has two \n\n at the end
   * which marks the end of the response
   *
   * @param std::string t_response
   * @return bool
   */
  bool Client::isResponseEnd(std::string t_buffer) {
    if( !t_buffer.empty() ){
      const char* c_response = t_buffer.c_str();
      const int response_len = strlen(c_response);
      const char nl1 = c_response[response_len - 1];
      const char nl2 = c_response[response_len - 2];

      if(nl1 == '\n' && nl2 == '\n'){
        return true;
      }
    }

    return false;
  }

  /*!
   * Clean up response string and check for json parser error
   *
   * @param std::string& t_response
   */
  void Client::cleanResponse( std::string &t_response ) {
    // check if empty string
    if( !t_response.empty() ){
      if( isResponseEnd( t_response ) ){
        // remove the \n\n record terminator
        Util::rtrim( t_response );
        Util::rtrim( t_response );
      }
    }
  }

  /*!
   * Read from ssl until \n\n record termination found
   *
   * @return std::string json response
   */
  std::string Client::getResponse() {
    std::string response;

    char buf[ 1024 ];
    SSL* ssl = getSSL();
    SSL_CTX_set_options(SSL_get_SSL_CTX(ssl), SSL_OP_NO_SSLv2);

    if(ssl){
      memset(buf, '\0', sizeof(buf));
      int bytes;
      // first read, if record block termination found, return response
      m_ssl_mutex.lock();
      bytes = SSL_read( ssl, buf, sizeof( buf ) - 1 );
      switch( SSL_get_error( ssl, bytes ) ){
        case SSL_ERROR_NONE:
          response.append( buf, strlen( buf ) );

          if( isResponseEnd( response ) ){
            // clean up response string
            cleanResponse( response );
            // unlock mutex
            m_ssl_mutex.unlock();
            // return response
            return response;
          }
          break;
        case SSL_ERROR_ZERO_RETURN:
          if( isResponseEnd( response ) ){
            // clean up response string
            cleanResponse( response );
            // unlock mutex
            m_ssl_mutex.unlock();
            // return response
            return response;
          }
          break;
        case SSL_ERROR_WANT_READ:
        case SSL_ERROR_WANT_WRITE:

          break;
      }
      // unlock mutex
      m_ssl_mutex.unlock();

      // no response end found, read on ..
      bool bLooping = true;
      while( bLooping ){

        memset( buf, '\0', sizeof( buf ) );
        m_ssl_mutex.lock();
        bytes = SSL_read( ssl, buf, sizeof( buf ) - 1 );

        switch( SSL_get_error( ssl, bytes ) ){
          case SSL_ERROR_NONE:
            response.append( buf, strlen( buf ) );

            if( isResponseEnd( response ) ){
              // unlock mutex
              m_ssl_mutex.unlock();
              // leave while
              bLooping = false;
            }
            break;
          case SSL_ERROR_ZERO_RETURN:
            if( isResponseEnd( response ) ){
              // unlock mutex
              m_ssl_mutex.unlock();
              // leave while
              bLooping = false;
            }
            break;
          case SSL_ERROR_WANT_READ:
          case SSL_ERROR_WANT_WRITE:

            break;
        } // - switch end
        // unlock mutex
        m_ssl_mutex.unlock();

      } // - while end
    } // - if ssl end

    // clean up response string
    cleanResponse( response );

    // return response string
    return response;
  }

  /*!
   * Get all symbols as list
   *
   * @return std::vector<SymbolRecord>
   */
  std::vector<SymbolRecord> Client::getAllSymbols() {
    std::vector<SymbolRecord> recordList;

    // Request
    std::string response = sendRequest( RequestFactory::getAllSymbols() );

    // Response
    if(Util::hasAPIResponseError( response )){
      return recordList;
    }

    try {
      auto returnData = getReturnData( std::move( response ) )->GetArray();

      for(auto& obj : returnData){
        SymbolRecord record;

        if(!obj["ask"].IsNull()){
          record.m_ask = obj["ask"].GetDouble();
        }
        if(!obj["bid"].IsNull()){
          record.m_bid = obj["bid"].GetDouble();
        }
        if(!obj["categoryName"].IsNull()){
          record.m_categoryName = obj["categoryName"].GetString();
        }
        if(!obj["contractSize"].IsNull()){
          record.m_contractSize = obj["contractSize"].GetUint();
        }
        if(!obj["currency"].IsNull()){
          record.m_currency = obj["currency"].GetString();
        }
        if(!obj["currencyPair"].IsNull()){
          record.m_currencyPair = obj["currencyPair"].GetBool();
        }
        if(!obj["currencyProfit"].IsNull()){
          record.m_currencyProfit = obj["currencyProfit"].GetString();
        }
        if(!obj["description"].IsNull()){
          record.m_description = obj["description"].GetString();
        }
        if(!obj["expiration"].IsNull()){
          record.m_expiration = obj["expiration"].GetUint64();
        }
        if(!obj["groupName"].IsNull()){
          record.m_groupName = obj["groupName"].GetString();
        }
        if(!obj["high"].IsNull()){
          record.m_high = obj["high"].GetDouble();
        }
        if(!obj["initialMargin"].IsNull()){
          record.m_initialMargin = obj["initialMargin"].GetInt();
        }
        if(!obj["instantMaxVolume"].IsNull()){
          record.m_instantMaxVolume = obj["instantMaxVolume"].GetUint();
        }
        if(!obj["leverage"].IsNull()){
          record.m_leverage = obj["leverage"].GetDouble();
        }
        if(!obj["longOnly"].IsNull()){
          record.m_longOnly = obj["longOnly"].GetBool();
        }
        if(!obj["lotMax"].IsNull()){
          record.m_lotMax = obj["lotMax"].GetDouble();
        }
        if(!obj["lotMin"].IsNull()){
          record.m_lotMin = obj["lotMin"].GetDouble();
        }
        if(!obj["lotStep"].IsNull()){
          record.m_lotStep = obj["lotStep"].GetDouble();
        }
        if(!obj["low"].IsNull()){
          record.m_low = obj["low"].GetDouble();
        }
        if(!obj["marginHedged"].IsNull()){
          record.m_marginHedged = obj["marginHedged"].GetInt();
        }
        if(!obj["marginHedgedStrong"].IsNull()){
          record.m_marginHedgedStrong = obj["marginHedgedStrong"].GetBool();
        }
        if(!obj["marginMaintenance"].IsNull()){
          record.m_marginMaintenance = obj["marginMaintenance"].GetInt();
        }
        if(!obj["marginMode"].IsNull()){
          record.m_marginMode = static_cast<MARGIN_MODE>(obj["marginMode"].GetInt());
        }
        if(!obj["percentage"].IsNull()){
          record.m_percentage = obj["percentage"].GetDouble();
        }
        if(!obj["precision"].IsNull()){
          record.m_precision = obj["precision"].GetInt();
        }
        if(!obj["profitMode"].IsNull()){
          record.m_profitMode = static_cast<PROFIT_MODE>(obj["profitMode"].GetInt());
        }
        if(!obj["quoteId"].IsNull()){
          record.m_quoteId = static_cast<QUOTEID>(obj["quoteId"].GetInt());
        }
        if(!obj["shortSelling"].IsNull()){
          record.m_shortSelling = obj["shortSelling"].GetBool();
        }
        if(!obj["spreadRaw"].IsNull()){
          record.m_spreadRaw = obj["spreadRaw"].GetDouble();
        }
        if(!obj["spreadTable"].IsNull()){
          record.m_spreadTable = obj["spreadTable"].GetDouble();
        }
        if(!obj["starting"].IsNull()){
          record.m_starting = obj["starting"].GetUint64();
        }
        if(!obj["stepRuleId"].IsNull()){
          record.m_stepRuleId = obj["stepRuleId"].GetInt();
        }
        if(!obj["stopsLevel"].IsNull()){
          record.m_stopsLevel = obj["stopsLevel"].GetInt();
        }
        if(!obj["swap_rollover3days"].IsNull()){
          record.m_swap_rollover3days = obj["swap_rollover3days"].GetInt();
        }
        if(!obj["swapEnable"].IsNull()){
          record.m_swapEnable = obj["swapEnable"].GetBool();
        }
        if(!obj["swapLong"].IsNull()){
          record.m_swapLong = obj["swapLong"].GetDouble();
        }
        if(!obj["swapShort"].IsNull()){
          record.m_swapShort = obj["swapShort"].GetDouble();
        }
        if(!obj["swapType"].IsNull()){
          record.m_swapType = obj["swapType"].GetInt();
        }
        if(!obj["symbol"].IsNull()){
          record.m_symbol = obj["symbol"].GetString();
        }
        if(!obj["tickSize"].IsNull()){
          record.m_tickSize = obj["tickSize"].GetDouble();
        }
        if(!obj["tickValue"].IsNull()){
          record.m_tickValue = obj["tickValue"].GetDouble();
        }
        if(!obj["time"].IsNull()){
          record.m_time = obj["time"].GetUint64();
        }
        if(!obj["timeString"].IsNull()){
          record.m_timeString = obj["timeString"].GetString();
        }
        if(!obj["trailingEnabled"].IsNull()){
          record.m_trailingEnabled = obj["trailingEnabled"].GetBool();
        }
        if(!obj["type"].IsNull()){
          record.m_type = obj["type"].GetInt();
        }

        recordList.push_back(record);
      }
    } catch(...){
      printf("Err: unknown error in Client::getAllSymbols()\n");
    }

    return recordList;
  }

  /*!
   * Get ChartLastRequest
   *
   * @param ChartLastInfoRecord& t_record
   * @return ChartLastRequestRecord
   */
  ChartLastRequestRecord Client::getChartLastRequest(ChartLastInfoRecord &t_record) {

    ChartLastRequestRecord chartLastRequest;
    chartLastRequest.m_digits = 0;

    // Request validation
    if(t_record.m_start == 0){
      printf("Err: ChartLastInfoRecord.m_start is 0!\n");
      return chartLastRequest;
    }

    if(t_record.m_symbol.empty()){
      printf("Err: ChartLastInfoRecord.m_symbol is empty!\n");
      return chartLastRequest;
    }

    // Request
    std::string response = sendRequest( RequestFactory::getChartLastRequest( t_record ) );

    // Response
    if(Util::hasAPIResponseError( response )){
      return chartLastRequest;
    }

    try {
      auto returnData = getReturnData( std::move( response ) )->GetObject();

      if(!returnData["digits"].IsNull()){
        chartLastRequest.m_digits = returnData["digits"].GetInt();
      }

      if(!returnData["rateInfos"].IsNull() && returnData["rateInfos"].IsArray()){
        for(auto& rateInfo : returnData["rateInfos"].GetArray()){
          RateInfoRecord rateInfoRecord;

          if(!rateInfo["close"].IsNull()){
            rateInfoRecord.m_close = rateInfo["close"].GetDouble();
          }
          if(!rateInfo["ctm"].IsNull()){
            rateInfoRecord.m_ctm = rateInfo["ctm"].GetUint64();
          }
          if(!rateInfo["ctmString"].IsNull()){
            rateInfoRecord.m_ctmString = rateInfo["ctmString"].GetString();
          }
          if(!rateInfo["high"].IsNull()){
            rateInfoRecord.m_high = rateInfo["high"].GetDouble();
          }
          if(!rateInfo["low"].IsNull()){
            rateInfoRecord.m_low = rateInfo["low"].GetDouble();
          }
          if(!rateInfo["open"].IsNull()){
            rateInfoRecord.m_open = rateInfo["open"].GetDouble();
          }
          if(!rateInfo["vol"].IsNull()){
            rateInfoRecord.m_vol = rateInfo["vol"].GetDouble();
          }

          // calculate down to pips
          double power = pow(10, chartLastRequest.m_digits);
          double openPrice = rateInfoRecord.m_open / power;
          rateInfoRecord.m_open = openPrice;
          rateInfoRecord.m_close = openPrice - (rateInfoRecord.m_close / power);
          rateInfoRecord.m_high = openPrice - (rateInfoRecord.m_high / power);
          rateInfoRecord.m_low = openPrice - (rateInfoRecord.m_low / power);

          chartLastRequest.m_rateInfos.push_back(rateInfoRecord);
        }
      }
    } catch(...){
      printf("Err: unknown error in Client::getChartLastRequest\n");
    }

    return chartLastRequest;
  }

  /*!
   * Get Calendar record list
   *
   * @return std::vector<CalendarRecord>
   */
  std::vector<CalendarRecord> Client::getCalendar() {
    std::vector<CalendarRecord> recordList;

    // Request
    std::string response = sendRequest( RequestFactory::getCalendar() );

    // Response
    if(Util::hasAPIResponseError( response )){
      return recordList;
    }

    try {
      auto returnData = getReturnData(std::move(response))->GetArray();

      for(auto& obj : returnData){
        CalendarRecord record;
        // Defaults
        record.m_country = "";
        record.m_current = "";
        record.m_forecast = "";
        record.m_impact = "";
        record.m_period = "";
        record.m_previous = "";
        record.m_time = 0;
        record.m_title = "";

        // Json
        if(!obj["country"].IsNull()){
          record.m_country = obj["country"].GetString();
        }

        if(!obj["current"].IsNull()){
          record.m_current = obj["current"].GetString();
        }

        if(!obj["forecast"].IsNull()){
          record.m_forecast = obj["forecast"].GetString();
        }

        if(!obj["impact"].IsNull()){
          record.m_impact = obj["impact"].GetString();
        }

        if(!obj["period"].IsNull()){
          record.m_period = obj["period"].GetString();
        }

        if(!obj["previous"].IsNull()){
          record.m_previous = obj["previous"].GetString();
        }

        if(!obj["time"].IsNull()){
          record.m_time = obj["time"].GetUint64();
        }

        if(!obj["title"].IsNull()){
          record.m_title = obj["title"].GetString();
        }

        recordList.push_back(record);
      }

    } catch(...){
      printf("Err: unknown error in Client::getCalendar()\n");
    }

    return recordList;
  }

  /*!
   * Get getChartRangeRequest
   *
   * @param ChartRangeInfoRecord t_record
   * @return ChartLastRequestRecord
   */
  ChartLastRequestRecord Client::getChartRangeRequest(ChartRangeInfoRecord &t_record) {
    ChartLastRequestRecord chartLastRequest;
    chartLastRequest.m_digits = 0;

    // Request validation
    if(t_record.m_start == 0){
      printf("Err: ChartRangeInfoRecord.m_start is 0!\n");
      return chartLastRequest;
    }

    if(t_record.m_end == 0){
      printf("Err: ChartRangeInfoRecord.m_end is 0!\n");
      return chartLastRequest;
    }

    if(t_record.m_symbol.empty()){
      printf("Err: ChartRangeInfoRecord.m_symbol is empty!\n");
      return chartLastRequest;
    }

    // Request
    std::string response = sendRequest( RequestFactory::getChartRangeRequest( t_record ) );

    // Response
    if(Util::hasAPIResponseError( response )){
      return chartLastRequest;
    }

    try {
      auto returnData = getReturnData( std::move( response ) )->GetObject();

      if(!returnData["digits"].IsNull()){
        chartLastRequest.m_digits = returnData["digits"].GetInt();
      }

      if( returnData["rateInfos"].IsNull() || !returnData["rateInfos"].IsArray() ){
        throw std::runtime_error("getChartRangeRequest() -> rateInfos is null or not an array!");
      }

      for( const auto& rateInfo : returnData["rateInfos"].GetArray() ){
        RateInfoRecord rateInfoRecord;

        if( !rateInfo["close"].IsNull() ){
          rateInfoRecord.m_close = rateInfo["close"].GetDouble();
        }
        if( !rateInfo["ctm"].IsNull() ){
          rateInfoRecord.m_ctm = rateInfo["ctm"].GetUint64();
        }
        if( !rateInfo["ctmString"].IsNull() ){
          rateInfoRecord.m_ctmString = rateInfo["ctmString"].GetString();
        }
        if( !rateInfo["high"].IsNull() ){
          rateInfoRecord.m_high = rateInfo["high"].GetDouble();
        }
        if( !rateInfo["low"].IsNull() ){
          rateInfoRecord.m_low = rateInfo["low"].GetDouble();
        }
        if( !rateInfo["open"].IsNull() ){
          rateInfoRecord.m_open = rateInfo["open"].GetDouble();
        }
        if( !rateInfo["vol"].IsNull() ){
          rateInfoRecord.m_vol = rateInfo["vol"].GetDouble();
        }

        // calculate down to pips
        double power = pow(10, chartLastRequest.m_digits);
        double openPrice = rateInfoRecord.m_open / power;
        rateInfoRecord.m_open = openPrice;
        rateInfoRecord.m_close = openPrice - (rateInfoRecord.m_close / power);
        rateInfoRecord.m_high = openPrice - (rateInfoRecord.m_high / power);
        rateInfoRecord.m_low = openPrice - (rateInfoRecord.m_low / power);

        chartLastRequest.m_rateInfos.push_back( rateInfoRecord );
      }

    } catch(const std::exception& ex){
      printf("Err: %s\n", ex.what());
    }

    return chartLastRequest;
  }

  /*!
   * Get commission def
   *
   * @param const std::string& t_symbol
   * @param const double t_volume
   * @return CommissionRecord
   */
  CommissionRecord Client::getCommissionDef(const std::string &t_symbol, const double t_volume) {
    CommissionRecord commissionRecord;
    // Defaults
    // Defaults
    commissionRecord.m_rateOfExchange = 0;
    commissionRecord.m_commission = 0;

    // Validation
    if(t_symbol.empty()){
      printf("Err: Symbol is empty!\n");
      return commissionRecord;
    }

    // Request
    std::string response = sendRequest( RequestFactory::getCommissionDef(t_symbol, t_volume) );

    if(Util::hasAPIResponseError( response )){
      return commissionRecord;
    }

    try {
      auto returnData = getReturnData( response )->GetObject();

      if(!returnData["commission"].IsNull()){
        commissionRecord.m_commission = returnData["commission"].GetDouble();
      }

      if(!returnData["rateOfExchange"].IsNull()){
        commissionRecord.m_rateOfExchange = returnData["rateOfExchange"].GetDouble();
      }
    } catch(...){
      printf("Err: unknown error in Client::getCommissionDef()\n");
    }

    return commissionRecord;
  }

  /*!
   * Get current user data
   *
   * @return UserDataRecord
   */
  UserDataRecord Client::getCurrentUserData() {
    UserDataRecord userDataRecord;
    // Defaults
    userDataRecord.m_companyUnit = 0;
    userDataRecord.m_currency = "";
    userDataRecord.m_group = "";
    userDataRecord.m_ibAccount = false;
    userDataRecord.m_leverage = 0;
    userDataRecord.m_leverageMultiplier = 0;
    userDataRecord.m_spreadType = "";
    userDataRecord.m_trailingStop = false;

    // Request
    std::string response = sendRequest( RequestFactory::getCurrentUserData() );

    if(Util::hasAPIResponseError( response )){
      return userDataRecord;
    }

    try {
      auto obj = getReturnData( response )->GetObject();

      if(!obj["companyUnit"].IsNull()){
        userDataRecord.m_companyUnit = obj["companyUnit"].GetInt();
      }

      if(!obj["currency"].IsNull()){
        userDataRecord.m_currency = obj["currency"].GetString();
      }

      if(!obj["group"].IsNull()){
        userDataRecord.m_group = obj["group"].GetString();
      }

      if(!obj["ibAccount"].IsNull()){
        userDataRecord.m_ibAccount = obj["ibAccount"].GetBool();
      }

      if(!obj["leverage"].IsNull()){
        userDataRecord.m_leverage = obj["leverage"].GetInt();
      }

      if(!obj["leverageMultiplier"].IsNull()){
        userDataRecord.m_leverageMultiplier = obj["leverageMultiplier"].GetDouble();
      }

      if(!obj["spreadType"].IsNull()){
        userDataRecord.m_spreadType = obj["spreadType"].GetString();
      }

      if(!obj["trailingStop"].IsNull()){
        userDataRecord.m_trailingStop = obj["trailingStop"].GetBool();
      }
    } catch(...){
      printf("Err: unknown error in Client::getCurrentUserData()\n");
    }

    return userDataRecord;
  }

  /*!
   * Get ibs history data
   *
   * @param const long long t_start
   * @param const long long t_end
   * @return std::vector<IBRecord>
   */
  std::vector<IBRecord> Client::getIbsHistory(const long long t_start, const long long t_end) {
    std::vector<IBRecord> recordList;

    // Validate
    if(t_start == 0){
      printf("Err: start time is 0!\n");
      return recordList;
    }

    if(t_end == 0){
      printf("Err: end time is 0!\n");
      return recordList;
    }

    // Request
    std::string response = sendRequest( RequestFactory::getIbsHistory( t_start, t_end ) );
    if(Util::hasAPIResponseError( response )){
      return recordList;
    }

    try {

      auto returnData = getReturnData( response )->GetArray();
      for(auto &obj : returnData){
        IBRecord record;

        // defaults
        record.m_closePrice = 0;
        record.m_login = "";
        record.m_nominal = 0;
        record.m_openPrice = 0;
        record.m_side = Side::S_IS_NULL;
        record.m_surname = "";
        record.m_symbol = "";
        record.m_timestamp = 0;
        record.m_volume = 0;

        // get json values
        if(!obj["closePrice"].IsNull()){
          record.m_closePrice = obj["closePrice"].GetDouble();
        }

        if(!obj["login"].IsNull()){
          record.m_login = obj["login"].GetString();
        }

        if(!obj["nominal"].IsNull()){
          record.m_nominal = obj["nominal"].GetDouble();
        }

        if(!obj["openPrice"].IsNull()){
          record.m_openPrice = obj["openPrice"].GetDouble();
        }

        if(!obj["side"].IsNull()){
          int theSide = obj["side"].GetInt();
          if(theSide == 0){
            record.m_side = Side::S_BUY;
          } else {
            record.m_side = Side::S_SELL;
          }
        }

        if(!obj["surname"].IsNull()){
          record.m_surname = obj["surname"].GetString();
        }

        if(!obj["symbol"].IsNull()){
          record.m_symbol = obj["symbol"].GetString();
        }

        if(!obj["timestamp"].IsNull()){
          record.m_timestamp = obj["timestamp"].GetUint64();
        }

        if(!obj["volume"].IsNull()){
          record.m_volume = obj["volume"].GetDouble();
        }

        recordList.push_back(record);
      }

    } catch(...){
      printf("Err: unknown error in Client::getIbsHistory()\n");
    }

    return recordList;
  }

  /*!
   * Get margin level
   *
   * @return MarginLevelRecord
   */
  MarginLevelRecord Client::getMarginLevel() {
    MarginLevelRecord record;

    // Defaults
    record.m_balance = 0;
    record.m_credit = 0;
    record.m_currency = "";
    record.m_equity = 0;
    record.m_margin = 0;
    record.m_margin_free = 0;
    record.m_margin_level = 0;

    // Request
    std::string response = sendRequest( RequestFactory::getMarginLevel() );
    if(Util::hasAPIResponseError( response )){
      return record;
    }

    // Parse
    try {
      auto obj = getReturnData( response )->GetObject();

      if(!obj["balance"].IsNull()){
        record.m_balance = obj["balance"].GetDouble();
      }
      if(!obj["credit"].IsNull()){
        record.m_credit = obj["credit"].GetDouble();
      }
      if(!obj["currency"].IsNull()){
        record.m_currency = obj["currency"].GetString();
      }
      if(!obj["equity"].IsNull()){
        record.m_equity = obj["equity"].GetDouble();
      }
      if(!obj["margin"].IsNull()){
        record.m_margin = obj["margin"].GetDouble();
      }
      if(!obj["margin_free"].IsNull()){
        record.m_margin_free = obj["margin_free"].GetDouble();
      }
      if(!obj["margin_level"].IsNull()){
        record.m_margin_level = obj["margin_level"].GetDouble();
      }

    } catch(...){
      printf("Err: unknown error in Client::getMarginLevel()\n");
    }

    return record;
  }

  /*!
   * Get Margin trade
   *
   * @param const std::string& t_symbol
   * @param double t_volume
   * @return double
   */
  double Client::getMarginTrade(const std::string &t_symbol, double t_volume) {
    // Validate
    if(t_symbol.empty()){
      printf("Err: Symbol is empty!\n");
      return 0;
    }

    // Request
    std::string response = sendRequest( RequestFactory::getMarginTrade(t_symbol, t_volume) );
    if(Util::hasAPIResponseError( response )){
      return 0;
    }

    double ret = 0;
    try {
      auto obj = getReturnData( response )->GetObject();

      if(!obj["margin"].IsNull()){
        ret = obj["margin"].GetDouble();
      }
    } catch(...){
      printf("Err: unknown error in Client::getMarginTrade()\n");
    }

    return ret;
  }

  /*!
   * Get news
   *
   * @param long long t_start
   * @param long long t_end
   * @return std::vector<NewsRecord>
   */
  std::vector<NewsRecord> Client::getNews(long long t_start, long long t_end) {
    std::vector<NewsRecord> newsList;

    // Validate
    if(t_start == 0){
      printf("Err: Start time is 0!\n");
      return newsList;
    }

    // Request
    std::string response = sendRequest( RequestFactory::getNews(t_start, t_end) );
    if(Util::hasAPIResponseError( response )){
      return newsList;
    }

    try {
      for(auto& obj : getReturnData( response )->GetArray()){
        NewsRecord record;

        // Defaults
        record.m_body = "";
        record.m_bodylen = 0;
        record.m_key = "";
        record.m_time = 0;
        record.m_timeString = "";
        record.m_title = "";

        if(!obj["body"].IsNull()){
          record.m_body = obj["body"].GetString();
        }
        if(!obj["bodylen"].IsNull()){
          record.m_bodylen = obj["bodylen"].GetInt();
        }
        if(!obj["key"].IsNull()){
          record.m_key = obj["key"].GetString();
        }
        if(!obj["time"].IsNull()){
          record.m_time = obj["time"].GetUint64();
        }
        if(!obj["timeString"].IsNull()){
          record.m_timeString = obj["timeString"].GetString();
        }
        if(!obj["title"].IsNull()){
          record.m_title = obj["title"].GetString();
        }

        newsList.push_back(record);
      }
    } catch(...){
      printf("Err: unknown error in Client::getNews()\n");
    }

    return newsList;
  }

  /*!
   * Get profit calculation
   *
   * @param double t_closePrice
   * @param double t_openPrice
   * @param TransactionCmd t_cmd
   * @param const std::string& t_symbol
   * @param double t_volume
   * @return double
   */
  double Client::getProfitCalculation(double t_closePrice, double t_openPrice, TransactionCmd t_cmd,
                                      const std::string &t_symbol, double t_volume) {
    double ret = 0;

    // validate
    if(t_symbol.empty()){
      printf("Err: Symbol is empty!\n");
      return ret;
    }

    // request
    std::string response = sendRequest( RequestFactory::getProfitCalculation(t_closePrice, t_openPrice, t_cmd, t_symbol, t_volume));
    if(Util::hasAPIResponseError( response )){
      return ret;
    }

    // parse
    try {
      auto obj = getReturnData( response )->GetObject();
      if(!obj["profit"].IsNull()){
        ret = obj["profit"].GetDouble();
      }
    } catch(...){
      printf("Err: unknown error in Client::getProfitCalculation()\n");
    }

    return ret;
  }

  /*!
   * Get Server time
   *
   * @return ServerTimeRecord
   */
  ServerTimeRecord Client::getServerTime() {
    ServerTimeRecord serverTimeRecord;

    // request
    std::string response = sendRequest( RequestFactory::getServerTime() );
    if(Util::hasAPIResponseError( response )){
      return serverTimeRecord;
    }

    try {
      auto obj = getReturnData( response )->GetObject();
      if(!obj["time"].IsNull()){
        serverTimeRecord.m_time = obj["time"].GetUint64();
      }
      if(!obj["timeString"].IsNull()){
        serverTimeRecord.m_timeString = obj["timeString"].GetString();
      }
    } catch(...){
      printf("Err: unknown error in Client::getServerTime()\n");
    }

    return serverTimeRecord;
  }

  /*!
   * Get step rules
   *
   * @return std::vector<StepRuleRecord>
   */
  std::vector<StepRuleRecord> Client::getStepRules() {
    std::vector<StepRuleRecord> recordList;

    // request
    std::string response = sendRequest( RequestFactory::getStepRules() );
    if(Util::hasAPIResponseError( response )){
      return recordList;
    }

    // parse
    try {
      for(auto& obj : getReturnData( response )->GetArray()){
        StepRuleRecord record;
        record.m_id = 0;
        record.m_name = "";
        record.m_steps = {};

        if(!obj["id"].IsNull()){
          record.m_id = obj["id"].GetInt();
        }
        if(!obj["name"].IsNull()){
          record.m_name = obj["name"].GetString();
        }
        if(!obj["steps"].IsNull() && obj["steps"].IsArray()){
          for(auto& stepObj : obj["steps"].GetArray()){
            StepRecord stepRecord;
            stepRecord.m_fromValue = 0;
            stepRecord.m_step = 0;

            if(!stepObj["fromValue"].IsNull()){
              stepRecord.m_fromValue = stepObj["fromValue"].GetDouble();
            }
            if(!stepObj["step"].IsNull()){
              stepRecord.m_step = stepObj["step"].GetDouble();
            }

            record.m_steps.push_back(stepRecord);
          }
        }

        recordList.push_back(record);
      }
    } catch(...){
      printf("Err: unknown error in Client::getStepRules()\n");
    }

    return recordList;
  }

  /*!
   * Get symbol info
   *
   * @param const std::string& t_symbol
   * @return SymbolRecord
   */
  SymbolRecord Client::getSymbol(const std::string &t_symbol) {
    SymbolRecord record;

    // request
    std::string response = sendRequest( RequestFactory::getSymbol( t_symbol ) );
    if(Util::hasAPIResponseError( response )){
      return record;
    }

    // parse
    try {
      auto obj = getReturnData( response )->GetObject();
      if(!obj["ask"].IsNull()){
        record.m_ask = obj["ask"].GetDouble();
      }
      if(!obj["bid"].IsNull()){
        record.m_bid = obj["bid"].GetDouble();
      }
      if(!obj["categoryName"].IsNull()){
        record.m_categoryName = obj["categoryName"].GetString();
      }
      if(!obj["contractSize"].IsNull()){
        record.m_contractSize = obj["contractSize"].GetUint();
      }
      if(!obj["currency"].IsNull()){
        record.m_currency = obj["currency"].GetString();
      }
      if(!obj["currencyPair"].IsNull()){
        record.m_currencyPair = obj["currencyPair"].GetBool();
      }
      if(!obj["currencyProfit"].IsNull()){
        record.m_currencyProfit = obj["currencyProfit"].GetString();
      }
      if(!obj["description"].IsNull()){
        record.m_description = obj["description"].GetString();
      }
      if(!obj["expiration"].IsNull()){
        record.m_expiration = obj["expiration"].GetUint64();
      }
      if(!obj["groupName"].IsNull()){
        record.m_groupName = obj["groupName"].GetString();
      }
      if(!obj["high"].IsNull()){
        record.m_high = obj["high"].GetDouble();
      }
      if(!obj["initialMargin"].IsNull()){
        record.m_initialMargin = obj["initialMargin"].GetInt();
      }
      if(!obj["instantMaxVolume"].IsNull()){
        record.m_instantMaxVolume = obj["instantMaxVolume"].GetUint();
      }
      if(!obj["leverage"].IsNull()){
        record.m_leverage = obj["leverage"].GetDouble();
      }
      if(!obj["longOnly"].IsNull()){
        record.m_longOnly = obj["longOnly"].GetBool();
      }
      if(!obj["lotMax"].IsNull()){
        record.m_lotMax = obj["lotMax"].GetDouble();
      }
      if(!obj["lotMin"].IsNull()){
        record.m_lotMin = obj["lotMin"].GetDouble();
      }
      if(!obj["lotStep"].IsNull()){
        record.m_lotStep = obj["lotStep"].GetDouble();
      }
      if(!obj["low"].IsNull()){
        record.m_low = obj["low"].GetDouble();
      }
      if(!obj["marginHedged"].IsNull()){
        record.m_marginHedged = obj["marginHedged"].GetInt();
      }
      if(!obj["marginHedgedStrong"].IsNull()){
        record.m_marginHedgedStrong = obj["marginHedgedStrong"].GetBool();
      }
      if(!obj["marginMaintenance"].IsNull()){
        record.m_marginMaintenance = obj["marginMaintenance"].GetInt();
      }
      if(!obj["marginMode"].IsNull()){
        record.m_marginMode = static_cast<MARGIN_MODE>(obj["marginMode"].GetInt());
      }
      if(!obj["percentage"].IsNull()){
        record.m_percentage = obj["percentage"].GetDouble();
      }
      if(!obj["precision"].IsNull()){
        record.m_precision = obj["precision"].GetInt();
      }
      if(!obj["profitMode"].IsNull()){
        record.m_profitMode = static_cast<PROFIT_MODE>(obj["profitMode"].GetInt());
      }
      if(!obj["quoteId"].IsNull()){
        record.m_quoteId = static_cast<QUOTEID>(obj["quoteId"].GetInt());
      }
      if(!obj["shortSelling"].IsNull()){
        record.m_shortSelling = obj["shortSelling"].GetBool();
      }
      if(!obj["spreadRaw"].IsNull()){
        record.m_spreadRaw = obj["spreadRaw"].GetDouble();
      }
      if(!obj["spreadTable"].IsNull()){
        record.m_spreadTable = obj["spreadTable"].GetDouble();
      }
      if(!obj["starting"].IsNull()){
        record.m_starting = obj["starting"].GetUint64();
      }
      if(!obj["stepRuleId"].IsNull()){
        record.m_stepRuleId = obj["stepRuleId"].GetInt();
      }
      if(!obj["stopsLevel"].IsNull()){
        record.m_stopsLevel = obj["stopsLevel"].GetInt();
      }
      if(!obj["swap_rollover3days"].IsNull()){
        record.m_swap_rollover3days = obj["swap_rollover3days"].GetInt();
      }
      if(!obj["swapEnable"].IsNull()){
        record.m_swapEnable = obj["swapEnable"].GetBool();
      }
      if(!obj["swapLong"].IsNull()){
        record.m_swapLong = obj["swapLong"].GetDouble();
      }
      if(!obj["swapShort"].IsNull()){
        record.m_swapShort = obj["swapShort"].GetDouble();
      }
      if(!obj["swapType"].IsNull()){
        record.m_swapType = obj["swapType"].GetInt();
      }
      if(!obj["symbol"].IsNull()){
        record.m_symbol = obj["symbol"].GetString();
      }
      if(!obj["tickSize"].IsNull()){
        record.m_tickSize = obj["tickSize"].GetDouble();
      }
      if(!obj["tickValue"].IsNull()){
        record.m_tickValue = obj["tickValue"].GetDouble();
      }
      if(!obj["time"].IsNull()){
        record.m_time = obj["time"].GetUint64();
      }
      if(!obj["timeString"].IsNull()){
        record.m_timeString = obj["timeString"].GetString();
      }
      if(!obj["trailingEnabled"].IsNull()){
        record.m_trailingEnabled = obj["trailingEnabled"].GetBool();
      }
      if(!obj["type"].IsNull()){
        record.m_type = obj["type"].GetInt();
      }
    } catch(...){
      printf("Err: unknown error in Client::getSymbol()\n");
    }

    return record;
  }

  /*!
   * Get tick price record
   *
   * @param int t_level
   * @param long long t_timestamp
   * @param std::vector<std::string&> t_symbols
   * @return std::vector<TickRecord>
   */
  std::vector<TickRecord>
  Client::getTickPrices(int t_level, long long t_timestamp, std::vector<std::string> t_symbols) {
    std::vector<TickRecord> tickList;

    // validate
    if(t_timestamp == 0){
      printf("Err: Timestamp is 0!\n");
      return tickList;
    }

    if(t_symbols.empty()){
      printf("Err: Symbol list is empty!\n");
      return tickList;
    }

    // request
    std::string response = sendRequest( RequestFactory::getTickPrices(t_level, t_timestamp, t_symbols) );
    if(Util::hasAPIResponseError( response )){
      return tickList;
    }

    try {
      auto obj = getReturnData( response )->GetObject();

      if(!obj["quotations"].IsNull() && obj["quotations"].IsArray()){
        for(auto& quote : obj["quotations"].GetArray()){
          TickRecord record;

          if(!quote["ask"].IsNull()){
            record.m_ask = quote["ask"].GetDouble();
          }
          if(!quote["askVolume"].IsNull()){
            record.m_askVolume = quote["askVolume"].GetInt();
          }
          if(!quote["bid"].IsNull()){
            record.m_bid = quote["bid"].GetDouble();
          }
          if(!quote["bidVolume"].IsNull()){
            record.m_bidVolume = quote["bidVolume"].GetInt();
          }
          if(!quote["high"].IsNull()){
            record.m_high = quote["high"].GetDouble();
          }
          if(!quote["level"].IsNull()){
            record.m_level = quote["level"].GetInt();
          }
          if(!quote["low"].IsNull()){
            record.m_low = quote["low"].GetDouble();
          }
          if(!quote["spreadRaw"].IsNull()){
            record.m_spreadRaw = quote["spreadRaw"].GetDouble();
          }
          if(!quote["spreadTable"].IsNull()){
            record.m_spreadTable = quote["spreadTable"].GetDouble();
          }
          if(!quote["symbol"].IsNull()){
            record.m_symbol = quote["symbol"].GetString();
          }
          if(!quote["timestamp"].IsNull()){
            record.m_timestamp = quote["timestamp"].GetUint64();
          }

          tickList.push_back(record);
        }
      }
    } catch(...){
      printf("Err: unknown error in Client::getTickPrices()\n");
    }

    return tickList;
  }

  /*!
   * Get trade records
   *
   * @param std::vector<unsigned long int> t_orders
   * @return std::vector<TradeRecord>
   */
  std::vector<TradeRecord> Client::getTradeRecords(std::vector<unsigned long int> t_orders) {
    std::vector<TradeRecord> recordList;

    // validate
    if(t_orders.empty()){
      printf("Err: Order number list is empty!\n");
      return recordList;
    }

    // request
    std::string response = sendRequest( RequestFactory::getTradeRecords(t_orders) );
    if(Util::hasAPIResponseError( response )){
      return recordList;
    }

    try {
      for(auto& obj : getReturnData( response )->GetArray()){
        TradeRecord record = getTradeRecordFromObj(obj);
        recordList.push_back(record);
      }
    } catch(...){
      printf("Err: unknown error in Client::getTradeRecords()\n");
    }

    return recordList;
  }

  /*!
   * Get trades
   *
   * @param bool t_openedOnly
   * @return std::vector<TradeRecord>
   */
  std::vector<TradeRecord> Client::getTrades(bool t_openedOnly) {
    std::vector<TradeRecord> recordList;

    // request
    std::string response = sendRequest( RequestFactory::getTrades(t_openedOnly) );
    if(Util::hasAPIResponseError( response )){
      return recordList;
    }

    // parse
    try {
      for(auto& obj : getReturnData( response )->GetArray()){
        TradeRecord record = getTradeRecordFromObj(obj);
        recordList.push_back(record);
      }
    } catch(...){
      printf("Err: unknown error in Client::getTrades()\n");
    }

    return recordList;
  }

  /*!
   * Get trade history
   *
   * @param long long t_start
   * @param long long t_end
   * @return std::vector<TradeRecord>
   */
  std::vector<TradeRecord> Client::getTradeHistory(long long t_start, long long t_end) {
    std::vector<TradeRecord> recordList;

    // validate
    if(t_start == 0){
      printf("Err: Start time is 0!\n");
      return recordList;
    }

    // request
    std::string response = sendRequest( RequestFactory::getTradesHistory( t_start, t_end ) );
    if(Util::hasAPIResponseError( response )){
      return recordList;
    }

    // parse
    try {
      for(auto& obj : getReturnData( response )->GetArray()){
        TradeRecord record = getTradeRecordFromObj(obj);
        recordList.push_back(record);
      }
    } catch(...){
      printf("Err: unknown error in Client::getTradeHistory()\n");
    }

    return recordList;
  }

  /*!
   * Get trading hours
   *
   * @param std::vector<std::string&> t_symbols
   * @return std::vector<TradingHourRecord>
   */
  std::vector<TradingHourRecord> Client::getTradingHours(std::vector<std::string> t_symbols) {
    std::vector<TradingHourRecord> recordList;

    // validate
    if(t_symbols.empty()){
      printf("Err: Symbol list is empty!\n");
      return recordList;
    }

    // request
    std::string response = sendRequest( RequestFactory::getTradingHours( t_symbols ) );
    if(Util::hasAPIResponseError( response )){
      return recordList;
    }

    // parse
    try {
      for(auto& obj : getReturnData( response )->GetArray()){
        TradingHourRecord record;

        if(!obj["symbol"].IsNull()){
          record.m_symbol = obj["symbol"].GetString();
        }

        if(obj["quotes"].IsArray()){
          for(auto& quote : obj["quotes"].GetArray()){
            QuoteRecord quoteRecord;

            if(!quote["day"].IsNull()){
              quoteRecord.m_day = static_cast<Weekdays>(quote["day"].GetInt());
            }
            if(!quote["fromT"].IsNull()){
              quoteRecord.m_fromT = quote["fromT"].GetUint64();
            }
            if(!quote["toT"].IsNull()){
              quoteRecord.m_toT = quote["toT"].GetUint64();
            }

            record.m_quotes.push_back(quoteRecord);
          }
        }

        if(obj["trading"].IsArray()){
          for(auto& trading : obj["trading"].GetArray()){
            TradingRecord tradingRecord;

            if(!trading["day"].IsNull()){
              tradingRecord.m_day = static_cast<Weekdays>(trading["day"].GetInt());
            }
            if(!trading["fromT"].IsNull()){
              tradingRecord.m_fromT = trading["fromT"].GetUint64();
            }
            if(!trading["toT"].IsNull()){
              tradingRecord.m_toT = trading["toT"].GetUint64();
            }

            record.m_trading.push_back(tradingRecord);
          }
        }

        recordList.push_back(record);
      }
    } catch(...){
      printf("Err: unknown error in Client::getTradingHours()\n");
    }

    return recordList;
  }

  /*!
   * Get api version
   *
   * @return std::string
   */
  std::string Client::getVersion() {
    std::string version;

    // request
    std::string response = sendRequest( RequestFactory::getVersion() );
    if(Util::hasAPIResponseError( response )){
      return version;
    }

    // parse
    try {
      auto obj = getReturnData( response )->GetObject();
      if(!obj["version"].IsNull()){
        version = obj["version"].GetString();
      }
    } catch(...){
      printf("Err: unknown error in Client::getVersion()\n");
    }

    return version;
  }

  /*!
   * Send ping
   */
  void Client::ping() {
    // request
    sendRequestWithoutResponse( RequestFactory::getPing() );
  }

  /*!
   * Get trade transaction
   *
   * @param TradeTransaction& t_info
   * @return unsigned long int order id
   */
  unsigned long int Client::tradeTransaction(TradeTransactionInfo &t_info) {
    unsigned long int ordernr = 0;

    // validate
    if(t_info.m_symbol.empty()){
      printf("Err: Symbol is empty!\n");
      return ordernr;
    }

    if(t_info.m_price == 0){
      printf("Err: Price is 0!\n");
      return ordernr;
    }

    // request
    std::string request = RequestFactory::getTradeTransaction( t_info );
    std::string response = sendRequest( request );

    // parse
    try {
      auto obj = getReturnData( response )->GetObject();
      if(!obj["order"].IsNull()){
        ordernr = static_cast<unsigned long int>(obj["order"].GetUint64());
      }
    } catch(...){
      printf("Err: unknown error in Client::tradeTransaction()\n");
    }

    return ordernr;
  }

  /*!
   * Get trade transaction status
   *
   * @param unsigned long int t_ordernr
   * @return TradeTransactionStatusRecord
   */
  TradeTransactionStatusRecord Client::getTradeTransactionStatus(unsigned long int t_ordernr) {
    TradeTransactionStatusRecord record;

    // validate
    if(t_ordernr == 0){
      printf("Err: Ordernr is 0!\n");
      return record;
    }

    // request
    std::string response = sendRequest( RequestFactory::getTradeTransactionStatus( t_ordernr ) );

    // parse
    try {
      auto obj = getReturnData( response )->GetObject();

      if(!obj["ask"].IsNull()){
        record.m_ask = obj["ask"].GetDouble();
      }
      if(!obj["bid"].IsNull()){
        record.m_bid = obj["bid"].GetDouble();
      }
      if(!obj["customComment"].IsNull()){
        record.m_customComment = obj["customComment"].GetString();
      }
      if(!obj["message"].IsNull()){
        record.m_message = obj["message"].GetString();
      }
      if(!obj["order"].IsNull()){
        record.m_order = static_cast<unsigned long int>(obj["order"].GetUint64());
      }
      if(!obj["requestStatus"].IsNull()){
        record.m_requestStatus = static_cast<RequestStatus>(obj["requestStatus"].GetInt());
      }
    } catch(...){
      printf("Err: unknown error in Client::getTradeTransactionStatus()\n");
    }

    return record;
  }

  /*!
   * Get trade record from json object
   *
   * @param const rapidjson::GenericValue<rapidjson::UTF8<char>> &obj
   * @return TradeRecord
   */
  TradeRecord Client::getTradeRecordFromObj(const rapidjson::GenericValue<rapidjson::UTF8<char>> &obj) {
    TradeRecord record;

    if(!obj["close_price"].IsNull()){
      record.m_close_price = obj["close_price"].GetDouble();
    }
    if(!obj["close_time"].IsNull()){
      record.m_close_time = obj["close_time"].GetUint64();
    }
    if(!obj["close_timeString"].IsNull()){
      record.m_close_timeString = obj["close_timeString"].GetString();
    }
    if(!obj["closed"].IsNull()){
      record.m_closed = obj["closed"].GetBool();
    }
    if(!obj["cmd"].IsNull()){
      record.m_cmd = static_cast<TransactionCmd>(obj["cmd"].GetInt());
    }
    if(!obj["comment"].IsNull()){
      record.m_comment = obj["comment"].GetString();
    }
    if(!obj["commission"].IsNull()){
      record.m_commission = obj["commission"].GetDouble();
    }
    if(!obj["customComment"].IsNull()){
      record.m_customComment = obj["customComment"].GetString();
    }
    if(!obj["digits"].IsNull()){
      record.m_digits = obj["digits"].GetInt();
    }
    if(!obj["expiration"].IsNull()){
      record.m_expiration = obj["expiration"].GetUint64();
    }
    if(!obj["expirationString"].IsNull()){
      record.m_expirationString = obj["expirationString"].GetString();
    }
    if(!obj["margin_rate"].IsNull()){
      record.m_margin_rate = obj["margin_rate"].GetDouble();
    }
    if(!obj["offset"].IsNull()){
      record.m_offset = obj["offset"].GetInt();
    }
    if(!obj["open_price"].IsNull()){
      record.m_open_price = obj["open_price"].GetDouble();
    }
    if(!obj["open_time"].IsNull()){
      record.m_open_time = obj["open_time"].GetUint64();
    }
    if(!obj["open_timeString"].IsNull()){
      record.m_open_timeString = obj["open_timeString"].GetString();
    }
    if(!obj["order"].IsNull()){
      record.m_order = static_cast<unsigned long int>(obj["order"].GetUint64());
    }
    if(!obj["order2"].IsNull()){
      record.m_order2 = static_cast<unsigned long int>(obj["order2"].GetUint64());
    }
    if(!obj["position"].IsNull()){
      record.m_position = static_cast<unsigned long int>(obj["position"].GetUint64());
    }
    if(!obj["profit"].IsNull()){
      record.m_profit = obj["profit"].GetDouble();
    }
    if(!obj["sl"].IsNull()){
      record.m_sl = obj["sl"].GetDouble();
    }
    if(!obj["storage"].IsNull()){
      record.m_storage = obj["storage"].GetDouble();
    }
    if(!obj["symbol"].IsNull()){
      record.m_symbol = obj["symbol"].GetString();
    }
    if(!obj["timestamp"].IsNull()){
      record.m_timestamp = obj["timestamp"].GetUint64();
    }
    if(!obj["tp"].IsNull()){
      record.m_tp = obj["tp"].GetDouble();
    }
    if(!obj["volume"].IsNull()){
      record.m_volume = obj["volume"].GetDouble();
    }

    return record;
  }

  /*!
   * Try to login and obtain a stream session id
   *
   * @param const char* t_username
   * @param const char* t_password
   * @return bool
   */
  bool Client::login(const char *t_username, const char *t_password) {
    std::string login_response = sendRequest( RequestFactory::getLogin( t_username, t_password, nullptr, nullptr ) );

    if( Util::hasAPIResponseError( login_response )){
      return false;
    }

    // get session id
    auto sessionId = parseStreamSessionId( login_response );

    if(sessionId == nullptr){
      printf("Err: streamSessionId is nullptr.\n");
      return false;
    }

    if(strlen(sessionId) > 0){
      // set session id
      setStreamSessionId( sessionId );
      // message
      if ( m_flag_verbose ){
        printf("Logged in.\n");
      }
      return true;
    }

    printf("Err: could not obtain stream session id!\n");

    return false;
  }

  /*!
   * Set stream listener
   *
   * @param StreamListener t_streamListener
   */
  void Client::setStreamListener(StreamListener *t_streamListener) {
    if(t_streamListener != nullptr){
      m_streamlistner = t_streamListener;

      // start listening on stream
      listenOnStream();
    }
  }

  /*!
   * Get ssl pointer
   *
   * @return SSL*
   */
  SSL *Client::getSSL() {
    switch(m_clientType){
      case ClientType::LOCAL:
      case ClientType::DEMO:
      case ClientType::REAL:
        return m_ssl;
      case ClientType::LOCAL_STREAM:
      case ClientType::DEMO_STREAM:
      case ClientType::REAL_STREAM:
        return m_ssl_stream;
      default:
        return nullptr;
    }
  }

  /*!
   * Set ssl pointer
   *
   * @param SSL* t_ssl
   */
  void Client::setSSL(SSL *t_ssl) {
    switch(m_clientType){
      case ClientType::LOCAL:
      case ClientType::DEMO:
      case ClientType::REAL:
        m_ssl = t_ssl;
        break;
      case ClientType::LOCAL_STREAM:
      case ClientType::DEMO_STREAM:
      case ClientType::REAL_STREAM:
        m_ssl_stream = t_ssl;
        break;
    }
  }

  /*!
   * Set client type
   *
   * @param ClientType t_clientType
   */
  void Client::setClientType(ClientType t_clientType) {
    m_clientType = t_clientType;
  }

  /*!
   * Get stream session id
   *
   * @return
   */
  const char* Client::getStreamSessionId() {
    return m_streamSessionId.c_str();
  }


  /*!
   * Set stream session id and start listening
   *
   * @param t_streamSessionId
   */
  void Client::setStreamSessionId(const char* t_streamSessionId) {
    m_streamSessionId = t_streamSessionId;
  }

  /*!
   * Subscribe to balance data
   */
  void Client::subscribeBalance() {
    sendRequestWithoutResponse( RequestFactory::subscribeBalance( getStreamSessionId() ) );
  }

  /*!
   * Stop balance subscription
   */
  void Client::stopBalance() {
    sendRequestWithoutResponse( RequestFactory::stopBalance() );
  }

  /*!
   * Parse balance stream record
   *
   * @param Value* t_data
   */
  void Client::parseBalance(Value* t_data) {
    // parse
    try {
      StreamBalanceRecord balanceRecord{0,0,0,0,0,0};

      auto returnData = t_data->GetObject();

      if(!returnData["balance"].IsNull()){
        balanceRecord.m_balance = returnData["balance"].GetDouble();
      }
      if(!returnData["credit"].IsNull()){
        balanceRecord.m_credit = returnData["credit"].GetDouble();
      }
      if(!returnData["equity"].IsNull()){
        balanceRecord.m_equity = returnData["equity"].GetDouble();
      }
      if(!returnData["margin"].IsNull()){
        balanceRecord.m_margin = returnData["margin"].GetDouble();
      }
      if(!returnData["marginFree"].IsNull()){
        balanceRecord.m_marginFree = returnData["marginFree"].GetDouble();
      }
      if(!returnData["marginLevel"].IsNull()){
        balanceRecord.m_marginLevel = returnData["marginLevel"].GetDouble();
      }

      m_streamlistner->onBalance(balanceRecord);
    } catch(...){
      printf("Err: unknown error thread->onBalance()\n");
    }
  }

  /*!
   * Subscribe to candle data
   *
   * @param std::string t_symbol
   */
  void Client::subscribeCandles(std::string t_symbol) {
    sendRequestWithoutResponse( RequestFactory::subscribeCandles( &t_symbol, getStreamSessionId() ) );
  }

  /*!
   * Parse candle stream response
   * @param Value* t_data
   */
  void Client::parseCandles(Value* t_data) {
    try {
      StreamCandleRecord candleRecord;

      auto returnData = t_data->GetObject();

      if(!returnData["close"].IsNull()){
        candleRecord.m_close = returnData["close"].GetDouble();
      }
      if(!returnData["ctm"].IsNull()){
        candleRecord.m_ctm = static_cast<long long>(returnData["ctm"].GetUint64());
      }
      if(!returnData["ctmString"].IsNull()){
        candleRecord.m_ctmString = returnData["ctmString"].GetString();
      }
      if(!returnData["high"].IsNull()){
        candleRecord.m_high = returnData["high"].GetDouble();
      }
      if(!returnData["low"].IsNull()){
        candleRecord.m_low = returnData["low"].GetDouble();
      }
      if(!returnData["open"].IsNull()){
        candleRecord.m_open = returnData["open"].GetDouble();
      }
      if(!returnData["quoteId"].IsNull()){
        candleRecord.m_quoteid = static_cast<QUOTEID >(returnData["quoteId"].GetInt());
      }
      if(!returnData["symbol"].IsNull()){
        candleRecord.m_symbol = returnData["symbol"].GetString();
      }
      if(!returnData["vol"].IsNull()){
        candleRecord.m_vol = returnData["vol"].GetDouble();
      }

      m_streamlistner->onCandle(candleRecord);
    } catch(...){
      printf("Err: unknown error thread->onCandle()\n");
    }
  }

  /*!
   * Stop candle subscription
   * @param t_symbol
   */
  void Client::stopCandles(std::string t_symbol) {
    sendRequestWithoutResponse( RequestFactory::stopCandles(&t_symbol) );
  }

  /*!
   * Subscribe to keep alive
   */
  void Client::subscribeKeepAlive() {
    sendRequestWithoutResponse( RequestFactory::subscribeKeepAlive( getStreamSessionId() ) );
  }

  /*!
   * Parse keep alive stream response
   *
   * @param Value* t_data
   */
  void Client::parseKeepAlive(Value* t_data) {
    try {
      auto returnData = t_data->GetObject();

      if(!returnData["timestamp"].IsNull()){
        m_streamlistner->onKeepAlive(returnData["timestamp"].GetInt());
      }

    } catch(...){
      printf("Err: unknown error thread->onKeepAlive()\n");
    }
  }

  /*!
   * Stop keep alive subscription
   */
  void Client::stopKeepAlive() {
    sendRequestWithoutResponse( RequestFactory::stopKeepAlive() );
  }

  /*!
   * Subscribe to news
   */
  void Client::subscribeNews() {
    sendRequestWithoutResponse( RequestFactory::subscribeNews( getStreamSessionId() ) );
  }

  /*!
   * Parse news stream response
   *
   * @param Value* t_data
   */
  void Client::parseNews(Value* t_data) {
    try {
      auto returnData = t_data->GetObject();

      StreamNewsRecord newsRecord{"","",0,""};

      if(!returnData["body"].IsNull()){
        newsRecord.m_body = returnData["body"].GetString();
      }
      if(!returnData["key"].IsNull()){
        newsRecord.m_key = returnData["key"].GetString();
      }
      if(!returnData["time"].IsNull()){
        newsRecord.m_time = returnData["time"].GetInt();
      }
      if(!returnData["title"].IsNull()){
        newsRecord.m_title = returnData["title"].GetString();
      }

      m_streamlistner->onNews(newsRecord);
    } catch(...){
      printf("Err: unknown error thread->onNews()\n");
    }
  }

  /*!
   * Stop news subscription
   */
  void Client::stopNews() {
    sendRequestWithoutResponse( RequestFactory::stopNews() );
  }

  /*!
   * Subscribe to profits
   */
  void Client::subscribeProfits() {
    sendRequestWithoutResponse( RequestFactory::subscribeProfits( getStreamSessionId() ) );
  }

  /*!
   * Parse news stream response
   *
   * @param Value* t_data
   */
  void Client::parseProfits(Value* t_data) {
    try {

      auto returnData = t_data->GetObject();
      StreamProfitRecord profitRecord{0,0,0,0};

      if(!returnData["order"].IsNull()){
        profitRecord.m_order = static_cast<unsigned long int>(returnData["order"].GetUint64());
      }
      if(!returnData["order2"].IsNull()){
        profitRecord.m_order2 = static_cast<unsigned long int>(returnData["order2"].GetUint64());
      }
      if(!returnData["position"].IsNull()){
        profitRecord.m_position = static_cast<unsigned long int>(returnData["position"].GetUint64());
      }
      if(!returnData["profit"].IsNull()){
        profitRecord.m_profit = returnData["profit"].GetDouble();
      }

      m_streamlistner->onProfits(profitRecord);
    } catch(...){
      printf("Err: unknown error thread->onProfits()\n");
    }
  }

  /*!
   * Stop profits subscription
   */
  void Client::stopProfits() {
    sendRequestWithoutResponse( RequestFactory::stopProfits() );
  }

  /*!
   * Subscribe to tick prices
   *
   * @param std::string t_symbol
   * @param int t_minArrivalTime
   * @param int t_maxLevel
   */
  void Client::subscribeTickPrices(std::string t_symbol, int t_minArrivalTime, int t_maxLevel) {
    sendRequestWithoutResponse( RequestFactory::subscribeTickPrices(&t_symbol, t_minArrivalTime, t_maxLevel, getStreamSessionId() ) );
  }

  /*!
   * Parse tick price stream response
   *
   * @param Value* t_data
   */
  void Client::parseTickPrices(Value* t_data) {
    try {
      StreamTickRecord tickRecord{0,0,0,0,0,0,0,QUOTEID::QD_FLOAT,0,0,"",0};

      auto streamData = t_data->GetObject();

      if(!streamData["ask"].IsNull()){
        tickRecord.m_ask = streamData["ask"].GetDouble();
      }
      if(!streamData["askVolume"].IsNull()){
        tickRecord.m_askVolume = streamData["askVolume"].GetInt();
      }
      if(!streamData["bid"].IsNull()){
        tickRecord.m_bid = streamData["bid"].GetDouble();
      }
      if(!streamData["bidVolume"].IsNull()){
        tickRecord.m_bidVolume = streamData["bidVolume"].GetInt();
      }
      if(!streamData["high"].IsNull()){
        tickRecord.m_high = streamData["high"].GetDouble();
      }
      if(!streamData["level"].IsNull()){
        tickRecord.m_level = streamData["level"].GetInt();
      }
      if(!streamData["low"].IsNull()){
        tickRecord.m_low = streamData["low"].GetDouble();
      }
      if(!streamData["quoteId"].IsNull()){
        tickRecord.m_quoteid = static_cast<QUOTEID>(streamData["quoteId"].GetInt());
      }
      if(!streamData["spreadRaw"].IsNull()){
        tickRecord.m_spreadRaw = streamData["spreadRaw"].GetDouble();
      }
      if(!streamData["spreadTable"].IsNull()){
        tickRecord.m_spreadTable = streamData["spreadTable"].GetDouble();
      }
      if(!streamData["symbol"].IsNull()){
        tickRecord.m_symbol = streamData["symbol"].GetString();
      }
      if(!streamData["timestamp"].IsNull()){
        tickRecord.m_timestamp = streamData["timestamp"].GetUint64();
      }

      m_streamlistner->onTickPrices(tickRecord);
    } catch(...){
      printf("Err: unknown error fork->onTickPrices()\n");
    }
  }

  /*!
   * Stop tick price subscription for symbol
   *
   * @param std::string t_symbol
   */
  void Client::stopTickPrices(std::string t_symbol) {
    sendRequestWithoutResponse( RequestFactory::stopTickPrices(&t_symbol) );
  }

  /*!
   * Subscribe to trades
   */
  void Client::subscribeTrades() {
    sendRequestWithoutResponse( RequestFactory::subscribeTrades( getStreamSessionId() ) );
  }

  /*!
   * Parse trade stream reaponse
   *
   * @param Value* t_data
   */
  void Client::parseTrades(Value* t_data) {
    try {

      auto obj = t_data->GetObject();

      StreamTradeRecord record{0};

      if(!obj["close_price"].IsNull()){
        record.m_close_price = obj["close_price"].GetDouble();
      }
      if(!obj["close_time"].IsNull()){
        record.m_close_time = obj["close_time"].GetUint64();
      }
      if(!obj["closed"].IsNull()){
        record.m_closed = obj["closed"].GetBool();
      }
      if(!obj["cmd"].IsNull()){
        record.m_cmd = static_cast<TransactionCmd>(obj["cmd"].GetInt());
      }
      if(!obj["comment"].IsNull()){
        record.m_comment = obj["comment"].GetString();
      }
      if(!obj["commission"].IsNull()){
        record.m_commission = obj["commission"].GetDouble();
      }
      if(!obj["customComment"].IsNull()){
        record.m_customComment = obj["customComment"].GetString();
      }
      if(!obj["digits"].IsNull()){
        record.m_digits = obj["digits"].GetInt();
      }
      if(!obj["expiration"].IsNull()){
        record.m_expiration = obj["expiration"].GetUint64();
      }
      if(!obj["margin_rate"].IsNull()){
        record.m_margin_rate = obj["margin_rate"].GetDouble();
      }
      if(!obj["offset"].IsNull()){
        record.m_offset = obj["offset"].GetInt();
      }
      if(!obj["open_price"].IsNull()){
        record.m_open_price = obj["open_price"].GetDouble();
      }
      if(!obj["open_time"].IsNull()){
        record.m_open_time = obj["open_time"].GetUint64();
      }
      if(!obj["order"].IsNull()){
        record.m_order = static_cast<unsigned long int>(obj["order"].GetUint64());
      }
      if(!obj["order2"].IsNull()){
        record.m_order2 = static_cast<unsigned long int>(obj["order2"].GetUint64());
      }
      if(!obj["position"].IsNull()){
        record.m_position = static_cast<unsigned long int>(obj["position"].GetUint64());
      }
      if(!obj["profit"].IsNull()){
        record.m_profit = obj["profit"].GetDouble();
      }
      if(!obj["sl"].IsNull()){
        record.m_sl = obj["sl"].GetDouble();
      }
      if(!obj["state"].IsNull()){
        record.m_state = obj["state"].GetString();
      }
      if(!obj["storage"].IsNull()){
        record.m_storage = obj["storage"].GetDouble();
      }
      if(!obj["symbol"].IsNull()){
        record.m_symbol = obj["symbol"].GetString();
      }
      if(!obj["tp"].IsNull()){
        record.m_tp = obj["tp"].GetDouble();
      }
      if(!obj["type"].IsNull()){
        record.m_type = static_cast<TransactionType>(obj["type"].GetInt());
      }
      if(!obj["volume"].IsNull()){
        record.m_volume = obj["volume"].GetDouble();
      }

      m_streamlistner->onTrades(record);

    } catch(...){
      printf("Err: unknown error thread->onTrades()\n");
    }
  }

  /*!
   * Stop trades subscription
   */
  void Client::stopTrades() {
    sendRequestWithoutResponse( RequestFactory::stopTrades() );
  }

  /*!
   * Subscribe to trade status
   */
  void Client::subscribeTradeStatus() {
    sendRequestWithoutResponse( RequestFactory::subscribeTradeStatus( getStreamSessionId() ) );
  }

  /*!
   * Parse trade status stream response
   *
   * @param Value* t_data
   */
  void Client::parseTradeStatus(Value* t_data) {
    try {
      auto streamData = t_data->GetObject();

      StreamTradeStatusRecord statusRecord{"","",0,0,RequestStatus::RS_PENDING};

      if(!streamData["customComment"].IsNull()){
        statusRecord.m_customComment = streamData["customComment"].GetString();
      }
      if(!streamData["message"].IsNull()){
        statusRecord.m_message = streamData["message"].GetString();
      }
      if(!streamData["order"].IsNull()){
        statusRecord.m_order = static_cast<unsigned long int>(streamData["order"].GetUint64());
      }
      if(!streamData["price"].IsNull()){
        statusRecord.m_price = streamData["price"].GetDouble();
      }
      if(!streamData["requestStatus"].IsNull()){
        statusRecord.m_requestStatus = static_cast<RequestStatus>(streamData["requestStatus"].GetInt());
      }

      m_streamlistner->onTradeStatus(statusRecord);
    } catch(...){
      printf("Err: unknown error thread->onTradeStatus()\n");
    }
  }

  /*!
   * Stop trade status subscription
   */
  void Client::stopTradeStatus() {
    sendRequestWithoutResponse( RequestFactory::stopTradeStatus() );
  }

  /*!
   * Get stream session id from json string
   *
   * @param std::string t_jsonString
   * @return std::string
   */
  const char* Client::parseStreamSessionId(std::string t_jsonString) {
    Document document = getDocumentFromJson(std::move(t_jsonString));

    if(document.HasParseError()){
      return nullptr;
    }

    if(!document["status"].IsNull()){
      if(document["status"].GetBool()){
        const char* sessionId = document["streamSessionId"].GetString();
        return sessionId;
      }
    }

    return nullptr;
  }

  /*!
   * Parse json to rapidjson document
   *
   * @param std::string t_jsonString
   * @return rapidjson::Document
   */
  Document Client::getDocumentFromJson(std::string t_jsonString){

    Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(t_jsonString.c_str());

    if(Util::hasDocumentParseError(&document)){
      printf("Err: CorruptJsonString: %s\n", t_jsonString.c_str());
    }

    return document;
  }

  /*!
   * Get returnData from json string
   *
   * @param std::string t_json
   * @return rapidjson::Value*
   */
  Value* Client::getReturnData(std::string t_json) {
    Document document = getDocumentFromJson(std::move(t_json));
    Value* value = nullptr;

    // check rapidjson parser
    if(Util::hasDocumentParseError(&document)){
      return value;
    }

    // has error?
    if(document.HasMember("errorCode")){
      printf("Err: API Error %s: %s\n", document["errorCode"].GetString(), document["errorDescr"].GetString());
    } else if(!document.HasMember("returnData") || !document.HasMember("status")){
      printf("Err: No returnData found.\n");
    } else if(!document["status"].IsNull()) {
      if (document["status"].GetBool()) {
        if(!document["returnData"].IsNull()){
          value = &document["returnData"];
        }
      }
    }

    return value;
  }

  /*!
   * Listen on stream for stream data
   */
  void Client::listenOnStream() {
    if(!m_ssl_stream){
      printf("Err: No ssl stream connection available!\n");
      return;
    }

    if(!m_streamlistner){
      printf("Err: No StreamListener found!\n");
      return;
    }

    // start thread
    std::thread listen_thread([=](){

      while(true){
        if(!m_ssl_stream){
          printf("Err: SSL connection is broken.\n");
          break;
        }

        // wait for receiving
        std::string response = getResponse();

        // if no response, continue looking
        if(response.empty()){
          continue;
        }

        // split responses into multiple json responses
        std::vector<std::string> multiResponse = Util::explode(response, '\n\n');

        for(auto& singleResponse : multiResponse){

          if(singleResponse.empty()){
            continue;
          }

          // debug response
          if( m_debug_json_response ){
            printf("Resp: %s\n", singleResponse.c_str());
          }

          // test for api response error
          if(Util::hasAPIResponseError( singleResponse )){
            break;
          }

          // parse single response
          Document document;
          document.Parse( singleResponse.c_str() );

          // get command
          std::string command;
          if(!document["command"].IsNull()){
            command = document["command"].GetString();
          }

          // get data
          Value* data;
          if(!document["data"].IsNull()){
            data = &document["data"];
          }

          if(!data){
            printf("Err: data is empty!\n");
            continue;
          }

          // check which command and call stream listener
          if(command == "tickPrices"){
            parseTickPrices(data);
          } else if(command == "balance"){
            parseBalance(data);
          } else if(command == "candle"){
            parseCandles(data);
          } else if(command == "keepAlive"){
            parseKeepAlive(data);
          } else if(command == "news"){
            parseNews(data);
          } else if(command == "profit"){
            parseProfits(data);
          } else if(command == "trade"){
            parseTrades(data);
          } else if(command == "tradeStatus"){
            parseTradeStatus(data);
          }

        } // - end for multiResponse

        // clear multiResponse
        multiResponse.clear();

      } // - end while
    });

    listen_thread.join();
  }

  /*!
   * Free up memory of ssl and ssl_stream
   */
  Client::~Client() {
    if(m_ssl != nullptr){
      SSL_CTX_free( SSL_get_SSL_CTX( m_ssl ) );
      SSL_free( m_ssl );
    }

    if(m_ssl_stream != nullptr){
      SSL_CTX_free( SSL_get_SSL_CTX( m_ssl_stream ) );
      SSL_free( m_ssl_stream );
    }
  }

  /*!
   * Set to true, to show all json requests in stdout
   *
   * @param bool flag
   */
  void Client::setDebugJsonRequests(bool flag) {
    m_debug_json_requests = flag;
  }

  /*!
   * Set to true, to show all json response in stdout
   *
   * @param bool flag
   */
  void Client::setDebugJsonResponse(bool flag) {
    m_debug_json_response = flag;
  }

  /*!
   * Set to true, to show all json response and requests in stdout
   *
   * @param bool flag_requests
   * @param bool flag_responses
   */
  void Client::setDebugOutput(bool flag_requests, bool flag_responses) {
    setDebugJsonRequests(flag_requests);
    setDebugJsonResponse(flag_responses);
  }

  /*!
   * Set verbose flag
   *
   * @param bool flag
   */
  void Client::setVerbose(bool flag) {
    m_flag_verbose = flag;
  }
};