#include "ht.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <cassert>

int main () {
    rll::ht_chain_addressing<int, std::string> ht_int_str ;

    std::cout << std::boolalpha ;
    assert(ht_int_str.ins(-10, "Hello") == true) ;
    assert(ht_int_str.srch(-10) == "Hello") ;
    // assert(ht_int_str.srch(-5) == "Hello") ;
    ht_int_str.del(2) ;
    ht_int_str.del(-10) ;
    // assert(ht_int_str.srch(-10) == "Hello") ;

    return 0 ;
}