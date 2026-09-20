#define BOOST_TEST_MODULE CommandsTest
#include <boost/test/unit_test.hpp>

#include <sstream>

#include "commands.hpp"

using namespace smirnova;

namespace {
  std::string run(const std::string& script) {
    std::istringstream in(script);
    std::ostringstream out;
    runSession(in, out);
    return out.str();
  }
}

BOOST_AUTO_TEST_CASE(unknown_command_is_reported)
{
  std::string output = run("frobnicate\n");
  BOOST_CHECK_EQUAL(output, "<INVALID COMMAND>\n");
}

BOOST_AUTO_TEST_CASE(warehouse_and_zone_setup)
{
  std::string output = run(
    "add-warehouse \"Depot\"\n"
    "add-zone \"Depot\" \"North\"\n"
    "add-zone \"Depot\" \"Depot\"\n");
  BOOST_CHECK(output.find("<OK: ZONE Depot ADDED AS WAREHOUSE>") != std::string::npos);
  BOOST_CHECK(output.find("<OK: ZONE North ADDED UNDER Depot>") != std::string::npos);
  BOOST_CHECK(output.find("<INVALID COMMAND: ZONE Depot ALREADY EXISTS>") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(registering_client_in_unknown_zone_fails)
{
  std::string output = run("reg-client 111 \"Ivanov\" \"Nowhere\"\n");
  BOOST_CHECK_EQUAL(output, "<INVALID COMMAND: ZONE Nowhere NOT FOUND>\n");
}

BOOST_AUTO_TEST_CASE(full_delivery_cycle)
{
  std::string output = run(
    "add-warehouse \"Depot\"\n"
    "add-zone \"Depot\" \"North\"\n"
    "reg-client 111 \"Ivanov\" \"North\"\n"
    "order 111 \"Fridge\"\n"
    "plan-route\n"
    "load-truck\n"
    "next-stop\n");

  BOOST_CHECK(output.find("<ROUTE PLANNED: Depot → North → Depot>") != std::string::npos);
  BOOST_CHECK(output.find("<TRUCK LOADED: 1 items>") != std::string::npos);
  BOOST_CHECK(output.find("<DELIVERED: Fridge → Ivanov (111)>") != std::string::npos);
  BOOST_CHECK(output.find("<ROUTE COMPLETE. RETURNING TO WAREHOUSE.>") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(dropping_client_with_active_order_fails)
{
  std::string output = run(
    "add-warehouse \"Depot\"\n"
    "add-zone \"Depot\" \"North\"\n"
    "reg-client 111 \"Ivanov\" \"North\"\n"
    "order 111 \"Fridge\"\n"
    "drop-client 111\n");
  BOOST_CHECK(output.find("<INVALID COMMAND: CLIENT 111 HAS ACTIVE ORDERS>") != std::string::npos);
}
