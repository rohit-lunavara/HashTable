#include "ht_direct_addressing.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <cassert>

int main () {
    rll::ht_direct_addressing<int64_t, std::string> ht_int_str { 5 } ;

    std::cout << std::boolalpha ;
    // assert(ht_int_str.ins(20, "Hello") == true) ;
    assert(ht_int_str.ins(2, "Hello") == true) ;
    assert(ht_int_str.srch(2) == "Hello") ;
    ht_int_str.del(2) ;
    // assert(ht_int_str.srch(2) == "Hello") ;

    return 0 ;
}