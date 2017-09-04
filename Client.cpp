//
// Created by Arne Gockeln on 02.09.17.
//

#include "Client.h"
#include "RequestFactory.h"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <netdb.h>
#include <vector>

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

    OpenSSL_add_ssl_algorithms();
    SSL_load_error_strings();

    ctx = SSL_CTX_new( SSLv23_client_method() );
    if(ctx == NULL){
      perror("Unable to create SSL context.");
      ERR_print_errors_fp(stderr);
      exit(EXIT_FAILURE);
    }

    printf("SSL context created.\n");

    return ctx;
  }

  /*!
   * Initialize client
   */
  void Client::initClient() {
    int server_socket;
    int server_port;
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

#if OPENSSL_VERSION_NUMBER < 0x10100000L
    SSL_library_init();
#else
    OPENSSL_init_ssl(0, NULL);
#endif

    SSL_CTX* ssl_ctx = createContext();
    SSL* ssl = SSL_new(ssl_ctx);
    SSL_set_fd(ssl, server_socket);
    if(SSL_connect(ssl) == -1){
      ERR_print_errors_fp(stderr);
      perror("SSL_connect failed.");
      close(server_socket);
      SSL_CTX_free(ssl_ctx);
      exit(EXIT_FAILURE);
    }

    // request
//    callback(ssl);
    setSSL(ssl);
  }

  /*!
   * Write to ssl fd
   *
   * @param std::string data
   */
  void Client::sendRequest(std::string data) {

    SSL *ssl = getSSL();

    if(ssl != nullptr && data.length() > 0){
      std::lock_guard<std::mutex> guard(m_ssl_mutex);
      if(SSL_write(ssl, data.data(), data.length()) < 0){
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
      }
    }
  }

  /*!
   * Read from ssl fd
   *
   * @return std::string
   */
  std::string Client::getResponse() {

    SSL *ssl = getSSL();

    std::string response;

    if(ssl != nullptr){
      int read_error, read_block = 0, read_block_on_write = 0;
      const int bufferSize = 1024;

      std::vector<std::string> chunks;

      do {

        read_block = 0;
        read_block_on_write = 0;

        char responseBuffer[bufferSize];
        read_error = SSL_read(ssl, responseBuffer, sizeof(responseBuffer) - 1);
        switch(SSL_get_error(ssl, read_error)){
          case SSL_ERROR_NONE:
            chunks.push_back(static_cast<std::string>(responseBuffer));

            break;
          case SSL_ERROR_WANT_READ:
            read_block = 1;
            break;
          case SSL_ERROR_WANT_WRITE:
            read_block_on_write = 1;
            break;
          case SSL_ERROR_ZERO_RETURN:
             read_block = 1;
            break;
          case SSL_ERROR_SYSCALL:
            fprintf(stderr, "SSL Error: Premature close\n");
             read_block = 1;
            break;
          default:
            fprintf(stderr, "SSL read problem.\n");
            read_block = 1;
            break;
        }
      } while(SSL_pending(ssl) > 0 && !read_block);

      // concatenate chunks
      for(auto& chunk : chunks){
        response.append(chunk);
      }

      Util::handleJsonResponseError(&response);
    }

    return response;
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