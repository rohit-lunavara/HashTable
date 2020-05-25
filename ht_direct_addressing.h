#pragma once

#include <cstdint>
#include <functional>
#include <concepts>
#include <vector>
#include <iterator>
#include <stdexcept>
#include <memory>

#include <iostream>

namespace rll {
    template <typename T>
    concept Hashable = requires(T a) {
        { std::hash<T>{}(a) } ->std::convertible_to<std::size_t> ;
    } ;

    template <Hashable K, class V>
    class HashNode {
        public :
            HashNode(K k, V v) : k { k }, v { v } {}
            K k ;
            V v ;
    } ;

    class OutOfRangeException : std::exception {
        public :
            const char* what() const noexcept {
                return "Error : Out of range.\n" ;
            }
    } ;

    class KeyNotFoundException : std::exception {
        public :
            const char* what() const noexcept {
                return "Error : Key not present.\n" ;
            }
    } ;

    template <Hashable K, class V>
    class ht_direct_addressing {
        private :
            std::vector<std::unique_ptr<HashNode<K, V>>> ht ;

        public :
            ht_direct_addressing(int64_t ht_size) {
                ht.resize(ht_size) ;
            }

            bool ins(K k, V v) {
                try {
                    // Prevents out-of-range access
                    ht.at(k) = std::make_unique<HashNode<K, V>>(k, v) ;
                } 
                catch (std::out_of_range& er) {
                    std::cerr << er.what() << "\n" ;
                    throw OutOfRangeException() ;
                }
                return true ;
            }

            void del(K k) {
                auto it = ht.begin() ;
                std::advance(it, k) ;
                ht.erase(it) ;
            }

            V srch(K k) {
                // Prevents out-of-range access
                try {
                    if (ht.at(k) == nullptr) {
                        throw KeyNotFoundException() ;
                    }
                    return ht.at(k)->v ;
                }
                catch (std::out_of_range& er) {
                    std::cerr << er.what() << "\n" ;
                    throw OutOfRangeException() ;
                }
            }
    };
} // namespace rll