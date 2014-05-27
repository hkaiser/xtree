/*
 * types.hpp
 *
 *  Created on: May 24, 2014
 *      Author: dmarce1
 */

#ifndef TYPES_HPP_
#define TYPES_HPP_

namespace xtree {

template<typename Derived>
class static_component_base;

template<typename Dims, typename origin >
class indexer;

template<int Ndim>
class location;

template<typename Derived, int Ndim>
class node;

template<int Ndim>
class output;

template<typename Derived, int Ndim>
class tree;

template<int N, int ...Params>
struct int_seq;

template<int N, int Ndim>
struct int_seq_const;

template<int Base, int Exponent>
struct pow_;

template<typename T, int ...N>
class vector;

template< class Derived >
class static_component;


template<typename U, typename T, int Ndim>
using get_type = T (U::*)(location<Ndim>);

template<typename U, typename T, int Ndim>
using set_type = void (U::*)(location<Ndim>, T);

enum op_type {
	REBRANCH, ASCEND, DESCEND, EXCHANGE
};

}

#endif /* TYPES_HPP_ */
