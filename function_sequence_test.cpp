//
//  function_sequence_test.cpp
//  ChrisTest
//
//  Created by Harward, Christopher on 9/18/18.
//  Copyright © 2018 Harward, Christopher. All rights reserved.
//

#include "catch.hpp"

#include "function_sequence.h"

TEST_CASE( "FunctionSequence", "[function_sequence]" ) {
    auto func1 = [](int& s) -> int&
    {
        s += 1;
        return s;
    };
    auto func2 = [](int& s) -> int&
    {
        s += 2;
        return s;
    };
    auto func3 = [](int& s) -> int&
    {
        s += 3;
        return s;
    };
    auto seq = make_function_sequence(func1, func2, std::move(func3));
    int test = 0;
    seq(test);
    REQUIRE(test == 6);
    
    
    auto func4 = [](std::string s) -> std::string {
        return s + " this ";
    };
    auto func5 = [](std::string s) -> std::string {
        return s + "is a ";
    };
    auto func6 = [](std::string s) -> std::string {
        return s + "test";
    };
    auto seq2 = make_function_sequence(func4, func5, func6);
    REQUIRE(seq2("chris:") == "chris: this is a test");
    
    
    auto func7 = [](std::string s) -> std::string {
        return s + " processed by func.";
    };
    auto seq3 = make_function_sequence(func7);
    REQUIRE(seq3("test") == "test processed by func.");
}
