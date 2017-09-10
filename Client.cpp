//
// Created by Arne Gockeln on 02.09.17.
//

#include "Client.h"
#include "RequestFactory.h"
#include "ResponseFactory.h"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <netdb.h>
#include <signal.h>

namespace xtbclient {

  /*!
 * Signal Handler for zombie processes
 */
  typedef void (*sighandler_t)(int);
  static sighandler_t zombie_signal_handler(int sig_nr, sighandler_t signalhandler){
    struct sigaction new_sig, old_sig;
    new_sig.sa_handler = signalhandler;
    sigemptyset(&new_sig.sa_mask);
    new_sig.sa_flags = SA_RESTART;
    if(sigaction(sig_nr, &new_sig, &old_sig) < 0){
      return SIG_ERR;
    }
    return old_sig.sa_handler;
  }

  static void no_zombie(int signr){
    pid_t pid;
    int ret;
    while((pid = waitpid(-1, &ret, WNOHANG)) > 0){
      std::printf("Child-Server with pid=%d closed.\n", pid);
    }
    return;
  }


  Client::Client(ClientType t_clientType) {
    zombie_signal_handler(SIGCHLD, no_zombie);

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
      perror("Unable to create SSL context.");
      ERR_print_errors_fp(stderr);
      exit(EXIT_FAILURE);
    }

    // config ctx
    /*const long flags = SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_COMPRESSION;
    SSL_CTX_set_options(ctx, flags);*/

    printf("SSL context created.\n");

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

//    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    socklen_t server_address_len = sizeof(server_address);

    if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
      perror("Unable to create socket.");
      exit(EXIT_FAILURE);
    }

    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&y, sizeof(y));
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, (const char*)&y, sizeof(y));

    if(connect(server_socket, (struct sockaddr *) &server_address, server_address_len) < 0){
      perror("Unable to connect.");
      close(server_socket);
      abort();
    }

    printf("Connected.\n");

    // openssl init
    OpenSSL_add_all_algorithms();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();
    SSL_load_error_strings();

    if(SSL_library_init() < 0){
      perror("Could not initialize the OpenSSL library!");
      exit(EXIT_FAILURE);
    }

    // create context
    SSL_CTX* ssl_ctx = createContext();
    SSL* ssl = SSL_new(ssl_ctx);
    if(ssl == nullptr){
      fprintf(stderr, "SSL_new failed.\n");
      exit(EXIT_FAILURE);
    }

    SSL_set_fd(ssl, server_socket);
    if(SSL_connect(ssl) == -1){
      ERR_print_errors_fp(stderr);
      perror("SSL_connect failed.");
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
      fprintf(stderr, "SSL is nullptr\n");
      exit(EXIT_FAILURE);
    }

    std::unique_lock<std::mutex> unique_lock(m_ssl_mutex);
    int write_error = SSL_write(t_ssl, t_data, t_data_len);
    unique_lock.unlock();

    switch(SSL_get_error(t_ssl, write_error)){
      case SSL_ERROR_NONE:
        if(t_data_len != write_error){
          perror("ssl incomplete write.");
          exit(EXIT_FAILURE);
        }
        break;
      default:
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
  }

  /*!
   * Mutex read from ssl
   *
   * @param SSL* t_ssl
   * @param void* t_buffer
   * @param int t_buffer_size
   * @return int read bytes
   */
  int Client::ssl_read(SSL *t_ssl, void* t_buffer, int t_buffer_size) {
    if(t_ssl == nullptr){
      fprintf(stderr, "SSL is nullptr\n");
      exit(EXIT_FAILURE);
    }

    std::unique_lock<std::mutex> unique_lock(m_ssl_mutex);
    int read_bytes = SSL_read(t_ssl, t_buffer, t_buffer_size);
    unique_lock.unlock();

    return read_bytes;
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
      // mutex write to ssl
      ssl_write(ssl, t_json.data(), static_cast<int>(t_json.length()));

      response = getResponse();
    }

    // return response
    return response;
  }

  /*!
   * Check if response has two \n\n at the end
   * which marks the end of the response
   *
   * @param std::string t_response
   * @return bool
   */
  bool Client::is_response_end(std::string t_buffer) {
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
  void Client::cleanResponse(std::string &t_response) {
    // check if empty string
    if( !t_response.empty() ){
      if( is_response_end( t_response ) ){
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

    char buf[1024];
    SSL* ssl = getSSL();
    SSL_CTX_set_options(SSL_get_SSL_CTX(ssl), SSL_OP_NO_SSLv2);

    if(ssl){
      memset(buf, '\0', sizeof(buf));
      int bytes;
      // first read, if record block termination found, return response
      bytes = SSL_read(ssl, buf, sizeof(buf) - 1);
      switch(SSL_get_error(ssl, bytes)){
        case SSL_ERROR_NONE:
          response.append(buf, strlen(buf));

          if( is_response_end( response ) ){
            // clean up response string
            cleanResponse(response);
            // return response
            return response;
          }
          break;
        case SSL_ERROR_ZERO_RETURN:
          if( is_response_end( response ) ){
            // clean up response string
            cleanResponse(response);
            // return response
            return response;
          }
          break;
        case SSL_ERROR_WANT_READ:
        case SSL_ERROR_WANT_WRITE:

          break;
      }

      // no response end found, read on ..
      bool bLooping = true;
      while( bLooping ){

        memset(buf, '\0', sizeof(buf));
        bytes = SSL_read(ssl, buf, sizeof(buf) - 1);

        switch(SSL_get_error(ssl, bytes)){
          case SSL_ERROR_NONE:
            response.append(buf, strlen(buf));

            if( is_response_end( response ) ){
              bLooping = false;
            }
            break;
          case SSL_ERROR_ZERO_RETURN:
            if( is_response_end( response ) ){
              bLooping = false;
            }
            break;
          case SSL_ERROR_WANT_READ:
          case SSL_ERROR_WANT_WRITE:

            break;
        } // - switch end
      } // - while end
    } // - if ssl end

    // clean up response string
    cleanResponse(response);

    // return response string
    return response;
  }

  /*!
   * Get ChartLastRequest
   *
   * @param ChartLastInfoRecrd& t_record
   * @return ChartLastRequest
   */
  ChartLastRequest Client::getChartLastRequest(ChartLastInfoRecord &t_record) {

    ChartLastRequest chartLastRequest;

    if(t_record.m_start == 0){
      Util::printError("ChartLastInfoRecord.m_start is 0!");
      return chartLastRequest;
    }

    if(t_record.m_symbol.empty()){
      Util::printError("ChartLastInfoRecord.m_symbol is empty!");
      return chartLastRequest;
    }

    std::string request = sendRequest( RequestFactory::getChartLastRequest( t_record ) );
    Util::debug(request);
    return ResponseFactory::getChartLastRequest(request);
  }

  /*!
   * Try to login and obtain a stream session id
   *
   * @param const char* t_username
   * @param const char* t_password
   * @return bool
   */
  bool Client::sendLogin(const char *t_username, const char *t_password) {
    std::string login_response = sendRequest( RequestFactory::getLogin( t_username, t_password, nullptr, nullptr ) );
    // get session id
    std::string sessionId = ResponseFactory::getStreamSessionId( login_response );

    if(!sessionId.empty()){
      // set session id
      setStreamSessionId( &sessionId );
      return true;
    }

    fprintf(stderr, "could not obtain stream session id!\n");

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
  std::string* Client::getStreamSessionId() {
    return m_streamSessionId;
  }


  /*!
   * Set stream session id
   *
   * @param t_streamSessionId
   */
  void Client::setStreamSessionId(std::string *t_streamSessionId) {
    m_streamSessionId = t_streamSessionId;
  }

  /*!
   * Subscribe to balance data
   */
  void Client::subscribeBalance() {
    sendRequest(RequestFactory::subscribeBalance(m_streamSessionId));

    StreamListener *s = m_streamlistner;
    startFork([s](std::string response){
      s->onBalance(response);
    });
  }

  /*!
   * Stop balance subscription
   */
  void Client::stopBalance() {
    sendRequest(RequestFactory::stopBalance());
  }

  /*!
   * Subscribe to candle data
   *
   * @param std::string t_symbol
   */
  void Client::subscribeCandles(std::string t_symbol) {
    sendRequest(RequestFactory::subscribeCandles(&t_symbol, m_streamSessionId));

    StreamListener *s = m_streamlistner;
    startFork([s](std::string response){
      s->onCandles(response);
    });
  }

  /*!
   * Stop candle subscription
   * @param t_symbol
   */
  void Client::stopCandles(std::string t_symbol) {
    sendRequest(RequestFactory::stopCandles(&t_symbol));
  }

  /*!
   * Subscribe to keep alive
   */
  void Client::subscribeKeepAlive() {
    sendRequest(RequestFactory::subscribeKeepAlive(m_streamSessionId));

    StreamListener *s = m_streamlistner;
    startFork([s](std::string response){
      s->onKeepAlive(response);
    });
  }

  /*!
   * Stop keep alive subscription
   */
  void Client::stopKeepAlive() {
    sendRequest(RequestFactory::stopKeepAlive());
  }

  /*!
   * Subscribe to news
   */
  void Client::subscribeNews() {
    sendRequest(RequestFactory::subscribeNews(m_streamSessionId));

    StreamListener *s = m_streamlistner;
    startFork([s](std::string response){
      s->onNews(response);
    });
  }

  /*!
   * Stop news subscription
   */
  void Client::stopNews() {
    sendRequest(RequestFactory::stopNews());
  }

  /*!
   * Subscribe to profits
   */
  void Client::subscribeProfits() {
    sendRequest(RequestFactory::subscribeProfits(m_streamSessionId));

    StreamListener *s = m_streamlistner;
    startFork([s](std::string response){
      s->onProfits(response);
    });
  }

  /*!
   * Stop profits subscription
   */
  void Client::stopProfits() {
    sendRequest(RequestFactory::stopProfits());
  }

  /*!
   * Subscribe to tick prices
   *
   * @param std::string t_symbol
   * @param int t_minArrivalTime
   * @param int t_maxLevel
   */
  void Client::subscribeTickPrices(std::string t_symbol, int t_minArrivalTime, int t_maxLevel) {
    sendRequest(RequestFactory::subscribeTickPrices(&t_symbol, t_minArrivalTime, t_maxLevel, m_streamSessionId));

    StreamListener *s = m_streamlistner;
    startFork([s](std::string response){
      s->onTickPrices(response);
    });
  }

  /*!
   * Stop tick price subscription for symbol
   *
   * @param std::string t_symbol
   */
  void Client::stopTickPrices(std::string t_symbol) {
    sendRequest(RequestFactory::stopTickPrices(&t_symbol));
  }

  /*!
   * Subscribe to trades
   */
  void Client::subscribeTrades() {
    sendRequest(RequestFactory::subscribeTrades(m_streamSessionId));

    StreamListener *s = m_streamlistner;
    startFork([s](std::string response){
      s->onTrades(response);
    });
  }

  /*!
   * Stop trades subscription
   */
  void Client::stopTrades() {
    sendRequest(RequestFactory::stopTrades());
  }

  /*!
   * Subscribe to trade status
   */
  void Client::subscribeTradeStatus() {
    sendRequest(RequestFactory::subscribeTradeStatus(m_streamSessionId));

    StreamListener *s = m_streamlistner;
    startFork([s](std::string response){
      s->onTradeStatus(response);
    });
  }

  /*!
   * Stop trade status subscription
   */
  void Client::stopTradeStatus() {
    sendRequest(RequestFactory::stopTradeStatus());
  }

  /*!
   * Start fork process for subscriptions
   *
   * @param std::function<void(std::string)> t_callback
   */
  void Client::startFork(std::function<void(std::string)> t_callback) {
    int timeout = 0;
    pid_t child_pid;

    if((child_pid = fork()) == 0){
      // wait 10 seconds for response, then quit loop
      while (timeout < 11) {
        // wait for receiving
        std::string jsonResponse = getResponse();

        // echo response
        if (jsonResponse.length() > 0) {
          // reset timeout
          timeout = 0;

          // handle callback
          t_callback(jsonResponse);
        }

        sleep(1);
        timeout++;
      }

      // leave fork
      exit(EXIT_SUCCESS);
    }
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
};