// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// plus.h: Rcpp R/C++ interface class library -- operator+
//                                                                      
// Copyright (C) 2010	Dirk Eddelbuettel and Romain Francois
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

#ifndef Rcpp__sugar__plus_h
#define Rcpp__sugar__plus_h

namespace Rcpp{
namespace sugar{

	template <int RTYPE, bool LHS_NA, typename LHS_T, bool RHS_NA, typename RHS_T >
	class Plus_Vector_Vector : public Rcpp::VectorBase<RTYPE,true, Plus_Vector_Vector<RTYPE,LHS_NA,LHS_T,RHS_NA,RHS_T> > {
	public:
		typedef typename Rcpp::VectorBase<RTYPE,LHS_NA,LHS_T> LHS_TYPE ;
		typedef typename Rcpp::VectorBase<RTYPE,RHS_NA,RHS_T> RHS_TYPE ;
		
		typedef typename traits::storage_type<RTYPE>::type STORAGE ;
		
		Plus_Vector_Vector( const LHS_TYPE& lhs_, const RHS_TYPE& rhs_ ) : 
			lhs(lhs_), rhs(rhs_) {}
		
		inline STORAGE operator[]( int i ) const {
			STORAGE lhs_ = lhs[i] ;
			if( traits::is_na<RTYPE>(lhs_) ) return lhs_ ;
			STORAGE rhs_ = rhs[i] ;
			return traits::is_na<RTYPE>(rhs_) ? rhs_ : (lhs_ + rhs_) ;
		}
		
		inline int size() const { return lhs.size() ; }
		
	private:
		const LHS_TYPE& lhs ;
		const RHS_TYPE& rhs ;
	} ;
	
	
	template <int RTYPE, typename LHS_T, bool RHS_NA, typename RHS_T >
	class Plus_Vector_Vector<RTYPE,false,LHS_T,RHS_NA,RHS_T> : public Rcpp::VectorBase<RTYPE,true, Plus_Vector_Vector<RTYPE,false,LHS_T,RHS_NA,RHS_T> > {
	public:
		typedef typename Rcpp::VectorBase<RTYPE,false,LHS_T> LHS_TYPE ;
		typedef typename Rcpp::VectorBase<RTYPE,RHS_NA,RHS_T> RHS_TYPE ;
		
		typedef typename traits::storage_type<RTYPE>::type STORAGE ;
		
		Plus_Vector_Vector( const LHS_TYPE& lhs_, const RHS_TYPE& rhs_ ) : 
			lhs(lhs_), rhs(rhs_) {}
		
		inline STORAGE operator[]( int i ) const {
			STORAGE rhs_ = rhs[i] ;
			if( traits::is_na<RTYPE>(rhs_) ) return rhs_ ;
			return lhs[i] + rhs_ ;
		}
		
		inline int size() const { return lhs.size() ; }
		
	private:
		const LHS_TYPE& lhs ;
		const RHS_TYPE& rhs ;
	} ;
	

	template <int RTYPE, bool LHS_NA, typename LHS_T, typename RHS_T >
	class Plus_Vector_Vector<RTYPE,LHS_NA,LHS_T,false,RHS_T> : public Rcpp::VectorBase<RTYPE,true, Plus_Vector_Vector<RTYPE,LHS_NA,LHS_T,false,RHS_T> > {
	public:
		typedef typename Rcpp::VectorBase<RTYPE,LHS_NA,LHS_T> LHS_TYPE ;
		typedef typename Rcpp::VectorBase<RTYPE,false,RHS_T> RHS_TYPE ;
		
		typedef typename traits::storage_type<RTYPE>::type STORAGE ;
		
		Plus_Vector_Vector( const LHS_TYPE& lhs_, const RHS_TYPE& rhs_ ) : 
			lhs(lhs_), rhs(rhs_) {}
		
		inline STORAGE operator[]( int i ) const {
			STORAGE lhs_ = lhs[i] ;
			if( traits::is_na<RTYPE>(lhs_) ) return lhs_ ;
			return lhs_ + rhs[i] ;
		}
		
		inline int size() const { return lhs.size() ; }
		
	private:
		const LHS_TYPE& lhs ;
		const RHS_TYPE& rhs ;
	} ;

	
	template <int RTYPE, typename LHS_T, typename RHS_T >
	class Plus_Vector_Vector<RTYPE,false,LHS_T,false,RHS_T> : public Rcpp::VectorBase<RTYPE,false, Plus_Vector_Vector<RTYPE,false,LHS_T,false,RHS_T> > {
	public:
		typedef typename Rcpp::VectorBase<RTYPE,false,LHS_T> LHS_TYPE ;
		typedef typename Rcpp::VectorBase<RTYPE,false,RHS_T> RHS_TYPE ;
		
		typedef typename traits::storage_type<RTYPE>::type STORAGE ;
		
		Plus_Vector_Vector( const LHS_TYPE& lhs_, const RHS_TYPE& rhs_ ) : 
			lhs(lhs_), rhs(rhs_) {}
		
		inline STORAGE operator[]( int i ) const {
			return lhs[i] + rhs[i] ;
		}
		
		inline int size() const { return lhs.size() ; }
		
	private:
		const LHS_TYPE& lhs ;
		const RHS_TYPE& rhs ;
	} ;
	

	
	
	
	template <int RTYPE, bool NA, typename T>
	class Plus_Vector_Primitive : public Rcpp::VectorBase<RTYPE,true, Plus_Vector_Primitive<RTYPE,NA,T> > {
	public:
		typedef typename Rcpp::VectorBase<RTYPE,NA,T> VEC_TYPE ;
		typedef typename traits::storage_type<RTYPE>::type STORAGE ;
		typedef STORAGE (Plus_Vector_Primitive::*METHOD)(int) const ;
		
		Plus_Vector_Primitive( const VEC_TYPE& lhs_, STORAGE rhs_ ) : 
			lhs(lhs_), rhs(rhs_), m() {
				
			m = Rcpp::traits::is_na<RTYPE>(rhs) ? 
				&Plus_Vector_Primitive::rhs_is_na :
				&Plus_Vector_Primitive::rhs_is_not_na ;	
				
		}
		
		inline STORAGE operator[]( int i ) const {
			return (this->*m)(i);
		}
		
		inline int size() const { return lhs.size() ; }
	
		
	private:
		const VEC_TYPE& lhs ;
		STORAGE rhs ;
		METHOD m;
		
		inline STORAGE rhs_is_na(int i) const { return rhs ; }
		inline STORAGE rhs_is_not_na(int i) const { 
			STORAGE x = lhs[i] ;
			return Rcpp::traits::is_na<RTYPE>(x) ? x : (x + rhs) ;
		}

	} ;
	
	

	template <int RTYPE, typename T>
	class Plus_Vector_Primitive<RTYPE,false,T> : public Rcpp::VectorBase<RTYPE,true, Plus_Vector_Primitive<RTYPE,false,T> > {
	public:
		typedef typename Rcpp::VectorBase<RTYPE,false,T> VEC_TYPE ;
		typedef typename traits::storage_type<RTYPE>::type STORAGE ;
		typedef STORAGE (Plus_Vector_Primitive::*METHOD)(int) const ;
		
		Plus_Vector_Primitive( const VEC_TYPE& lhs_, STORAGE rhs_ ) : 
			lhs(lhs_), rhs(rhs_), m() {
				
			m = Rcpp::traits::is_na<RTYPE>(rhs) ? 
				&Plus_Vector_Primitive::rhs_is_na :
				&Plus_Vector_Primitive::rhs_is_not_na ;	
				
		}
		
		inline STORAGE operator[]( int i ) const {
			return (this->*m)(i) ;
		}
		
		inline int size() const { return lhs.size() ; }
	
		
	private:
		const VEC_TYPE& lhs ;
		STORAGE rhs ;
		METHOD m;
		
		inline STORAGE rhs_is_na(int i) const { return rhs ; }
		inline STORAGE rhs_is_not_na(int i) const { 
			return lhs[i] + rhs ;
		}

	} ;
	
	
}
}

template <int RTYPE,bool NA, typename T>
inline Rcpp::sugar::Plus_Vector_Primitive<RTYPE,NA,T>
operator+( 
	const Rcpp::VectorBase<RTYPE,NA,T>& lhs, 
	typename Rcpp::traits::storage_type<RTYPE>::type rhs 
) {
	return Rcpp::sugar::Plus_Vector_Primitive<RTYPE,NA, T  >( lhs, rhs ) ;
}


template <int RTYPE,bool NA, typename T>
inline Rcpp::sugar::Plus_Vector_Primitive<RTYPE,NA,T>
operator+( 
	typename Rcpp::traits::storage_type<RTYPE>::type rhs, 
	const Rcpp::VectorBase<RTYPE,NA,T>& lhs
) {
	return Rcpp::sugar::Plus_Vector_Primitive<RTYPE,NA, T  >( lhs, rhs ) ;
}

template <int RTYPE,bool LHS_NA, typename LHS_T, bool RHS_NA, typename RHS_T>
inline Rcpp::sugar::Plus_Vector_Vector< 
	RTYPE , 
	LHS_NA, LHS_T , 
	RHS_NA, RHS_T 
	>
operator+( 
	const Rcpp::VectorBase<RTYPE,LHS_NA,LHS_T>& lhs,
	const Rcpp::VectorBase<RTYPE,RHS_NA,RHS_T>& rhs
) {
	return Rcpp::sugar::Plus_Vector_Vector<
		RTYPE, 
		LHS_NA, LHS_T,
		RHS_NA, RHS_T
		>( lhs, rhs ) ;
}

#endif
