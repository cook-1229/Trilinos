#pragma once
#ifndef ROL_VECTORCLONE_HPP
#define ROL_VECTORCLONE_HPP

// @HEADER
// ************************************************************************
//
//               Rapid Optimization Library (ROL) Package
//                 Copyright (2014) Sandia Corporation
//
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact lead developers:
//              Drew Kouri   (dpkouri@sandia.gov) and
//              Denis Ridzal (dridzal@sandia.gov)
//
// ************************************************************************
// @HEADER

#include "ROL_Vector.hpp"
#include <exception>
#include <typeinfo>
#include <utility>
#include <map>

/** @ingroup la_group
    \class ROL::VectorClone
    \brief Container for wrapping a reusable cloned vector. Declaring an
           object of this type as a class member variable will decrease
           the number of clones needed as memory need only be allocated once
           in the lifetime of the host object. Verifies that member and argument
           types and dimensions agree when called.
*/

namespace ROL {

namespace details {

using namespace std;

template<typename Real>
class VectorClone {
private:

  Ptr<Vector<Real>> vec_;
  bool is_allocated_;

public:

  VectorClone() : vec_(nullPtr), is_allocated_(false) {}

  Ptr<Vector<Real>> operator() ( const Vector<Real>& x ) {
    if( is_allocated_ ) {
      if( typeid(x) != typeid(*vec_) )
        throw logic_error("Argument and member vector types are different!");
      if( x.dimension() != vec_->dimension() )
        throw logic_error("Argument and member vector types have different dimensions!");
    }
    else {
      vec_ = x.clone();
      is_allocated_ = true;
    }
    return vec_;
  }

  Ptr<Vector<Real>> operator() ( const Ptr<const Vector<Real>>& x ) {
    if( is_allocated_ ) {
      if( typeid(*x) != typeid(*vec_) )
        throw logic_error("Argument and member vector types are different!");
      if( x->dimension() != vec_->dimension() )
        throw logic_error("Argument and member vector types have different dimensions!");
    }
    else {
      vec_ = x->clone();
      is_allocated_ = true;
    }
    return vec_;
  }
}; // VectorClone



/** @ingroup la_group
    \class ROL::VectorCloneMap
    \brief Container for wrapping a collection of uniquely-named reusable cloned vectors,
           which in are stored in a map with string-valued names for keys. 
*/


template<typename Real>
class VectorCloneMap {
private:

  map<const char*, VectorClone<Real>> clones_;
 
  template<typename First, typename...Rest>
  void Constructor_Impl( First first, Rest... rest ) {
    clones_[static_cast<const char*>(first)] = VectorClone<Real>();
    Constructor_Impl( rest... );
  }

  template<typename First>
  void Constructor_Impl( First first ) {
    clones_[static_cast<const char*>(first)] = VectorClone<Real>();
  }

public:

  /** \brief Preallocate keys if desired */
  template<typename... Keys>
  VectorCloneMap( Keys&&...keys ) {
    Constructor_Impl( forward<Keys>(keys)... );
  }

  Ptr<Vector<Real>> operator() ( const Vector<Real>& x, const char* name ) {
    return clones_[name](x);
  }

   Ptr<Vector<Real>> operator() ( const Ptr<const Vector<Real>>& x, const char* name ) {
    return clones_[name](x);
  }
 

}; // VectorCloneSet

} // namespace details

using details::VectorClone;
using details::VectorCloneMap;

} // namespace ROL


#endif // ROL_VECTORCLONE_HPP

