#define BOOST_TEST_MODULE TruckTest
#include <boost/test/unit_test.hpp>

#include <sstream>

#include "truck.hpp"

using namespace smirnova;

namespace {
  std::vector< std::string > route2() {
    return {"North", "East"};
  }
}

BOOST_AUTO_TEST_CASE(empty_truck_reports_empty)
{
  Truck truck;
  std::ostringstream out;
  truck.print(out);
  BOOST_CHECK_EQUAL(out.str(), "<TRUCK IS EMPTY>\n");
}

BOOST_AUTO_TEST_CASE(loaded_truck_shows_position_and_route)
{
  Truck truck;
  truck.setRoute(route2(), "Warehouse");
  truck.push({"Sofa", "456", "Ivanov", "East"});
  truck.push({"Fridge", "123", "Kozlov", "North"});

  std::ostringstream out;
  truck.print(out);
  std::string text = out.str();
  BOOST_CHECK(text.find("<TRUCK POSITION: Warehouse>") != std::string::npos);
  BOOST_CHECK(text.find("<ROUTE: Warehouse → North → East → Warehouse>") != std::string::npos);
  BOOST_CHECK(text.find("1. Fridge") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(advance_delivers_only_current_zone)
{
  Truck truck;
  truck.setRoute(route2(), "Warehouse");
  truck.push({"Sofa", "456", "Ivanov", "East"});
  truck.push({"Fridge", "123", "Kozlov", "North"});

  std::ostringstream out;
  BOOST_CHECK(truck.advance(out));
  std::string text = out.str();
  BOOST_CHECK(text.find("<ARRIVED: North>") != std::string::npos);
  BOOST_CHECK(text.find("<DELIVERED: Fridge → Kozlov (123)>") != std::string::npos);
  BOOST_CHECK(text.find("<NEXT STOP: East>") != std::string::npos);
  BOOST_CHECK(text.find("Sofa") == std::string::npos);
  BOOST_CHECK(!truck.empty());
}

BOOST_AUTO_TEST_CASE(advance_through_full_route_completes_and_empties)
{
  Truck truck;
  truck.setRoute(route2(), "Warehouse");
  truck.push({"Sofa", "456", "Ivanov", "East"});
  truck.push({"Fridge", "123", "Kozlov", "North"});

  std::ostringstream out1;
  truck.advance(out1);
  std::ostringstream out2;
  BOOST_CHECK(truck.advance(out2));
  BOOST_CHECK(out2.str().find("<ROUTE COMPLETE. RETURNING TO WAREHOUSE.>") != std::string::npos);
  BOOST_CHECK(truck.empty());

  std::ostringstream out3;
  BOOST_CHECK(!truck.advance(out3));
}

BOOST_AUTO_TEST_CASE(advance_on_empty_truck_returns_false)
{
  Truck truck;
  truck.setRoute(route2(), "Warehouse");
  std::ostringstream out;
  BOOST_CHECK(!truck.advance(out));
}
