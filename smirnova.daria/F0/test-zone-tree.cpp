#define BOOST_TEST_MODULE ZoneTreeTest
#include <boost/test/unit_test.hpp>

#include <sstream>

#include "zone_tree.hpp"

using namespace smirnova;

BOOST_AUTO_TEST_CASE(first_zone_becomes_root)
{
  ZoneTree tree;
  BOOST_CHECK(tree.addZone("", "Warehouse") == ZoneAddResult::Added);
  BOOST_CHECK(tree.hasRoot());
  BOOST_CHECK_EQUAL(tree.rootName(), "Warehouse");
  BOOST_CHECK(tree.exists("Warehouse"));
}

BOOST_AUTO_TEST_CASE(second_root_rejected)
{
  ZoneTree tree;
  tree.addZone("", "Warehouse");
  BOOST_CHECK(tree.addZone("", "Other") == ZoneAddResult::RootAlreadyExists);
}

BOOST_AUTO_TEST_CASE(child_requires_existing_parent)
{
  ZoneTree tree;
  BOOST_CHECK(tree.addZone("Warehouse", "North") == ZoneAddResult::ParentNotFound);
  tree.addZone("", "Warehouse");
  BOOST_CHECK(tree.addZone("Warehouse", "North") == ZoneAddResult::Added);
  BOOST_CHECK(tree.exists("North"));
}

BOOST_AUTO_TEST_CASE(duplicate_zone_name_rejected)
{
  ZoneTree tree;
  tree.addZone("", "Warehouse");
  tree.addZone("Warehouse", "North");
  BOOST_CHECK(tree.addZone("Warehouse", "North") == ZoneAddResult::ZoneAlreadyExists);
  BOOST_CHECK(tree.addZone("", "North") == ZoneAddResult::ZoneAlreadyExists);
}

BOOST_AUTO_TEST_CASE(plan_route_is_pre_order)
{
  ZoneTree tree;
  tree.addZone("", "Warehouse");
  tree.addZone("Warehouse", "North");
  tree.addZone("North", "East");
  tree.addZone("Warehouse", "West");

  std::set< std::string > targets = {"North", "East", "West"};
  std::vector< std::string > route = tree.planRoute(targets);

  std::vector< std::string > expected = {"North", "East", "West"};
  BOOST_CHECK_EQUAL_COLLECTIONS(route.begin(), route.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(plan_route_ignores_zones_without_orders)
{
  ZoneTree tree;
  tree.addZone("", "Warehouse");
  tree.addZone("Warehouse", "North");
  tree.addZone("Warehouse", "West");

  std::set< std::string > targets = {"West"};
  std::vector< std::string > route = tree.planRoute(targets);
  BOOST_REQUIRE_EQUAL(route.size(), 1u);
  BOOST_CHECK_EQUAL(route[0], "West");
}

BOOST_AUTO_TEST_CASE(print_map_marks_active_zones)
{
  ZoneTree tree;
  tree.addZone("", "Warehouse");
  tree.addZone("Warehouse", "North");

  std::ostringstream out;
  tree.printMap({"North"}, out);
  std::string text = out.str();

  BOOST_CHECK(text.find("Warehouse") != std::string::npos);
  BOOST_CHECK(text.find("North*") != std::string::npos);
  BOOST_CHECK(text.find("└── North*") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(print_map_empty_tree)
{
  ZoneTree tree;
  std::ostringstream out;
  tree.printMap({}, out);
  BOOST_CHECK_EQUAL(out.str(), "<MAP IS EMPTY>\n");
}
