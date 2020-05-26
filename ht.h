#pragma once

#include <cstdint>
#include <functional>
#include <concepts>
#include <vector>
#include <deque>
#include <iterator>
#include <stdexcept>
#include <memory>

// DEBUG
#include <iostream>
namespace rll {
    template <typename T>
    concept Hashable = requires(T a) {
        { std::hash<T>{}(a) } -> std::convertible_to<std::size_t> ;
    } ;

    template <class K, class V>
    class Node {
        public :
            Node(K k, V v) : k { k }, v { v } {}
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

    class InvalidProbeException : std::exception {
        public :
            const char* what() const noexcept {
                return "Error : Probe type is invalid.\n" ;
            }
    } ;

    template <class K, class V>
    class ht_direct_addressing {
        private :
            std::vector<std::unique_ptr<Node<K, V>>> ht ;

        public :
            ht_direct_addressing(std::size_t ht_size) {
                ht.resize(ht_size) ;
            }

            bool ins (const K& k, const V& v) {
                try {
                    // Prevents out-of-range access
                    ht.at(k) = std::make_unique<Node<K, V>>(k, v) ;
                } 
                catch (std::out_of_range& er) {
                    std::cerr << er.what() << "\n" ;
                    throw OutOfRangeException() ;
                }
                return true ;
            }

            void del (const K& k) {
                auto it = ht.begin() ;
                std::advance(it, k) ;
                ht.erase(it) ;
            }

            V srch (const K& k) {
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
    } ;

    template <Hashable K, class V>
    class ht_chain_addressing {
        private :
            std::vector<std::unique_ptr<std::deque<Node<std::size_t, V>>>> ht ;
            static constexpr int32_t bin_size { 10 } ;

        public :
            ht_chain_addressing() {
                ht.resize(bin_size) ;
            }

            bool ins (const K& k, const V& v) {
                std::size_t hash_k { std::hash<K>{}(k) } ;
                Node<std::size_t, V> h { hash_k, v } ;
                hash_k %= bin_size ;

                // No node at this index yet
                if (ht.at(hash_k) == nullptr) {
                    ht.at(hash_k) = std::make_unique<std::deque<Node<std::size_t, V>>>() ;
                }
                ht.at(hash_k)->push_back(h) ;
                return true ;
            }

            void del (const K& k) {
                std::size_t hash_k { std::hash<K>{}(k) } ;
                int bin { static_cast<int>(hash_k % bin_size) } ;
                if (ht.at(bin) == nullptr) {
                    return ;
                }

                auto it = std::find_if(ht.at(bin)->begin(), ht.at(bin)->end(), 
                [&] (Node<std::size_t, V> n) -> bool { 
                    return (n.k == hash_k) ;
                }) ;

                // Element found
                if (it != ht.at(bin)->end()) {
                    ht.at(bin)->erase(it) ;
                }

                return ;
            }

            V srch (const K& k) {
                std::size_t hash_k { std::hash<K>{}(k) } ;
                int bin { static_cast<int>(hash_k % bin_size) } ;
                if (ht.at(bin) == nullptr) {
                    throw KeyNotFoundException() ;
                }

                auto it = std::find_if(ht.at(bin)->begin(), ht.at(bin)->end(), 
                [&] (Node<std::size_t, V> n) -> bool { 
                    return (n.k == hash_k) ;
                }) ;

                // Element not found
                if (it == ht.at(bin)->end()) {
                    throw KeyNotFoundException() ;
                }

                return it->v ;
            }
    } ;

    enum class ProbeType {
        Linear,
        Quadratic
    } ;

    template <Hashable K>
    class Probe {
        private :
            static constexpr uint16_t q_c_1 = 3 ;
            static constexpr uint16_t q_c_2 = 5 ;

        public : 
            ProbeType pr_t ;

            std::size_t hash (const K& k, const std::size_t& i, const std::size_t& ht_size) {
                if (pr_t == ProbeType::Linear) {
                    return l_hash(k, i, ht_size) ;
                }
                else if (pr_t == ProbeType::Quadratic) {
                    return q_hash(k, i, ht_size) ;
                }
                else {
                    throw InvalidProbeException() ;
                }
            }

        private :
            std::size_t l_hash (const K& k, const std::size_t& i, const std::size_t& ht_size) {
                return ( std::hash<K>{}(k) + i ) % ht_size ;
            }

            std::size_t q_hash (const K& k, const std::size_t& i, const std::size_t& ht_size) {
                return ( std::hash<K>{}(k) + i * q_c_1 + i * i * q_c_2 ) % ht_size ;
            }
    } ;

    template <Hashable K, class V>
    class ht_open_addressing {
        private :
            std::vector<std::optional<Node<K, V>>> ht ;
            std::size_t curr_size ;
            Probe<K> pr ;

        public :

            ht_open_addressing(std::size_t ht_size, ProbeType pr_type) {
                ht.resize(ht_size) ;
                curr_size = 0 ;
                pr.pr_t = pr_type ;
            }

            bool ins (const K& k, const V& v) {
                std::size_t i { 0 } ;
                std::size_t j { 0 } ;

                do  {
                    j = pr.hash(k, i, ht.size()) ;
                    if (!ht.at(j)) {
                        // std::cout << "J : " << j << "\n" ;
                        ht.at(j) = Node<K, V> { k, v } ;
                        ++curr_size ;
                        return true ;
                    }
                    else {
                        ++i ;
                    }
                } while (curr_size < ht.size()) ;
                return false ;
            }

            V srch (const K& k) {
                std::size_t i { 0 } ;
                std::size_t j { 0 } ;

                do  {
                    j = pr.hash(k, i, ht.size()) ;
                    auto h = ht.at(j) ;
                    if (h && h.value().k == k) {
                        return h.value().v ;
                    }
                    else {
                        ++i ;
                    }
                } while (i != ht.size()) ;
                throw KeyNotFoundException() ;
            }
    } ;
} // namespace rll