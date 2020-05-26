#include "ht.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <vector>

int main () {
    std::vector<rll::ht_open_addressing<int, std::string>> vec_ht_int_str 
    {
        { 5, rll::ProbeType::Linear },
        { 5, rll::ProbeType::Quadratic },
        { 5, rll::ProbeType::DoubleHash }
    } ;

    std::cout << std::boolalpha ;
    for (auto& ht_int_str : vec_ht_int_str) {
        assert(ht_int_str.ins(-5, "Hello") == true) ;
        assert(ht_int_str.ins(-1, "Hi") == true) ;
        assert(ht_int_str.ins(-4, "Hello") == true) ;
        assert(ht_int_str.ins(-2, "Hi") == true) ;
        assert(ht_int_str.ins(-3, "Hello") == true) ;
        assert(ht_int_str.ins(0, "Hello") == false) ;

        assert(ht_int_str.srch(-5) == "Hello") ;
        assert(ht_int_str.srch(-4) == "Hello") ;
        assert(ht_int_str.srch(-3) == "Hello") ;
        assert(ht_int_str.srch(-2) == "Hi") ;
        assert(ht_int_str.srch(-1) == "Hi") ;
        // assert(ht_int_str.srch(0) == "Hello") ;
    }

    return 0 ;
}