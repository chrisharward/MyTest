//
//  tuple_test.cpp
//  ChrisTest
//
//  Created by Harward, Christopher on 9/4/18.
//  Copyright © 2018 Harward, Christopher. All rights reserved.
//

#include "catch.hpp"
#include <sstream>

#include "tuple_stream_ops.h"

TEST_CASE( "Print tuple", "[tuple]" ) {
    {
        std::ostringstream osstr;
        std::tuple<> t0;
        osstr << t0;
        REQUIRE( osstr.str() == "[]" );
    }
    
    {
        std::ostringstream osstr;
        std::tuple<int> t1 = std::make_tuple( 1 );
        osstr << t1;
        REQUIRE( osstr.str() == "[1]" );
    }
    
    {
        std::ostringstream osstr;
        std::tuple<int, bool> t2 = std::make_tuple( 1, true );
        osstr << t2;
        REQUIRE( osstr.str() == "[1 1]" );
    }
    
    {
        std::ostringstream osstr;
        std::tuple<int, bool, std::string, double> t3 = std::make_tuple( 1, true, "string", 1.01 );
        osstr << t3;
        REQUIRE( osstr.str() == "[1 1 string 1.01]" );
    }
    
    {
        std::ostringstream osstr;
        long l = 3;
        std::tuple<int, bool, std::string, double, long&> t3 = std::make_tuple( 1, true, "string", 1.01, std::ref(l) );
        osstr << t3;
        REQUIRE( osstr.str() == "[1 1 string 1.01 3]" );
    }
}

TEST_CASE( "Parse tuple", "[tuple]" ) {
    {
        std::istringstream isstr("[]");
        std::tuple<> t;
        isstr >> t;
        REQUIRE( isstr.good() );
    }
    
    {
        std::istringstream isstr("[100]");
        std::tuple<int> t;
        isstr >> t;
        REQUIRE( isstr.good() );
        REQUIRE( std::get<0>(t) == 100 );
    }
    
    {
        std::istringstream isstr("[100][1 2 test 1]");
        std::tuple<int> t;
        std::tuple<int, int, std::string, bool> t2;
        isstr >> t >> t2;
        REQUIRE( isstr.good() );
        REQUIRE( std::get<0>(t) == 100 );
        REQUIRE( std::get<0>(t2) == 1 );
        REQUIRE( std::get<1>(t2) == 2 );
        REQUIRE( std::get<2>(t2) == "test" );
        REQUIRE( std::get<3>(t2) == true );
    }
}
