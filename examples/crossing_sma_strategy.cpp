//
// Created by Arne Gockeln on 09.09.17.
//

// ------------------------------------------------------------------
// -------- WARNING! THIS IS ONLY FOR EDUCATION! --------------------
// ------------------------------------------------------------------

#include <iostream>
#include <xtbclient/Client.h>

// here are my account credentials defined
// the file is not part of the repository
#include "../Credentials.h"

using namespace xtbclient;
using namespace std;

/*!
 * The forex handler
 */
class FXHandler {
private:
  Client *m_client;
  vector<StreamCandleRecord> m_candleList;
  unsigned long int m_order = 0;
  const string m_symbol = "EURUSD";

  /*!
   * Store the previous sma calculation results
   */
  const int m_sma_fast = 5;
  const int m_sma_slow = 10;
  double m_previous_sma_fast = 0;
  double m_previous_sma_slow = 0;

  /*!
   * trade direction 0 no trade, 1 buy, -1 sell
   */
  int m_trade_direction = 0;

  /*!
   * Get SMA on close price
   * @param const int t_count
   * @return double
   */
  double getSimpleMovingAverage(const int t_period){
    double result = 0;

    if( m_candleList.empty() || t_period <= 0 ){
      return result;
    }

    int i = 0;
    auto reverseIterator = m_candleList.rbegin();
    for(; reverseIterator != m_candleList.rend(); ++reverseIterator){
      result += (*reverseIterator).m_close;

      if( i == ( t_period - 1 ) ){
        break;
      }

      i++;
    }

    result = result / t_period;

    return result;
  }

public:
  FXHandler(Client* t_client) {
    m_client = t_client;
  }
  ~FXHandler(){}

  vector<StreamCandleRecord>& getCandleList(){
    return m_candleList;
  }

  string getSymbol(){
    return m_symbol;
  }

  /*!
   * Check if SMAs are crossing
   * @return bool
   */
  bool isEntrySituation(){

    if( m_order > 0 ) {
      return false;
    }

    // reset values
    m_trade_direction = 0;

    // get moving averages
    double smaFast = getSimpleMovingAverage( m_sma_fast );
    double smaSlow = getSimpleMovingAverage( m_sma_slow );

    // buy signal
    if( smaFast > smaSlow && m_previous_sma_fast < m_previous_sma_slow ){
      m_trade_direction = 1;
    }
    // sell signal
    if( smaFast < smaSlow && m_previous_sma_fast > m_previous_sma_slow ){
      m_trade_direction = -1;
    }

    // set previous values
    m_previous_sma_fast = smaFast;
    m_previous_sma_slow = smaSlow;

    // log
    printf("sma%d/sma%d %f/%f %s\n",
            m_sma_fast,
            m_sma_slow,
            smaFast,
            smaSlow,
            (m_trade_direction == 1 ? "buy signal" : (m_trade_direction == -1 ? "sell signal" : "no signal")));

    return m_trade_direction != 0;
  }

  /*!
   * Open Trade Transaction
   * @return
   */
  bool openTrade() {

    // open only 1 trade
    if( m_order > 0 ) {
      return false;
    }

    printf("openTrade\n");

    // get current price information
    SymbolRecord eurusd = m_client->getSymbol( m_symbol );

    // prepare transaction
    TradeTransactionInfo trade;
    trade.m_symbol = m_symbol;
    trade.m_expiration = 0;
    trade.m_offset = 0;
    trade.m_order = 0;
    trade.m_volume = 0.1;
    trade.m_type = TT_OPEN;
    trade.m_cmd = m_trade_direction == 1 ? TC_BUY : TC_SELL;
    trade.m_customComment = m_trade_direction == 1 ? "BUY" : "SELL";

    // Risc
    const double risc = 0.00020;

    // - BUY
    if( m_trade_direction == 1 ){
      trade.m_price = eurusd.m_ask;
      trade.m_sl = trade.m_price - risc;
      trade.m_tp = trade.m_price + (risc * 2);
    }
    // - SELL
    else if( m_trade_direction == - 1 ){
      trade.m_price = eurusd.m_bid;
      trade.m_sl = trade.m_price + risc;
      trade.m_tp = trade.m_price - (risc * 2);
    }

    // send to api and get order id
    unsigned long int orderID = m_client->tradeTransaction( trade );

    // check the status of the transaction
    TradeTransactionStatusRecord transactionStatus = m_client->getTradeTransactionStatus( orderID );

    if( transactionStatus.m_requestStatus == RS_ACCEPTED ){
      m_order = transactionStatus.m_order;
    } else {
      printf("transactionStatus: %d\n", static_cast<int>( transactionStatus.m_requestStatus ) );
    }

    return m_order > 0;
  }

  /*!
   * Close open transaction
   * @return bool
   */
  bool closeTrade() {

    if( m_order == 0 ) {
      return false;
    } // no open trades available

    // get open trades
    vector<TradeRecord> tradeList = m_client->getTrades( true );
    if( !tradeList.empty() ){
      for( auto& trade : tradeList ){
        if( trade.m_order2 == m_order ) {
          // close trade
          TradeTransactionInfo tradeInfo;
          tradeInfo.m_price = trade.m_close_price;
          tradeInfo.m_sl = 0;
          tradeInfo.m_tp = 0;
          tradeInfo.m_symbol = trade.m_symbol;
          tradeInfo.m_order = trade.m_order;
          tradeInfo.m_cmd = TC_BUY;
          tradeInfo.m_type = TT_CLOSE;
          tradeInfo.m_expiration = 0;
          tradeInfo.m_volume = trade.m_volume;

          m_client->tradeTransaction( tradeInfo );

          // check the status of the transaction
          TradeTransactionStatusRecord transactionStatus = m_client->getTradeTransactionStatus( trade.m_order );
          if( transactionStatus.m_requestStatus == RS_ACCEPTED ){
            m_order = 0;
          }

          printf("closeTrade->transactionStatus %d\n", static_cast<int>(transactionStatus.m_requestStatus));
        }
      }
    }

    return m_order == 0;
  }
};

/*!
   * The magic happens here!
   */
class SimpleListener: public StreamListener {
private:
  FXHandler* m_handler;

public:
  SimpleListener(FXHandler* t_handler){
    m_handler = t_handler;
  }

  void onCandle(StreamCandleRecord candleRecord) override {
    m_handler->getCandleList().push_back( candleRecord );

    printf("Candle OHLC: %f %f %f %f | ", candleRecord.m_open, candleRecord.m_high, candleRecord.m_low, candleRecord.m_close);

    // check for entry
    if( m_handler->isEntrySituation() ){
      m_handler->openTrade();
    }
  }

  // get transaction information to stdout
  void onTradeStatus(StreamTradeStatusRecord statusRecord) override {
    string status = "ACCEPTED";

    switch( statusRecord.m_requestStatus ){
      case RS_ACCEPTED:
        status = "ACCEPTED";
        break;
      case RS_PENDING:
        status = "PENDING";
        break;
      case RS_REJECTED:
        status = "REJECTED";
        break;
      default:
      case RS_ERROR:
        status = "ERROR";
        break;
    }

    printf("Trade: %s %s/%ld price: %f status: %s\n", statusRecord.m_customComment.c_str(), statusRecord.m_message.c_str(), statusRecord.m_order,
            statusRecord.m_price, status.c_str());
  }
};

/*!
 * Start
 * @return
 */
int main() {

  Client client(ClientType::DEMO);
  FXHandler fxHandler( &client );

  if( client.login( XTBACCOUNTID, XTBPASSWORD ) ){

    // Prefill candle list with historical data
    ChartRangeInfoRecord rangeRequest;
    rangeRequest.m_symbol = fxHandler.getSymbol();
    rangeRequest.m_period = PERIOD_M1;
    rangeRequest.m_ticks = 0;
    rangeRequest.m_end = Util::getUTCTimestampMS();
    rangeRequest.m_start = Util::getUTCTimestampDifferenceMS(5*3600);
    ChartLastRequestRecord chartRangeRecord = client.getChartRangeRequest( rangeRequest );

    for(auto &item : chartRangeRecord.m_rateInfos){
      StreamCandleRecord candle;
      candle.m_symbol = rangeRequest.m_symbol;
      candle.m_open = item.m_open;
      candle.m_high = item.m_high;
      candle.m_low = item.m_low;
      candle.m_close = item.m_close;
      candle.m_ctmString = item.m_ctmString;
      candle.m_ctm = item.m_ctm;
      candle.m_vol = item.m_vol;

      fxHandler.getCandleList().push_back( candle );
    }

    printf("Historical Data received: %ld candles\n", fxHandler.getCandleList().size());

    // set stream client
    Client streamClient(ClientType::DEMO_STREAM);
    streamClient.setStreamSessionId( client.getStreamSessionId() );

    // subscribe to tade status
    streamClient.subscribeTradeStatus();
    // subscribe to candles
    streamClient.subscribeCandles( fxHandler.getSymbol() );

    printf("Waiting for next candle...\n");

    // set stream listener and start listening
    SimpleListener listener( &fxHandler );
    streamClient.setStreamListener( &listener );
  }
}