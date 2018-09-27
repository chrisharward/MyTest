// STL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "catch.hpp"

#include "algorithm.h"

#include <algorithm>
#include <type_traits>
#include <numeric>
#include <memory>
#include <random>
#include <iterator>

TEST_CASE("all_of", "[all_of]")
{
   auto pred = [](char c) { return c == 'e'; };

   REQUIRE(STL::all_of(nullptr, nullptr, pred) == true);

   const char test1[] = "abcde";
   REQUIRE(STL::all_of(test1, test1, pred) == true);
   REQUIRE(STL::all_of(test1, test1 + sizeof(test1) - 1, pred) == false);

   const char test2[] = "e";
   REQUIRE(STL::all_of(test2, test2 + sizeof(test2) - 1, pred) == true);

   const char test3[] = "eeeeee";
   REQUIRE(STL::all_of(test3, test3 + sizeof(test3) - 1, pred) == true);

   const char test4[] = "eeeeeec";
   REQUIRE(STL::all_of(test4, test4 + sizeof(test4) - 1, pred) == false);

   const char test5[] = "ceeeee";
   REQUIRE(STL::all_of(test5, test5 + sizeof(test5) - 1, pred) == false);

   const char test6[] = "eeereee";
   REQUIRE(STL::all_of(test6, test6 + sizeof(test6) - 1, pred) == false);
}

TEST_CASE("any_of", "[any_of]")
{
   using STL::any_of;

   auto pred = [](int i) { return i == 42; };

   REQUIRE(any_of(nullptr, nullptr, pred) == false);

   const int testArr[] = { 1, 2, 3, 4, 5, 6 };
   REQUIRE(any_of(testArr, testArr, pred) == false);
   REQUIRE(any_of(testArr, testArr + sizeof(testArr), pred) == false);

   const int testArr1[] = { 1, 2, 3, 4, 5, 6, 42, 43, 44, 45 };
   REQUIRE(any_of(testArr1, testArr1 + sizeof(testArr1), pred) == true);

   const int testArr2[] = { 42, 42, 42 };
   REQUIRE(any_of(testArr2, testArr2 + sizeof(testArr2), pred) == true);

   const int testArr3[] = { 42, 1, 6 };
   REQUIRE(any_of(testArr3, testArr3 + sizeof(testArr3), pred) == true);
}

TEST_CASE("none_of", "[none_of]")
{
   using STL::none_of;

   auto pred = [](int i) { return i == 42; };

   REQUIRE(none_of(nullptr, nullptr, pred) == true);

   const int testArr[] = { 1, 2, 3, 4, 5, 6 };
   REQUIRE(none_of(testArr, testArr, pred) == true);
   REQUIRE(none_of(testArr, testArr + sizeof(testArr), pred) == true);

   const int testArr1[] = { 1, 2, 3, 4, 5, 6, 42, 43, 44, 45 };
   REQUIRE(none_of(testArr1, testArr1 + sizeof(testArr1), pred) == false);
}

struct TestFunctor
{
   void operator()(int &val)
   {
      val *= 2;
      count++;
   }

   TestFunctor() = default;
   TestFunctor(const TestFunctor&) = delete;
   TestFunctor& operator=(const TestFunctor&) = delete;

   TestFunctor(TestFunctor&&) = default;
   TestFunctor& operator=(TestFunctor&&) = default;

   int count = 0;
};


TEST_CASE("for_each", "[for_each]")
{
   using STL::for_each;

   int test = 0;
   TestFunctor tf;

   auto func = [&test](int &i) { test++; i *= 2; };

   tf = for_each(nullptr, nullptr, std::move(tf));

   REQUIRE(tf.count == 0);
   int testArr[] = { 1, 2, 3, 4, 5, 6 };
   tf = for_each(testArr, testArr, std::move(tf));
   REQUIRE(tf.count == 0);
   REQUIRE(*std::begin(testArr) == 1);
   REQUIRE(testArr[5] == 6);

   tf = for_each(std::begin(testArr), std::end(testArr), std::move(tf));
   REQUIRE(tf.count == 6);
   REQUIRE(*std::begin(testArr) == 2);
   REQUIRE(testArr[5] == 12);
}

TEST_CASE("for_each_n", "[for_each_n]")
{
   int count = 0;
   auto tf = [&count](int &i) { count++; i *= 2; };

   STL::for_each_n(nullptr, 0, tf);
   REQUIRE(count == 0);

   int testArr[] = { 1, 2, 3, 4, 5, 6 };
   STL::for_each_n(std::begin(testArr), 0, tf);
   REQUIRE(count == 0);

   STL::for_each_n(std::begin(testArr), 1, std::move(tf));
   REQUIRE(count == 1);
   REQUIRE(testArr[0] == 2);
   REQUIRE(testArr[1] == 2);

   STL::for_each_n(std::begin(testArr), std::extent<decltype(testArr)>::value, std::move(tf));
   REQUIRE(count == 7);
   REQUIRE(testArr[0] == 4);
   REQUIRE(testArr[1] == 4);
   REQUIRE(testArr[5] == 12);

   std::vector<int> testVec(6);
   std::iota(std::begin(testVec), std::end(testVec), 10);
      
   STL::for_each_n(std::begin(testVec), 3, std::move(tf));
   REQUIRE(count == 10);
   REQUIRE(testVec[0] == 20);
   REQUIRE(testVec[1] == 22);
   REQUIRE(testVec[2] == 24);
   REQUIRE(testVec[3] == 13);
}

TEST_CASE("count", "[count]")
{
   using STL::count;
   
   int testArr[] = { 1, 2, 3, 4, 5, 6, 1, 2, 3 };
   REQUIRE(count(std::begin(testArr), std::begin(testArr), 0) == typename std::iterator_traits<int*>::difference_type{});

   REQUIRE(count(std::begin(testArr), std::end(testArr), 0) == typename std::iterator_traits<int*>::difference_type{});
   REQUIRE(count(std::begin(testArr), std::end(testArr), 6) == typename std::iterator_traits<int*>::difference_type{1});
   REQUIRE(count(std::begin(testArr), std::end(testArr), 1) == typename std::iterator_traits<int*>::difference_type{2});
   REQUIRE(count(std::begin(testArr), std::end(testArr), 7) == typename std::iterator_traits<int*>::difference_type{});
}

TEST_CASE("count_if", "[count_if]")
{
   using STL::count_if;

   int testArr[] = { 1, 2, 3, 4, 5, 6, 1, 2, 3 };
   REQUIRE(count_if(std::begin(testArr), std::begin(testArr), [](auto &val) { return val == 6; }) == typename std::iterator_traits<int*>::difference_type{});
   REQUIRE(count_if(std::begin(testArr), std::end(testArr), [](auto &val) { return val == 6; }) == typename std::iterator_traits<int*>::difference_type{1});
   REQUIRE(count_if(std::begin(testArr), std::end(testArr), [](auto &val) { return val == 2; }) == typename std::iterator_traits<int*>::difference_type{ 2 });
   REQUIRE(count_if(std::begin(testArr), std::end(testArr), [](auto &val) { return val == 7; }) == typename std::iterator_traits<int*>::difference_type{});
}

TEST_CASE("mismatch", "[mismatch]")
{
   int testArr1[] = { 1, 2, 3, 4, 5, 6, 1, 2, 3 };
   int testArr2[] = { 1, 2, 3, 4, 5, 6, 1, 2, 3 };
   int testArr3[] = { 2 };
   int testArr4[] = { 1, 3 };
   int testArr5[] = { 1, 2, 3, 4, 5, 6, 1, 2, 4 };
   int testArr6[] = { 1, 2, 3, 4, 5, 6 };

   auto res = STL::mismatch(std::begin(testArr1), std::begin(testArr1), std::begin(testArr1));
   REQUIRE(res.first == std::begin(testArr1));
   REQUIRE(res.second == std::begin(testArr1));

   res = STL::mismatch(std::begin(testArr1), std::begin(testArr1), std::begin(testArr2));
   REQUIRE(res.first == std::begin(testArr1));
   REQUIRE(res.second == std::begin(testArr2));

   res = STL::mismatch(std::begin(testArr1), std::begin(testArr1) + 1, std::begin(testArr2));
   REQUIRE(res.first == std::begin(testArr1) + 1);
   REQUIRE(res.second == std::begin(testArr2) + 1);

   res = STL::mismatch(std::begin(testArr1), std::end(testArr1), std::begin(testArr2));
   REQUIRE(res.first == std::end(testArr1));
   REQUIRE(res.second == std::end(testArr2));

   res = STL::mismatch(std::begin(testArr1), std::end(testArr1), std::begin(testArr3));
   REQUIRE(res.first == std::begin(testArr1));
   REQUIRE(res.second == std::begin(testArr3));

   res = STL::mismatch(std::begin(testArr1), std::end(testArr1), std::begin(testArr4));
   REQUIRE(res.first == std::begin(testArr1) + 1);
   REQUIRE(res.second == std::begin(testArr4) + 1);

   res = STL::mismatch(std::begin(testArr1), std::end(testArr1), std::begin(testArr5));
   REQUIRE(res.first == std::end(testArr1) - 1);
   REQUIRE(res.second == std::end(testArr5) - 1);

   res = STL::mismatch(std::begin(testArr1), std::end(testArr1), std::begin(testArr2), std::end(testArr2));
   REQUIRE(res.first == std::end(testArr1));
   REQUIRE(res.second == std::end(testArr2));
}

TEST_CASE("equal", "[equal]")
{
   int testArr1[] = { 1, 2, 3, 4, 5, 6, 1, 2, 3 }; 
   int testArr2[] = { 1, 2, 3, 4, 5, 6, 1, 2, 3 };

   REQUIRE(STL::equal(std::begin(testArr1),
      std::begin(testArr1),
      std::begin(testArr1),
      std::begin(testArr1),
      [](const int &i1, const int &i2) {return i1 == i2; }) == true);

   REQUIRE(STL::equal(std::begin(testArr1),
      std::begin(testArr1),
      std::begin(testArr1) + 1,
      std::begin(testArr1) + 1,
      [](const int &i1, const int &i2) {return i1 == i2; }) == true);

   REQUIRE(STL::equal(std::begin(testArr1),
      std::begin(testArr1) + 1,
      std::begin(testArr1),
      std::begin(testArr1) + 1,
      [](const int &i1, const int &i2) {return i1 == i2; }) == true);

   REQUIRE(STL::equal(std::begin(testArr1),
      std::begin(testArr1) + 1,
      std::begin(testArr1) + 1,
      std::begin(testArr1) + 1,
      [](const int &i1, const int &i2) {return i1 == i2; }) == false);

   REQUIRE(STL::equal(std::begin(testArr1),
      std::begin(testArr1) + 1,
      std::begin(testArr1) + 1,
      std::begin(testArr1) + 2,
      [](const int &i1, const int &i2) {return i1 == i2; }) == false);

   REQUIRE(STL::equal(std::begin(testArr1),
      std::end(testArr1),
      std::begin(testArr2),
      std::end(testArr2),
      [](const int &i1, const int &i2) {return i1 == i2; }) == true);
}

TEST_CASE("find", "[find]")
{
   int testArr1[] = { 1, 2, 3, 4, 5, 6, 1, 2, 3 };

   REQUIRE(STL::find(std::begin(testArr1), std::begin(testArr1), 3) == std::begin(testArr1));
   REQUIRE(STL::find(std::begin(testArr1), std::end(testArr1), 7) == std::end(testArr1));
   REQUIRE(STL::find(std::begin(testArr1), std::end(testArr1), 1) == std::begin(testArr1));
   REQUIRE(STL::find(std::begin(testArr1), std::end(testArr1), 3) == std::begin(testArr1) + 2);

   REQUIRE(STL::find_if(std::begin(testArr1), std::begin(testArr1), [](const auto &val) { return val == 1; }) == std::begin(testArr1));
   REQUIRE(STL::find_if(std::begin(testArr1), std::end(testArr1), [](const auto &val) { return val == 7; }) == std::end(testArr1));
   REQUIRE(STL::find_if(std::begin(testArr1), std::end(testArr1), [](const auto &val) { return val == 1; }) == std::begin(testArr1));
   REQUIRE(STL::find_if(std::begin(testArr1), std::end(testArr1), [](const auto &val) { return val == 3; }) == std::begin(testArr1) + 2);
   REQUIRE(STL::find_if_not(std::begin(testArr1), std::end(testArr1), [](const auto &val) { return val == 1; }) == std::begin(testArr1) + 1);
}

TEST_CASE("find_end", "[find_end]")
{
   int testArr1[] = { 2, 1, 2, 3, 4, 5, 6, 1, 2, 3, 10, 1, 2, 3 };
   int testArr2[] = { 1, 2, 3 };
   int testArr3[] = { 4, 5, 6 };
   int testArr4[] = { 2, 1 };
   int testArr5[] = { 2, 1, 2, 3, 4, 5, 6, 1, 2, 3, 10, 1, 2, 3, 2, 3 };

   REQUIRE(STL::find_end(std::begin(testArr1), std::begin(testArr1), std::begin(testArr2), std::begin(testArr2)) == std::begin(testArr1));
   REQUIRE(STL::find_end(std::begin(testArr1), std::begin(testArr1), std::begin(testArr2), std::end(testArr2)) == std::begin(testArr1));
   REQUIRE(STL::find_end(std::begin(testArr1), std::end(testArr1), std::begin(testArr2), std::begin(testArr2)) == std::end(testArr1));
   REQUIRE(STL::find_end(std::begin(testArr1), std::end(testArr1), std::begin(testArr2), std::end(testArr2)) == std::begin(testArr1) + 11);
   REQUIRE(STL::find_end(std::begin(testArr1), std::end(testArr1), std::begin(testArr2), std::end(testArr2) - 1) == std::begin(testArr1) + 11);
   REQUIRE(STL::find_end(std::begin(testArr1), std::end(testArr1), std::begin(testArr3), std::end(testArr3)) == std::begin(testArr1) + 4);
   REQUIRE(STL::find_end(std::begin(testArr1), std::end(testArr1), std::begin(testArr4), std::end(testArr4)) == std::begin(testArr1));
   REQUIRE(STL::find_end(std::begin(testArr5), std::end(testArr5), std::begin(testArr2), std::end(testArr2)) == std::begin(testArr5) + 11);
   REQUIRE(STL::find_end(std::begin(testArr1), std::end(testArr1), std::begin(testArr4), std::end(testArr4), 
      [](const auto &val1, const auto &val2) { return val1 == val2; }) == std::begin(testArr1));
}

TEST_CASE("find_first_of", "[find_first_of]")
{
   int testArr1[] = { 2, 1, 2, 3, 4, 5, 6, 1, 2, 3, 10, 1, 2, 3, 11, 12 };
   int testArr2[] = { 1, 2, 3 };
   int testArr3[] = { 3, 2, 1 };
   int testArr4[] = { 2, 1 };
   int testArr5[] = { 11, 12 };

   REQUIRE(STL::find_first_of(std::begin(testArr1), std::begin(testArr1), std::begin(testArr2), std::end(testArr2)) == std::begin(testArr1));
   REQUIRE(STL::find_first_of(std::begin(testArr1), std::end(testArr1), std::begin(testArr2), std::begin(testArr2)) == std::end(testArr1));
   REQUIRE(STL::find_first_of(std::begin(testArr1), std::end(testArr1), std::begin(testArr3), std::end(testArr3)) == std::end(testArr1));
   REQUIRE(STL::find_first_of(std::begin(testArr1), std::end(testArr1), std::begin(testArr2), std::end(testArr2)) == std::begin(testArr1) + 1);
   REQUIRE(STL::find_first_of(std::begin(testArr1), std::end(testArr1), std::begin(testArr4), std::end(testArr4)) == std::begin(testArr1));
   REQUIRE(STL::find_first_of(std::begin(testArr1), std::end(testArr1), std::begin(testArr5), std::end(testArr5)) == std::begin(testArr1) + 14);
   REQUIRE(STL::search(std::begin(testArr1), std::end(testArr1), std::begin(testArr5), std::end(testArr5)) == std::begin(testArr1) + 14);
}


TEST_CASE("adjacent_find", "[adjacent_find]")
{
   int testArr1[] = { 2, 1, 2, 3, 4, 5, 6, 1, 2, 3, 10, 1, 2, 3, 11, 12 };
   int testArr2[] = { 1 };
   int testArr3[] = { 3, 3, 2, 1 };
   int testArr4[] = { 3, 2, 1, 1 };

   REQUIRE(STL::adjacent_find(std::begin(testArr1), std::begin(testArr1)) == std::begin(testArr1));
   REQUIRE(STL::adjacent_find(std::begin(testArr1), std::end(testArr1)) == std::end(testArr1));
   REQUIRE(STL::adjacent_find(std::begin(testArr2), std::end(testArr2)) == std::end(testArr2));
   REQUIRE(STL::adjacent_find(std::begin(testArr2), std::end(testArr2)) == std::end(testArr2));
   REQUIRE(STL::adjacent_find(std::begin(testArr3), std::end(testArr3)) == std::begin(testArr3));
   REQUIRE(STL::adjacent_find(std::begin(testArr4), std::end(testArr4)) == std::begin(testArr4) + 2);
}

TEST_CASE("search_n", "[search_n]")
{
   int testArr1[] = { 2, 1, 2, 3, 4, 5, 6, 1, 2, 3, 10, 1, 2, 3, 11, 12 };
   int testArr2[] = { 2, 2, 1, 2, 3, 4, 5, 6, 1, 2, 3, 10, 1, 2, 3, 11, 12, 12 };

   REQUIRE(STL::search_n(std::begin(testArr1), std::begin(testArr1), 1, 1) == std::begin(testArr1));
   REQUIRE(STL::search_n(std::begin(testArr1), std::end(testArr1), 1, 13) == std::end(testArr1));
   REQUIRE(STL::search_n(std::begin(testArr1), std::end(testArr1), 1, 2) == std::begin(testArr1));
   REQUIRE(STL::search_n(std::begin(testArr1), std::end(testArr1), 0, 2) == std::end(testArr1));
   REQUIRE(STL::search_n(std::begin(testArr1), std::end(testArr1), 2, 2) == std::end(testArr1));
   REQUIRE(STL::search_n(std::begin(testArr2), std::end(testArr2), 2, 2) == std::begin(testArr2));
   REQUIRE(STL::search_n(std::begin(testArr2), std::end(testArr2), 2, 12) == std::begin(testArr2) + 16);
   REQUIRE(STL::search_n(std::begin(testArr2), std::end(testArr2), 3, 12) == std::end(testArr2));
}


TEST_CASE("copy", "[copy]")
{
   int testArr1[] = { 1, 2, 3, 4, 5, 6 };
   std::vector<int> testVec(6UL, 0x00);
   std::vector<int> testVecExp(6UL, 0x00);

   int testArr2[] = { 3, 4, 5, 6, 7, 8 };
   int testArr2exp[] = { 3, 5, 7 };

   REQUIRE(STL::copy(std::begin(testArr1), std::begin(testArr1), std::begin(testVec)) == std::begin(testVec));
   REQUIRE(std::equal(std::begin(testVec), std::end(testVec), std::begin(testVecExp), std::end(testVecExp)));
   REQUIRE(STL::copy(std::begin(testArr1), std::end(testArr1), std::begin(testVec)) == std::end(testVec));
   REQUIRE(std::equal(std::begin(testVec), std::end(testVec), std::begin(testArr1), std::end(testArr1)));

   REQUIRE(STL::copy_if(std::begin(testArr2), std::end(testArr2), std::begin(testVec), [](const auto &val) { return val % 2 == 1; }) == std::begin(testVec) + 3);
   REQUIRE(std::equal(std::begin(testVec), std::begin(testVec) + 3, std::begin(testArr2exp), std::end(testArr2exp)));
}

TEST_CASE("copy_n", "[copy_n]")
{
   int testArr1[] = { 1, 2, 3, 4, 5, 6 };
   std::vector<int> testVec;

   STL::copy_n(std::begin(testArr1), 0, std::back_inserter(testVec));
   REQUIRE(testVec.size() == 0);

   STL::copy_n(std::begin(testArr1), 1, std::back_inserter(testVec));

   REQUIRE(testVec == std::vector<int>{ 1 });

   testVec.resize(3);
   REQUIRE(STL::copy_n(std::begin(testArr1), 2, std::begin(testVec) + 1) == std::end(testVec));
   REQUIRE(testVec == std::vector<int>({ 1, 1, 2 }));

   REQUIRE(STL::copy_n(std::begin(testArr1), 3, std::begin(testVec)) == std::end(testVec));
   REQUIRE(testVec == std::vector<int>({ 1, 2, 3 }));
}

TEST_CASE("copy_backwards", "[copy_backwards]")
{
   int testArr1[] = { 1, 2, 3, 4, 5, 6 };
   std::vector<int> testVec;

   testVec.resize(std::extent<decltype(testArr1)>::value);
   REQUIRE(STL::copy_backwards(std::begin(testArr1), std::begin(testArr1), std::begin(testVec)) == std::begin(testVec));

   REQUIRE(STL::copy_backwards(std::begin(testArr1), std::end(testArr1), std::end(testVec)) == std::begin(testVec));

   REQUIRE(testVec == std::vector<int>({1, 2, 3, 4, 5, 6}));
}

TEST_CASE("move", "[move]")
{
   std::vector<std::unique_ptr<int>> testArr1;
   testArr1.emplace_back(std::make_unique<int>(1));
   testArr1.emplace_back(std::make_unique<int>(2));
   testArr1.emplace_back(std::make_unique<int>(3));
   testArr1.emplace_back(std::make_unique<int>(4));
   testArr1.emplace_back(std::make_unique<int>(5));
   testArr1.emplace_back(std::make_unique<int>(6));

   std::vector<std::unique_ptr<int>> testVec(testArr1.size());

   REQUIRE(STL::move(std::begin(testArr1), std::begin(testArr1), std::begin(testVec)) == std::begin(testVec));
   REQUIRE(STL::move(std::begin(testArr1), std::end(testArr1), std::begin(testVec)) == std::end(testVec));

   std::vector<std::unique_ptr<int>> testArrExp;
   testArrExp.emplace_back(std::make_unique<int>(1));
   testArrExp.emplace_back(std::make_unique<int>(2));
   testArrExp.emplace_back(std::make_unique<int>(3));
   testArrExp.emplace_back(std::make_unique<int>(4));
   testArrExp.emplace_back(std::make_unique<int>(5));
   testArrExp.emplace_back(std::make_unique<int>(6));

   REQUIRE(*testArrExp[0] == *testVec[0]);
   REQUIRE(*testArrExp[1] == *testVec[1]);
   REQUIRE(*testArrExp[2] == *testVec[2]);
   REQUIRE(*testArrExp[3] == *testVec[3]);
   REQUIRE(*testArrExp[4] == *testVec[4]);
   REQUIRE(*testArrExp[5] == *testVec[5]);

   REQUIRE(testArr1[0] == nullptr);
   REQUIRE(testArr1[1] == nullptr);
   REQUIRE(testArr1[2] == nullptr);
   REQUIRE(testArr1[3] == nullptr);
   REQUIRE(testArr1[4] == nullptr);
   REQUIRE(testArr1[5] == nullptr);
}

TEST_CASE("move_backwards", "[move_backwards]")
{
   std::vector<std::unique_ptr<int>> testArr1;
   testArr1.emplace_back(std::make_unique<int>(1));
   testArr1.emplace_back(std::make_unique<int>(2));
   testArr1.emplace_back(std::make_unique<int>(3));
   testArr1.emplace_back(std::make_unique<int>(4));
   testArr1.emplace_back(std::make_unique<int>(5));
   testArr1.emplace_back(std::make_unique<int>(6));

   std::vector<std::unique_ptr<int>> testVec(testArr1.size());
   REQUIRE(STL::move_backwards(std::begin(testArr1), std::begin(testArr1), std::end(testVec)) == std::end(testVec));

   REQUIRE(STL::move_backwards(std::begin(testArr1) + 5, std::end(testArr1), std::end(testVec)) == std::begin(testVec) + 5);

   REQUIRE(testVec[0] == nullptr);
   REQUIRE(testVec[1] == nullptr);
   REQUIRE(testVec[2] == nullptr);
   REQUIRE(testVec[3] == nullptr);
   REQUIRE(testVec[4] == nullptr);
   REQUIRE(*testVec[5] == 6);

   REQUIRE(*testArr1[0] == 1);
   REQUIRE(*testArr1[1] == 2);
   REQUIRE(*testArr1[2] == 3);
   REQUIRE(*testArr1[3] == 4);
   REQUIRE(*testArr1[4] == 5);
   REQUIRE(testArr1[5] == nullptr);

   REQUIRE(STL::move_backwards(std::begin(testArr1) + 3, std::begin(testArr1) + 5, std::begin(testVec) + 5) == std::begin(testVec) + 3);

   REQUIRE(testVec[0] == nullptr);
   REQUIRE(testVec[1] == nullptr);
   REQUIRE(testVec[2] == nullptr);
   REQUIRE(*testVec[3] == 4);
   REQUIRE(*testVec[4] == 5);
   REQUIRE(*testVec[5] == 6);

   REQUIRE(*testArr1[0] == 1);
   REQUIRE(*testArr1[1] == 2);
   REQUIRE(*testArr1[2] == 3);
   REQUIRE(testArr1[3] == nullptr);
   REQUIRE(testArr1[4] == nullptr);
   REQUIRE(testArr1[5] == nullptr);
}

TEST_CASE("transform", "[transform]")
{
   int testArr1[] = { 1, 2, 3, 4, 5, 6 };
   std::vector<int> testVec;
   std::vector<int> testVec2;

   testVec.resize(std::extent<decltype(testArr1)>::value);
   testVec2.resize(std::extent<decltype(testArr1)>::value);

   REQUIRE(STL::transform(std::begin(testArr1), std::begin(testArr1), std::begin(testVec), [](const auto &val) { return 2 * val; }) == std::begin(testVec));
   REQUIRE(STL::transform(std::begin(testArr1), std::end(testArr1), std::begin(testVec), [](const auto &val) { return 2 * val; }) == std::end(testVec));

   REQUIRE(testVec == std::vector<int>({2, 4, 6, 8, 10, 12}));

   REQUIRE(STL::transform(std::begin(testArr1), std::begin(testArr1), std::begin(testVec), std::begin(testVec2), [](const auto &val1, const auto &val2) { return val1 * val2; }) == std::begin(testVec2));
   REQUIRE(STL::transform(std::begin(testArr1), std::end(testArr1), std::begin(testVec), std::begin(testVec2), [](const auto &val1, const auto &val2) { return val1 * val2; }) == std::end(testVec2));

   REQUIRE(testVec2 == std::vector<int>({ 2, 8, 18, 32, 50, 72 }));
}

TEST_CASE("generate", "[generate]")
{
   std::vector<int> testVec(6, 0);
   std::vector<int> expVec(6, 0);
   
   STL::generate(std::begin(testVec), std::begin(testVec), [count = 0]() mutable { return ++count; });
   REQUIRE(expVec == testVec);
      
   std::iota(expVec.begin(), expVec.end(), 1);
   STL::generate(std::begin(testVec), std::end(testVec), [count = 0]() mutable { return ++count; });
   REQUIRE(expVec == testVec);

   std::vector<int> testVec2(6, 0);
   std::vector<int> expVec2(6, 0);

   auto next = STL::generate_n(std::begin(testVec2), 0u, [count = 0]() mutable { return ++count; });
   REQUIRE(expVec2 == testVec2);
   REQUIRE(*next == 0);

   testVec2[1] = 10;
   expVec2[0] = 1;
   expVec2[1] = 10;
   
   next = STL::generate_n(std::begin(testVec2), 1u, [count = 0]() mutable { return ++count; });
   REQUIRE(expVec2 == testVec2);
   REQUIRE(*next == 10);

   std::iota(expVec2.begin(), expVec2.begin() + 4, 1);
   next = STL::generate_n(std::begin(testVec2), 4u, [count = 0]() mutable { return ++count; });
   REQUIRE(expVec2 == testVec2);
   REQUIRE(*next == 0);

   std::iota(expVec2.begin(), expVec2.end(), 1);
   next = STL::generate_n(std::begin(testVec2), 6u, [count = 0]() mutable { return ++count; });
   REQUIRE(expVec2 == testVec2);
   REQUIRE(next == std::end(testVec2));
}

TEST_CASE("remove", "[remove]")
{
   std::vector<int> testVec(6, 0);
   std::vector<int> expVec(6, 0);

   std::iota(testVec.begin(), testVec.end(), 1);
   std::iota(expVec.begin(), expVec.end(), 2);
   expVec[5] = 6;

   REQUIRE(STL::remove(testVec.begin(), testVec.end(), 1) == testVec.begin() + 5);
   REQUIRE(testVec == expVec);
}

TEST_CASE("remove_copy", "[remove_copy]")
{
   std::vector<int> testVec(6, 0);
   std::vector<int> testTrgVec(6, 0);

   std::vector<int> expVec = { 2, 3, 4, 5, 6, 0};

   std::iota(testVec.begin(), testVec.end(), 1);
   REQUIRE(STL::remove_copy(testVec.begin(), testVec.end(), testTrgVec.begin(), 1) == testTrgVec.begin() + 5);
   REQUIRE(testTrgVec == expVec);

   std::vector<int> testTrgVec2(6, 0);
   std::vector<int> expVec2 = { 2, 4, 6, 0, 0, 0 };
   REQUIRE(STL::remove_copy_if(testVec.begin(), testVec.end(), testTrgVec2.begin(), [](auto i) { return i % 2 == 0; }) == testTrgVec2.begin() + 3);
   REQUIRE(testTrgVec2 == expVec2);
}

TEST_CASE("replace", "[replace]")
{
   std::vector<int> srcVec(6, 0);
   std::iota(srcVec.begin(), srcVec.end(), 1);

   std::vector<int> trgVec = { 1, 2, 3, 42, 5, 6 };
   STL::replace(srcVec.begin(), srcVec.end(), 4, 42);
   REQUIRE(trgVec == srcVec);

   std::iota(srcVec.begin(), srcVec.end(), 1);
   STL::replace_if(srcVec.begin(), srcVec.end(), [](auto i) { return i == 4; }, 42);
   REQUIRE(trgVec == srcVec);

   std::iota(srcVec.begin(), srcVec.end(), 1);
   auto res = STL::replace_copy(srcVec.begin(), srcVec.end(), trgVec.begin(), 4, 42);
   REQUIRE(trgVec == srcVec);
   REQUIRE(trgVec.end() == res);

   std::iota(srcVec.begin(), srcVec.end(), 1);
   res = STL::replace_copy_if(srcVec.begin(), srcVec.end(), trgVec.begin(), [](auto v) { return v == 4; }, 42);
   REQUIRE(trgVec == srcVec);
   REQUIRE(trgVec.end() == res);
}

class copyable
{
public:
   copyable(int i) : i_(i) {}
   ~copyable() = default;

   copyable& operator= (const copyable& other) = default;
   copyable(const copyable& other) = default;

   copyable& operator= (copyable&& other) = delete;
   copyable(copyable&& other) = delete;

   int i_;
};

class moveable
{
public:
   moveable() = default;
   moveable(int i) : i_(i) {}
   ~moveable() = default;

   moveable& operator= (const moveable& other) = delete;
   moveable(const moveable& other) = delete;

   moveable& operator=(moveable&& other) = default;
   moveable(moveable&& other) = default;

   bool operator==(const moveable &other) const
   {
      return this->i_ == other.i_;
   }

   int i_;
};

TEST_CASE("swap", "[swap]")
{
   moveable m1(1), m2(2);

   STL::swap(m1, m2);

   REQUIRE(m1.i_ == 2);
   REQUIRE(m2.i_ == 1);

   copyable c1(1), c2(2);

   STL::swap(c1, c2);

   REQUIRE(c1.i_ == 2);
   REQUIRE(c2.i_ == 1);

   int i1 = 1, i2 = 2;

   STL::swap(i1, i2);

   REQUIRE(i1 == 2);
   REQUIRE(i2 == 1);

   moveable src[] = { 1, 2, 3, 4, 5 };
   moveable trg[5];
   moveable exp[] = { 1, 2, 3, 4, 5 };

   STL::swap(src, trg);
   REQUIRE(std::equal(std::begin(trg), std::end(trg), std::begin(exp), std::end(exp)));

   std::vector<moveable> srcv(5), trgv(5), expv1(5), expv2(5);
   std::iota(srcv.begin(), srcv.end(), 1);
   std::iota(trgv.begin(), trgv.end(), 6);
   std::iota(expv1.begin(), expv1.end(), 1);
   std::iota(expv2.begin(), expv2.end(), 6);

   STL::swap_ranges(srcv.begin(), srcv.end(), trgv.begin(), trgv.end());
   REQUIRE(srcv == expv2);
   REQUIRE(trgv == expv1);
}

TEST_CASE("reverse", "[reverse]")
{
   std::vector<int> src = { 1 };
   std::vector<int> exp = { 1 };

   STL::reverse(src.begin(), src.end());
   REQUIRE(src == exp);

   std::vector<int> srcv = { 1, 2, 3, 4, 5 };
   std::vector<int> expv = { 5, 4, 3, 2, 1 };

   STL::reverse(srcv.begin(), srcv.end());
   REQUIRE(srcv == expv);

   std::vector<int> srcv2 = { 1, 2, 3, 4, 5 };
   std::vector<int> trg(5);

   STL::reverse_copy(srcv2.begin(), srcv2.end(), trg.begin());
   REQUIRE(expv == trg);
}

TEST_CASE("rotate", "[rotate]")
{
   std::vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
   std::vector<int> exp = { 4, 5, 6, 7, 8, 9, 10, 1, 2, 3 };

   auto res = STL::rotate(vec.begin(), vec.begin() + 3, vec.end());

   REQUIRE(*res == 1);
   REQUIRE(vec == exp);

   std::iota(vec.begin(), vec.end(), 1);

   std::vector<int> trg(10);

   auto res2 = STL::rotate_copy(vec.begin(), vec.begin() + 3, vec.end(), trg.begin());
   REQUIRE(res2 == trg.end());
   REQUIRE(trg == exp);
}

TEST_CASE("unique", "[unique]")
{
   std::vector<int> t1 = { 1, 2, 3, 4, 5 };
   std::vector<int> e1 = { 1, 2, 3, 4, 5 };

   auto p = [](auto val1, auto val2) { return val1 == val2; };

   REQUIRE(t1.end() == STL::unique(t1.begin(), t1.end(), p));
   REQUIRE(e1 == t1);

   std::vector<int> t2 = { 1, 1, 2, 3, 4, 5 };
   std::vector<int> e2 = { 1, 2, 3, 4, 5, 1 };

   auto res = STL::unique(t2.begin(), t2.end(), p);
   REQUIRE(res == t2.begin() + 5);
   REQUIRE(e2 == t2);

   std::vector<int> t3 = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
   std::vector<int> e3 = { 1, 2, 3, 4, 5, 5, 4, 3, 2, 1 };

   res = STL::unique(t3.begin(), t3.end(), p);
   REQUIRE(res == t3.begin() + 5);
   REQUIRE(e3 == t3);

   std::vector<std::string> t4 = { "1", "2", "3", "4", "5" };
   REQUIRE(t4.end() == STL::unique(t4.begin(), t4.end(), p));
}

#include <chrono>
#include <iostream>

TEST_CASE("partition", "[partition]")
{
   std::vector<int> t0;

   auto p = [](auto val) { return val < 3; };
   REQUIRE(t0.end() == STL::partition(t0.begin(), t0.end(), p));

   std::vector<int> t1 = { 1 };
   REQUIRE(t1.end() == STL::partition(t1.begin(), t1.end(), p));

   std::vector<int> t2 = { 5 };
   REQUIRE(t2.begin() == STL::partition(t2.begin(), t2.end(), p));

   std::vector<int> t3 = { 5, 4, 3, 2, 1 };
   std::vector<int> e3 = { 2, 1, 3, 5, 4 };
   REQUIRE(t3.begin() + 2 == STL::partition(t3.begin(), t3.end(), p));
   REQUIRE(t3 == e3);

   std::vector<int> t4 = { 5, 4, 3, 2, 1 };
   std::vector<int> e4 = { 2, 1, 5, 4, 3 };
   REQUIRE(t4.begin() + 2 == STL::stable_partition(t4.begin(), t4.end(), p));
   REQUIRE(t4 == e4);

   std::vector<int> t5 = { 1, 2, 3, 4, 5, 10, 1, 2, 4, 5 };
   std::vector<int> e5 = { 1, 2, 1, 2, 3, 4, 5, 10, 4, 5 };
   REQUIRE(t5.begin() + 4 == STL::stable_partition(t5.begin(), t5.end(), p));
   REQUIRE(t5 == e5);

   std::vector<int> t6 = { 1, 2, 3, 4, 5, 10, 1, 2, 4, 5 };
   std::vector<int> e6 = { 1, 2, 1, 2, 3, 4, 5, 10, 4, 5 };
   REQUIRE(t6.begin() + 4 == STL::stable_partition_rec(t6.begin(), t6.end(), p));
   REQUIRE(t6 == e6);

   std::vector<int> t7(100000);

   // First create an instance of an engine.
   std::random_device rnd_device;
   // Specify the engine and distribution.
   std::mt19937 mersenne_engine(rnd_device());
   std::uniform_int_distribution<int> dist(1, 50000);

   auto gen = std::bind(dist, mersenne_engine);
   generate(std::begin(t7), std::end(t7), gen);

   
   auto start = std::chrono::system_clock::now();
   auto res1 = STL::stable_partition_rec(t7.begin(), t7.end(), p);
   auto end = std::chrono::system_clock::now();
   auto dur = end - start;
   //std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count() << std::endl;

   std::vector<int> t9 = t7;
   auto res3 = STL::stable_partition_rec(t9.begin(), t9.end(), p);

   REQUIRE(t7 == t9);
   REQUIRE(*res1 == *res3);
   
   std::vector<int> t8 = t7;
   start = std::chrono::system_clock::now();

   auto res2 = STL::stable_partition(t8.begin(), t8.end(), p);

   end = std::chrono::system_clock::now();
   dur = end - start;
   //std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count() << std::endl;

   std::vector<int> t10 = t8;

   auto res4 = STL::stable_partition(t10.begin(), t10.end(), p);

   REQUIRE(*res2 == *res4);
   REQUIRE(t8 == t10);
   REQUIRE(t7 == t8);

   REQUIRE(STL::is_partitioned(std::begin(t7), std::end(t7), p));
   REQUIRE(STL::is_partitioned(std::begin(t8), std::end(t8), p));
   REQUIRE(STL::is_partitioned(std::begin(t9), std::end(t9), p));
   REQUIRE(STL::is_partitioned(std::begin(t10), std::end(t10), p));
}

TEST_CASE("heap", "[heap]")
{
   std::vector<int> v{ 3, 1, 4, 1, 5, 9 };
   
   make_heap(v.begin(), v.end());

   REQUIRE(v == std::vector<int>({ 9, 5, 4, 1, 1, 3 }));

   v.push_back(1);
   push_heap(v.begin(), v.end());

   REQUIRE(v == std::vector<int>({ 9, 5, 4, 1, 1, 3, 1 }));

   v.push_back(11);
   push_heap(v.begin(), v.end());

   REQUIRE(v == std::vector<int>({ 11, 9, 4, 5, 1, 3, 1, 1 }));

   v.push_back(6);
   push_heap(v.begin(), v.end());

   REQUIRE(v == std::vector<int>({ 11, 9, 4, 6, 1, 3, 1, 1, 5 }));
   
   pop_heap(v.begin(), v.end());

   REQUIRE(v == std::vector<int>({ 9, 6, 4, 5, 1, 3, 1, 1, 11 }));
   v.pop_back();

   pop_heap(v.begin(), v.end());

   REQUIRE(v == std::vector<int>({ 6, 5, 4, 1, 1, 3, 1, 9 }));
   v.pop_back();

   sort_heap(v.begin(), v.end());
   REQUIRE(v == std::vector<int>({ 1, 1, 1, 3, 4, 5, 6 }));

   REQUIRE(!is_heap(v.begin(), v.end()));

   v = { 11, 9, 4, 5, 1, 3, 1, 1 };
   REQUIRE(is_heap(v.begin(), v.end()));
   v = { 11, 9, 4, 6, 1, 3, 1, 1, 5 };
   REQUIRE(is_heap(v.begin(), v.end()));
   v = { 9, 6, 4, 5, 1, 3, 1, 1, 11 };
   REQUIRE(!is_heap(v.begin(), v.end()));
}

TEST_CASE("sort", "[sort]")
{
   std::vector<int> v{ 1, 3, 5, 6, 10, 18, 19, 24 };
   REQUIRE(is_sorted(v.begin(), v.end()));

   v = { 1, 17, 3, 5, 6, 10, 18, 19, 24 };
   REQUIRE(!is_sorted(v.begin(), v.end()));

   STL::sort(v.begin(), v.end());
   REQUIRE(is_sorted(v.begin(), v.end()));

   REQUIRE(STL::lower_bound(std::begin(v), std::end(v), 7, std::less<int>()) == v.begin() + 4);
   REQUIRE(STL::lower_bound(std::begin(v), std::end(v), 8, std::less<int>()) == v.begin() + 4);
   REQUIRE(STL::lower_bound(std::begin(v), std::end(v), 6, std::less<int>()) == v.begin() + 3);

   std::vector<int> v0;
   REQUIRE(STL::lower_bound(std::begin(v0), std::end(v0), 0, std::less<int>()) == v0.end());
   REQUIRE(STL::upper_bound(std::begin(v0), std::end(v0), 0, std::less<int>()) == v0.end());

   std::vector<int> v1 = { 1 };
   REQUIRE(STL::lower_bound(std::begin(v1), std::end(v1), 0, std::less<int>()) == v1.begin());
   REQUIRE(STL::lower_bound(std::begin(v1), std::end(v1), 1, std::less<int>()) == v1.begin());
   REQUIRE(STL::lower_bound(std::begin(v1), std::end(v1), 2, std::less<int>()) == v1.end());

   REQUIRE(STL::upper_bound(std::begin(v1), std::end(v1), 0, std::less<int>()) == v1.begin());
   REQUIRE(STL::upper_bound(std::begin(v1), std::end(v1), 1, std::less<int>()) == v1.end());
   REQUIRE(STL::lower_bound(std::begin(v1), std::end(v1), 2, std::less<int>()) == v1.end());
   
   std::vector<int> v2 = { 1, 3 };
   REQUIRE(STL::lower_bound(std::begin(v2), std::end(v2), 0) == v2.begin());
   REQUIRE(STL::lower_bound(std::begin(v2), std::end(v2), 1) == v2.begin());
   REQUIRE(STL::lower_bound(std::begin(v2), std::end(v2), 2) == v2.begin() + 1);
   REQUIRE(STL::lower_bound(std::begin(v2), std::end(v2), 3) == v2.begin() + 1);
   REQUIRE(STL::lower_bound(std::begin(v2), std::end(v2), 4) == v2.end());

   REQUIRE(STL::upper_bound(std::begin(v2), std::end(v2), 0) == v2.begin());
   REQUIRE(STL::upper_bound(std::begin(v2), std::end(v2), 1) == v2.begin() + 1);
   REQUIRE(STL::upper_bound(std::begin(v2), std::end(v2), 2) == v2.begin() + 1);
   REQUIRE(STL::upper_bound(std::begin(v2), std::end(v2), 3) == v2.end());
   REQUIRE(STL::upper_bound(std::begin(v2), std::end(v2), 4) == v2.end());

   std::vector<int> v3 = { 1, 3, 5 };
   REQUIRE(STL::lower_bound(std::begin(v3), std::end(v3), 0) == v3.begin());
   REQUIRE(STL::lower_bound(std::begin(v3), std::end(v3), 1) == v3.begin());
   REQUIRE(STL::lower_bound(std::begin(v3), std::end(v3), 2) == v3.begin() + 1);
   REQUIRE(STL::lower_bound(std::begin(v3), std::end(v3), 3) == v3.begin() + 1);
   REQUIRE(STL::lower_bound(std::begin(v3), std::end(v3), 4) == v3.begin() + 2);
   REQUIRE(STL::lower_bound(std::begin(v3), std::end(v3), 5) == v3.begin() + 2);
   REQUIRE(STL::lower_bound(std::begin(v3), std::end(v3), 6) == v3.end());

   REQUIRE(STL::upper_bound(std::begin(v3), std::end(v3), 0) == v3.begin());
   REQUIRE(STL::upper_bound(std::begin(v3), std::end(v3), 1) == v3.begin() + 1);
   REQUIRE(STL::upper_bound(std::begin(v3), std::end(v3), 2) == v3.begin() + 1);
   REQUIRE(STL::upper_bound(std::begin(v3), std::end(v3), 3) == v3.begin() + 2);
   REQUIRE(STL::upper_bound(std::begin(v3), std::end(v3), 4) == v3.begin() + 2);
   REQUIRE(STL::upper_bound(std::begin(v3), std::end(v3), 5) == v3.end());
   REQUIRE(STL::upper_bound(std::begin(v3), std::end(v3), 6) == v3.end());

   std::vector<int> v4 = { 1, 3, 3, 5 };
   REQUIRE(STL::lower_bound(std::begin(v4), std::end(v4), 0) == v4.begin());
   REQUIRE(STL::lower_bound(std::begin(v4), std::end(v4), 1) == v4.begin());
   REQUIRE(STL::lower_bound(std::begin(v4), std::end(v4), 2) == v4.begin() + 1);
   REQUIRE(STL::lower_bound(std::begin(v4), std::end(v4), 3) == v4.begin() + 1);
   REQUIRE(STL::lower_bound(std::begin(v4), std::end(v4), 4) == v4.begin() + 3);
   REQUIRE(STL::lower_bound(std::begin(v4), std::end(v4), 5) == v4.begin() + 3);
   REQUIRE(STL::lower_bound(std::begin(v4), std::end(v4), 6) == v4.end());

   REQUIRE(STL::upper_bound(std::begin(v4), std::end(v4), 0) == v4.begin());
   REQUIRE(STL::upper_bound(std::begin(v4), std::end(v4), 1) == v4.begin() + 1);
   REQUIRE(STL::upper_bound(std::begin(v4), std::end(v4), 2) == v4.begin() + 1);
   REQUIRE(STL::upper_bound(std::begin(v4), std::end(v4), 3) == v4.begin() + 3);
   REQUIRE(STL::upper_bound(std::begin(v4), std::end(v4), 4) == v4.begin() + 3);
   REQUIRE(STL::upper_bound(std::begin(v4), std::end(v4), 5) == v4.end());
   REQUIRE(STL::upper_bound(std::begin(v4), std::end(v4), 6) == v4.end());
}

TEST_CASE("merge", "[merge]")
{
   std::vector<int> v00;
   std::vector<int> v01;
   std::vector<int> v02;

   REQUIRE(STL::merge(std::begin(v00), std::end(v00), std::begin(v01), std::end(v01), std::begin(v02)) == v02.end());

   std::vector<int> v10 = { 1, 3 };
   std::vector<int> v11 = { 0, 2, 4, 5 };
   std::vector<int> v12(6, 0);

   REQUIRE(STL::merge(std::begin(v10), std::end(v10), std::begin(v11), std::end(v11), std::begin(v12)) == v12.end());
   REQUIRE(v12 == std::vector<int>({0, 1, 2, 3, 4, 5}));

   std::vector<int> v20 = { 1, 3, 5, 7, 2, 4, 6 };
   STL::inplace_merge(std::begin(v20), std::begin(v20) + 4, std::end(v20));
   REQUIRE(v20 == std::vector<int>({ 1, 2, 3, 4, 5, 6, 7 }));

   std::vector<int> v30 = { 1, 3, 5, 7, 2, 4, 6, 8 };
   STL::inplace_merge(std::begin(v30), std::begin(v30) + 4, std::end(v30));
   REQUIRE(v30 == std::vector<int>({ 1, 2, 3, 4, 5, 6, 7, 8 }));

   std::vector<int> v40 = { 1, 3 };
   STL::inplace_merge(std::begin(v40), std::begin(v40) + 1, std::end(v40));
   REQUIRE(v40 == std::vector<int>({ 1, 3 }));

   std::vector<int> v50 = { 1, 2, 3, 4, 7, 8, 9, 10 };
   STL::inplace_merge(std::begin(v50), std::begin(v50) + 4, std::end(v50));
   REQUIRE(v50 == std::vector<int>({ 1, 2, 3, 4, 7, 8, 9, 10 }));

   std::vector<int> v60 = { 7, 8, 9, 10, 1, 2, 3, 4 };
   STL::inplace_merge(std::begin(v60), std::begin(v60) + 4, std::end(v60));
   REQUIRE(v60 == std::vector<int>({ 1, 2, 3, 4, 7, 8, 9, 10 }));

   std::vector<int> v70 = { 1, 2, 3, 4, 5, 6, 7, 8 };
   std::vector<int> v71;
   std::vector<int> v72 = { 9, 10 };
   std::vector<int> v73 = { 1, 2, 9 };
   std::vector<int> v74 = { 1, 2 };
   std::vector<int> v75 = { 1 };
   std::vector<int> v76 = { 1 };

   REQUIRE(STL::includes(std::begin(v70), std::end(v70), std::begin(v71), std::end(v71)));
   REQUIRE(!STL::includes(std::begin(v70), std::end(v70), std::begin(v72), std::end(v72)));
   REQUIRE(!STL::includes(std::begin(v70), std::end(v70), std::begin(v73), std::end(v73)));
   REQUIRE(STL::includes(std::begin(v70), std::end(v70), std::begin(v74), std::end(v74)));
   REQUIRE(STL::includes(std::begin(v70), std::end(v70), std::begin(v75), std::end(v75)));
   REQUIRE(STL::includes(std::begin(v76), std::end(v76), std::begin(v75), std::end(v75)));
   REQUIRE(!STL::includes(std::begin(v76), std::end(v76), std::begin(v74), std::end(v74)));

   std::vector<int> v80 = { 1, 2, 3, 4, 5, 6 };
   std::vector<int> v81 = { 2, 4, 6 };
   std::vector<int> v82(3);

   auto it = STL::set_intersection(std::begin(v80), std::end(v80), std::begin(v81), std::end(v81),
      std::begin(v82));

   REQUIRE(v82 == v81);
   REQUIRE(it == std::end(v82));

   std::vector<int> v90 = { 1, 2, 3, 4, 5, 6 };
   std::vector<int> v91 = { 2, 4, 6, 8, 10, 12 };
   std::vector<int> v92 = { 1, 3, 5, 8, 10, 12 };
   std::vector<int> v93(6);

   it = STL::set_symmetric_difference(std::begin(v90), std::end(v90), std::begin(v91), std::end(v91),
      std::begin(v93));

   REQUIRE(v92 == v93);
   REQUIRE(it == std::end(v93));

   std::vector<int> v100 = { 1, 2, 3, 4, 5, 6 };
   std::vector<int> v101 = { 2, 4, 6, 8, 10, 12 };
   std::vector<int> v102 = { 1, 2, 3, 4, 5, 6, 8, 10, 12 };
   std::vector<int> v103(9);

   it = STL::set_union(std::begin(v100), std::end(v100), std::begin(v101), std::end(v101),
      std::begin(v103));

   REQUIRE(v102 == v103);
   REQUIRE(it == std::end(v103));
}

TEST_CASE("accumulate", "[accumulate]")
{
   std::vector<int> v0 = { 1, 2, 3, 4, 5, 6 };

   auto res = STL::accumulate(std::begin(v0), std::end(v0), 0);
   REQUIRE(res == 21);

   res = STL::accumulate(std::begin(v0), std::begin(v0), 3);
   REQUIRE(res == 3);

   std::vector<int> v1 = { 2, 2, 2, 2, 2, 2 };

   res = STL::inner_product(std::begin(v0), std::end(v0), std::begin(v1), 0);
   REQUIRE(res == 42);

   std::vector<int> v2(6);
   auto diff = STL::adjacent_difference(std::begin(v0), std::end(v0), std::begin(v2));
   REQUIRE(diff == std::end(v2));
   REQUIRE(v2 == std::vector<int>({1, 1, 2, 2, 3, 3}));

   std::vector<int> v3(6);
   auto end = STL::partial_sum(std::begin(v0), std::end(v0), std::begin(v3));
   REQUIRE(end == std::end(v3));
   REQUIRE(v3 == std::vector<int>({ 1, 3, 6, 10, 15, 21 }));
}

TEST_CASE("reduce", "[reduce]")
{
   std::vector<int> v1 = { 1, 2, 3, 4, 5, 6 };
   REQUIRE(STL::reduce(std::begin(v1), std::end(v1)) == 21);
}

TEST_CASE("exclusive_scan", "[exclusive_scan]")
{
   std::vector<int> v1 = { 1, 2, 3, 4, 5, 6 };
   std::vector<int> v2 = { 4, 5, 7, 10, 14, 19 };
   std::vector<int> v3(6, 0);
   std::vector<int> v4 = { 1, 3, 6, 10, 15, 21 };

   REQUIRE(STL::exclusive_scan(std::begin(v1), std::end(v1), std::begin(v3), 4) == std::end(v3));
   REQUIRE(v3 == v2);

   REQUIRE(STL::inclusive_scan(std::begin(v1), std::end(v1), std::begin(v3)) == std::end(v3));
   REQUIRE(v3 == v4);
}

TEST_CASE("transform_reduce", "[transform_reduce]")
{
   std::vector<int> v1 = { 1, 2, 3, 4, 5, 6 };
   std::vector<int> v2 = { 2, 2, 2, 3, 3, 3 };

   REQUIRE(STL::transform_reduce(std::begin(v1), std::end(v1), std::begin(v2), 4) == 61);
}

#include "array.h"
TEST_CASE("array", "[array]")
{
   static_assert(std::is_same<int, STL::array<int, 5>::value_type>::value, 
      "STL::array::value_type");
   static_assert(std::is_same<std::size_t, STL::array<int, 5>::size_type>::value,
      "STL::array::size_type");
   static_assert(std::is_same<std::ptrdiff_t, STL::array<int, 5>::difference_type>::value,
      "STL::array::difference_type");
   static_assert(std::is_same<int&, STL::array<int, 5>::reference>::value,
      "STL::array::reference");
   static_assert(std::is_same<const int&, STL::array<int, 5>::const_reference>::value,
      "STL::array::const_reference");
   static_assert(std::is_same<int*, STL::array<int, 5>::pointer>::value,
      "STL::array::pointer");
   static_assert(std::is_same<const int*, STL::array<int, 5>::const_pointer>::value,
      "STL::array::const_pointer");
   static_assert(std::is_same<STL::array<int, 5>::array_iterator, STL::array<int, 5>::iterator>::value,
      "STL::array::iterator");
   static_assert(std::is_same<STL::array<int, 5>::const_array_iterator, STL::array<int, 5>::const_iterator>::value,
      "STL::array::const_iterator");
   static_assert(std::is_same<STL::array<int, 5>::array_reverse_iterator, STL::array<int, 5>::reverse_iterator>::value,
      "STL::array::reverse_iterator");
   static_assert(std::is_same<STL::array<int, 5>::const_array_reverse_iterator, STL::array<int, 5>::const_reverse_iterator>::value,
      "STL::array::const_reverse_iterator");

   STL::array<int, 5> a1{ 0, 0, 0, 0, 0 };
   const STL::array<int, 5> a2{ 1, 2, 3, 4, 5 };

   REQUIRE(a1[0] == 0);
   a1[0] = 1;
   REQUIRE(a2[0] == 1);
   REQUIRE(a1.at(0) == 1);

   a1.at(0) = 2;
   REQUIRE(a1[0] == 2);

   REQUIRE(a1.front() == 2);
   a1.front() = 3;
   REQUIRE(a1.front() == 3);
   REQUIRE(a2.front() == 1);
   REQUIRE(a1.back() == 0);
   REQUIRE(a2.back() == 5);
   a1.back() = 1;
   REQUIRE(a1.back() == 1);
   REQUIRE(a1.data()[0] == 3);

   STL::array<int, 5> a3{ 1, 2, 3, 4, 5 };
   STL::array<int, 5> a4{ 5, 4, 3, 2, 1 };
   STL::array<int, 5> a5{ 1, 2, 3, 4, 5 };
   STL::array<int, 5> a6{ 5, 4, 3, 2, 1 };

   a3.swap(a4);

   REQUIRE(a3 == a6);
   REQUIRE(a4 == a5);

   a3.swap(a4);

   REQUIRE(a3 == a5);
   REQUIRE(a4 == a6);

   static_assert(std::is_same<int, STL::array<int, 5>::array_iterator::value_type>::value,
      "STL::array<int, 5>::array_iterator::value_type");
   static_assert(std::is_same<std::ptrdiff_t, STL::array<int, 5>::array_iterator::difference_type>::value,
      "STL::array<int, 5>::array_iterator::difference_type");
   static_assert(std::is_same<int&, STL::array<int, 5>::array_iterator::reference>::value,
      "STL::array<int, 5>::array_iterator::reference");
   static_assert(std::is_same<int*, STL::array<int, 5>::array_iterator::pointer>::value,
      "STL::array<int, 5>::array_iterator::pointer");
   static_assert(std::is_same<std::random_access_iterator_tag,
      STL::array<int, 5>::array_iterator::iterator_category>::value,
      "STL::array<int, 5>::array_iterator::iterator_category");

   static_assert(std::is_same<int, STL::array<int, 5>::const_array_iterator::value_type>::value,
      "STL::array<int, 5>::const_array_iterator::value_type");
   static_assert(std::is_same<std::ptrdiff_t, STL::array<int, 5>::const_array_iterator::difference_type>::value,
      "STL::array<int, 5>::const_array_iterator::difference_type");
   static_assert(std::is_same<const int&, STL::array<int, 5>::const_array_iterator::reference>::value,
      "STL::array<int, 5>::const_array_iterator::reference");
   static_assert(std::is_same<const int*, STL::array<int, 5>::const_array_iterator::pointer>::value,
      "STL::array<int, 5>::const_array_iterator::pointer");
   static_assert(std::is_same<std::random_access_iterator_tag, 
      STL::array<int, 5>::const_array_iterator::iterator_category>::value,
      "STL::array<int, 5>::const_array_iterator::iterator_category");

   auto it = a3.cbegin();
   REQUIRE(*it == 1);
   REQUIRE(*a2.cbegin() == 1);
   REQUIRE(*a2.begin() == 1);
   REQUIRE(*a3.cbegin() == 1);
   REQUIRE(*a3.begin() == 1);

   it += 1;
   REQUIRE(*it == 2);
   it += 3;
   REQUIRE(*it == 5);
   it = a3.cbegin();
   ++it;
   REQUIRE(*it == 2);

   auto it1 = a3.cbegin();
   REQUIRE(it != it1);
   ++it1;
   REQUIRE(it == it1);

   auto it2 = a3.begin();
   *it2 = 10;
   REQUIRE(*it2 == 10);

   STL::array<std::string, 3> s1{ "array", "unit", "test" };
   auto it3 = s1.begin();
   REQUIRE(it3->size() == 5);
   auto it4 = it3++;
   REQUIRE(it3->size() == 4);
   REQUIRE(it4->size() == 5);
   --it3;
   REQUIRE(it3 == it4);
   it3++;
   REQUIRE(it3->size() == 4);
   it3--;
   REQUIRE(it3 == it4);
   REQUIRE(it3->size() == 5);

   it3 = it3 + 1;
   REQUIRE(*it3 == "unit");

   it3 -= 1;
   REQUIRE(*it3 == "array");

   it3 += 2;
   REQUIRE(*it3 == "test");

   it3 = it3 - 2;
   REQUIRE(*it3 == "array");
   REQUIRE(it3[2] == "test");
   
   it3[2] = "test2";
   REQUIRE(it3[2] == "test2");

   it3++;
   REQUIRE(it4 < it3);
   REQUIRE(it3 > it4);
   
   it3--;
   REQUIRE(it4 <= it3);
   REQUIRE(it4 >= it3);
   REQUIRE(STL::get<0>(s1) == "array");
   REQUIRE(STL::get<1>(s1) == "unit");
   REQUIRE(STL::get<2>(s1) == "test2");
   REQUIRE(STL::get<2>(STL::array<std::string, 4>{ "this", "is", "a", "test" }) == "a");

   const STL::array<std::string, 4> s2{ "this", "is", "a", "test" };
   REQUIRE(STL::get<3>(s2) == "test");
   REQUIRE(STL::get<3>(std::move(s2)) == "test");

   STL::array<std::string, 3> s3{ "array", "unit", "test3" };
   REQUIRE(s1 < s3);

   auto rit = s1.rbegin();
   REQUIRE(*rit == "test2");
   REQUIRE(rit->size() == 5);
   REQUIRE(rit[1] == "unit");
   
   ++rit;
   REQUIRE(*rit == "unit");
   REQUIRE(rit->size() == 4);
   
   auto rit2 = rit - 1;
   REQUIRE(*rit2 == "test2");
   REQUIRE(rit2->size() == 5);
}

#include "optional.h"

TEST_CASE("optional", "[optional]")
{
   STL::optional<int> o1;
   REQUIRE(!o1.has_value());
   REQUIRE(!o1);
   REQUIRE_THROWS(o1.value());

   STL::optional<int> o2(10);
   REQUIRE(o2.has_value());
   REQUIRE(o2);
   REQUIRE(o2.value() == 10);

   STL::optional<int> o3(o2);
   REQUIRE(o3.has_value());
   REQUIRE(o3);
   REQUIRE(o3.value() == 10);

   STL::optional<long> o4(9);
   REQUIRE(o4.has_value());
   REQUIRE(o4);
   REQUIRE(o4.value() == 9);

   STL::optional<long> o5(std::move(o4));
   REQUIRE(o5.has_value());
   REQUIRE(o5);
   REQUIRE(o5.value() == 9);

   constexpr bool test = std::is_same_v<std::decay_t<STL::optional<long>&>, STL::optional<int>>;

   STL::optional<long> o6(std::move(o3));
   REQUIRE(o6.has_value());
   REQUIRE(o6);
   REQUIRE(o6.value() == 10);

   STL::optional<long> o7(o6);
   REQUIRE(o7.has_value());
   REQUIRE(o7);
   REQUIRE(o7.value() == 10);

   STL::optional<long> o8(o3);
   REQUIRE(o8.has_value());
   REQUIRE(o8);
   REQUIRE(o8.value() == 10);

   STL::optional<long> o9;
   o9 = o8;
   REQUIRE(o9.has_value());
   REQUIRE(o9);
   REQUIRE(o9.value() == 10);

   STL::optional<int> o10;
   o10 = o9;
   REQUIRE(o10.has_value());
   REQUIRE(o10);
   REQUIRE(o10.value() == 10);
   REQUIRE(*o10 == 10);

   STL::optional<int> o11;
   o11 = 5l;
   REQUIRE(o11.has_value());
   REQUIRE(o11);
   REQUIRE(o11.value() == 5l);

   o11 = STL::nullopt_t{0};
   REQUIRE(!o11.has_value());
   REQUIRE(!o11);
   REQUIRE_THROWS(o11.value());
}