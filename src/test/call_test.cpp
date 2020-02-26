#include "lib/call.h"
#include <cstdio>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#define TEST(x) printf(x " - ");tests++;
#define PASS printf("PASSED\n");
#define FAIL printf("FAILED\n");failures++;

int failures;
int tests;

void noop() {}

void call_expected(call c)
{
  PASS;
}

void call_unexpected(call c)
{
  FAIL;
}

void test_call_creation()
{
  TEST("TESTING CALL CREATION");
  call *c = new call();
  delete c;
  PASS;
}

void test_call_access()
{
  TEST("TESTING ACCESS OF A PARAMETER TO A CALL");
  call *c = new call();
  c -> tree().put("endpoint", "parameter");
  if(c -> tree().get<std::string>("endpoint") != "parameter")
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING ACCESS OF A NESTED PARAMETER");
  c -> tree().put("nested1.nested2.3", 5);
  if(c -> tree().get<int>("nested1.nested2.3") != 5)
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING FAILURE OF ACCESS OF A NULL PARAMETER");
  try
  {
    c -> tree().get<int>("a");
    FAIL;
  }
  catch(std::exception& e)
  {
    PASS;
  }
  delete c;
}

void test_call_encoding()
{
  TEST("TESTING ENCODING AND DECODING OF CALL");
  call *c = new call();
  c -> tree().put("endpoint", "parameter");
  c -> tree().put("1.2.3", 5);
  call *c2 = new call();
  c2 -> decode(c -> encode());
  if(c2 -> tree().get<int>("1.2.3") != 5)
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  delete c;
  delete c2;
}

void test_call_network_transfer()
{
  TEST("TESTING TRANSFER OF CALL WITHIN NETWORK");
  call *c = new call;
  c -> tree().put("endpoint", "parameter");
  c -> tree().put("1.2.3", 5);
  // prepare sockets
  boost::asio::io_service io_service;

  // Create all I/O objects.
  boost::asio::ip::tcp::acceptor acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 0));
  boost::asio::ip::tcp::socket rs(io_service);
  boost::asio::ip::tcp::socket ws(io_service);

  // Connect client and server sockets.
  acceptor.async_accept(rs, boost::bind(&noop));
  ws.async_connect(acceptor.local_endpoint(), boost::bind(&noop));
  io_service.run();

  // transferring call
  write_call(&ws, *c);
  call c2 = read_call(&rs);
  if(c2.tree().get<std::string>("endpoint") != "parameter")
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  delete c;
}

void test_endpoint_table()
{
  endpoint_table et;
  call c;
  TEST("TESTING CALLBACK CALL ON EXPECTED ENDPOINT");
  et.add("1", call_expected);
  et.add_err(call_unexpected);
  et.look_up("1", c);
  TEST("TESTING CALLBACK CALL ON UNEXPECTED ENDPOINT");
  et.add("1", call_unexpected);
  et.add_err(call_expected);
  et.look_up("2", c);
  TEST("TESTING REMOVAL OF UNEXPECTED ENDPOINT");
  et.remove("1");
  et.add_err(call_expected);
  et.look_up("1", c);
}

int main()
{
  test_call_creation();
  test_call_access();
  test_call_encoding();
  test_call_network_transfer();
  test_endpoint_table();
  printf("PASSED %d/%d TESTS!\n", tests - failures, tests);
  return failures;
}
