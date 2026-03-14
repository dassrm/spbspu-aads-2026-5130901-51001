#include <cassert>
#include <functional>
#include <iostream>
#include <string>

#include "list.hpp"

namespace {

using smirnova::List;
using smirnova::LIter;
using smirnova::LCIter;

void assertThrows(std::function< void() > fn)
{
  bool threw = false;
  try {
    fn();
  } catch (const std::runtime_error&) {
    threw = true;
  }
  assert(threw);
}

void testEmpty()
{
  const List< int > lst;
  assert(lst.empty());
  assert(lst.size() == 0);
  assert(lst.begin() == lst.end());
}

void testPushAndIterate()
{
  List< int > lst;
  lst.pushBack(1);
  lst.pushBack(2);
  lst.pushBack(3);
  assert(lst.size() == 3);
  assert(lst.front() == 1);
  assert(lst.back() == 3);

  List< int > lst2;
  lst2.pushFront(3);
  lst2.pushFront(2);
  lst2.pushFront(1);
  assert(lst2.front() == 1);
  assert(lst2.back() == 3);

  const List< int > clst(lst);
  assert(clst.front() == 1);
  assert(clst.back() == 3);
}

void testPopFrontBack()
{
  List< int > lst;
  lst.pushBack(1);
  lst.pushBack(2);
  lst.pushBack(3);

  lst.popFront();
  assert(lst.front() == 2);
  assert(lst.size() == 2);

  lst.popBack();
  assert(lst.back() == 2);
  assert(lst.size() == 1);
}

void testIterators()
{
  List< int > lst;
  lst.pushBack(1);
  lst.pushBack(2);
  lst.pushBack(3);

  LIter< int > it = lst.end();
  --it;
  assert(*it == 3);
  --it;
  assert(*it == 2);
  --it;
  assert(*it == 1);
  assert(it == lst.begin());

  const LIter< int > old = it++;
  assert(*old == 1);
  assert(*it == 2);

  const LIter< int > old2 = it--;
  assert(*old2 == 2);
  assert(*it == 1);

  LCIter< int > cit(lst.begin());
  assert(*cit == 1);
  LCIter< int > cend = lst.cend();
  --cend;
  assert(*cend == 3);
}

void testInsertErase()
{
  List< int > lst;
  lst.pushBack(1);
  lst.pushBack(3);

  LIter< int > it = lst.begin();
  ++it;
  it = lst.insert(it, 2);
  assert(*it == 2);
  assert(lst.size() == 3);
  assert(lst.front() == 1);
  assert(lst.back() == 3);

  it = lst.begin();
  ++it;
  it = lst.erase(it);
  assert(*it == 3);
  assert(lst.size() == 2);
  assert(lst.front() == 1);
  assert(lst.back() == 3);
}

void testCopyMove()
{
  List< int > a;
  a.pushBack(1);
  a.pushBack(2);

  List< int > b(a);
  b.pushBack(3);
  assert(a.size() == 2);
  assert(b.size() == 3);

  List< int > c(std::move(a));
  assert(c.size() == 2);
  assert(a.empty());

  List< int > d;
  d.pushBack(99);
  d = b;
  assert(d.size() == 3);
  assert(d.front() == 1);

  List< int > e;
  e = std::move(c);
  assert(e.size() == 2);
  assert(c.empty());

  e = e;
  assert(e.size() == 2);
}

void testClearAndSwap()
{
  List< int > lst;
  lst.pushBack(1);
  lst.pushBack(2);
  lst.clear();
  assert(lst.empty());
  lst.pushBack(3);
  assert(lst.front() == 3
