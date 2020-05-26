#include "ht.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <cassert>

int main () {
    rll::ht_open_addressing<int, std::string> ht_int_str { 5, rll::ProbeType::Linear } ;
    std::cout << std::boolalpha ;
    assert(ht_int_str.ins(-5, "Hello") == true) ;
    assert(ht_int_str.ins(-1, "Hello") == true) ;
    assert(ht_int_str.ins(-4, "Hello") == true) ;
    assert(ht_int_str.ins(-2, "Hello") == true) ;
    assert(ht_int_str.ins(-3, "Hello") == true) ;
    assert(ht_int_str.ins(0, "Hello") == false) ;

    assert(ht_int_str.srch(-5) == "Hello") ;
    assert(ht_int_str.srch(-4) == "Hello") ;
    assert(ht_int_str.srch(-3) == "Hello") ;
    assert(ht_int_str.srch(-2) == "Hello") ;
    assert(ht_int_str.srch(-1) == "Hello") ;
    // assert(ht_int_str.srch(0) == "Hello") ;

    // ht_int_str.del(2) ;
    // ht_int_str.del(-10) ;
    // assert(ht_int_str.srch(-10) == "Hello") ;

    return 0 ;
}