// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// match.h: Rcpp R/C++ interface class library -- match
//
// Copyright (C) 2012   Dirk Eddelbuettel and Romain Francois
//
// This file is part of Rcpp.
//
// Rcpp is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Rcpp is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Rcpp.  If not, see <http://www.gnu.org/licenses/>.

#ifndef Rcpp__sugar__match_h
#define Rcpp__sugar__match_h
          
namespace Rcpp{
namespace sugar{

template <typename HASH>
class HashIndexInserter {
public:
    HashIndexInserter( HASH& hash_ ) : hash(hash_), index(1){}
    
    template <typename T>
    inline void operator()( T value ){
        hash.insert( std::make_pair(value, index++) ) ;
    }
    
private:
    HASH& hash ;
    int index;
} ; 
template <typename HASH>
class HashIndexFinder {
public:
    HashIndexFinder( HASH& hash_) : hash(hash_), end(hash.end()) {}
    
    template <typename T>
    inline int operator()( T value ){
        typename HASH::const_iterator it = hash.find(value);
        if( it == end ){
            return NA_INTEGER ;    
        } else {
            return it->second ;    
        }
    }
    
private:
    HASH& hash ;
    typename HASH::const_iterator end ;
} ;

    
// version for INTSXP, REALSXP, RAWSXP, CPLXSXP
template <int RTYPE, typename TABLE_T>        
class IndexHash {
public:
    IndexHash( const TABLE_T& table ): hash() {
        for_each( get_const_begin(table), get_const_end(table), HashIndexInserter<HASH>(hash) ) ;
    }
    
    template <typename T>
    IntegerVector match( const T& obj ){
        int n=obj.size() ;
        IntegerVector out = no_init(n) ;
        std::transform( 
            get_const_begin(obj), get_const_end(obj), 
            out.begin(), 
            HashIndexFinder<HASH>(hash)
        ) ;
        return out ;
    }
 
private:
    typedef RCPP_UNORDERED_MAP< typename Rcpp::traits::storage_type<RTYPE>::type, int > HASH ;
    typedef typename HASH::const_iterator HASH_iterator ;
    HASH hash ;    
}; 
    
} // sugar

template <int RTYPE, bool NA, typename T, bool RHS_NA, typename RHS_T>
inline IntegerVector match( const VectorBase<RTYPE,NA,T>& x, const VectorBase<RTYPE,RHS_NA,RHS_T>& table ){
    sugar::IndexHash<RTYPE,RHS_T> hash( table.get_ref() ) ;
    return hash.match( x.get_ref() ) ;
}


template <int RTYPE, bool NA, typename T>
inline IntegerVector self_match( const VectorBase<RTYPE,NA,T>& x ){
    const T& ref = x.get_ref() ;
    sugar::IndexHash<RTYPE,T> hash( ref ) ;
    return hash.match( ref ) ;
}


} // Rcpp
#endif
