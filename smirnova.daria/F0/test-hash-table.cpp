#define BOOST_TEST_MODULE HashTableTest
#include <boost/test/unit_test.hpp>

#include <string>

#include "hash_table.hpp"
#include "string_hash.hpp"

using namespace smirnova;

BOOST_AUTO_TEST_CASE(insert_and_find)
{
  HashTable< std::string, int, StringHash > table;
  BOOST_CHECK(table.insert("a", 1));
  auto it = table.find("a");
  BOOST_REQUIRE(it != table.end());
  BOOST_CHECK_EQUAL(it->second, 1);
}

BOOST_AUTO_TEST_CASE(duplicate_key_rejected)
{
  HashTable< std::string, int, StringHash > table;
  BOOST_CHECK(table.insert("a", 1));
  BOOST_CHECK(!table.insert("a", 2));
  BOOST_CHECK_EQUAL(table.find("a")->second, 1);
}

BOOST_AUTO_TEST_CASE(find_missing_returns_end)
{
  HashTable< std::string, int, StringHash > table;
  BOOST_CHECK(table.find("missing") == table.end());
}

BOOST_AUTO_TEST_CASE(iterator_can_modify_value)
{
  HashTable< std::string, int, StringHash > table;
  table.insert("a", 1);
  table.find("a")->second = 42;
  BOOST_CHECK_EQUAL(table.find("a")->second, 42);
}

BOOST_AUTO_TEST_CASE(erase_existing_and_missing)
{
  HashTable< std::string, int, StringHash > table;
  table.insert("a", 1);
  BOOST_CHECK(table.erase("a"));
  BOOST_CHECK(!table.erase("a"));
  BOOST_CHECK(table.find("a") == table.end());
  BOOST_CHECK(table.empty());
}

BOOST_AUTO_TEST_CASE(size_tracks_inserts_and_erases)
{
  HashTable< std::string, int, StringHash > table;
  BOOST_CHECK_EQUAL(table.size(), 0u);
  table.insert("a", 1);
  table.insert("b", 2);
  BOOST_CHECK_EQUAL(table.size(), 2u);
  table.erase("a");
  BOOST_CHECK_EQUAL(table.size(), 1u);
}

BOOST_AUTO_TEST_CASE(grows_past_initial_bucket_count)
{
  HashTable< std::string, int, StringHash > table;
  for (int i = 0; i < 200; ++i) {
    BOOST_CHECK(table.insert("k" + std::to_string(i), i));
  }
  BOOST_CHECK_EQUAL(table.size(), 200u);
  for (int i = 0; i < 200; ++i) {
    auto it = table.find("k" + std::to_string(i));
    BOOST_REQUIRE(it != table.end());
    BOOST_CHECK_EQUAL(it->second, i);
  }
}

BOOST_AUTO_TEST_CASE(iteration_visits_every_element_once)
{
  HashTable< std::string, int, StringHash > table;
  for (int i = 0; i < 30; ++i) {
    table.insert("k" + std::to_string(i), i);
  }
  std::size_t visited = 0;
  for (auto it = table.begin(); it != table.end(); ++it) {
    ++visited;
  }
  BOOST_CHECK_EQUAL(visited, table.size());
}
