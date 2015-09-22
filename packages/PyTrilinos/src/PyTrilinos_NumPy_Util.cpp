// @HEADER
// ***********************************************************************
//
//          PyTrilinos: Python Interfaces to Trilinos Packages
//                 Copyright (2014) Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia
// Corporation, the U.S. Government retains certain rights in this
// software.
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
// Questions? Contact William F. Spotz (wfspotz@sandia.gov)
//
// ***********************************************************************
// @HEADER

#include "PyTrilinos_NumPy_Util.hpp"

namespace PyTrilinos
{

/////////////////////
// Specializations //
/////////////////////

template<>
int NumPy_TypeCode< bool >()
{
  return NPY_BOOL;
}

template<>
int NumPy_TypeCode< char >()
{
  return NPY_BYTE;
}

template<>
int NumPy_TypeCode< unsigned char >()
{
  return NPY_UBYTE;
}

template<>
int NumPy_TypeCode< short >()
{
  return NPY_SHORT;
}

template<>
int NumPy_TypeCode< unsigned short >()
{
  return NPY_USHORT;
}

template<>
int NumPy_TypeCode< int >()
{
  return NPY_INT;
}

template<>
int NumPy_TypeCode< unsigned int >()
{
  return NPY_UINT;
}

template<>
int NumPy_TypeCode< long >()
{
  return NPY_LONG;
}

template<>
int NumPy_TypeCode< unsigned long >()
{
  return NPY_ULONG;
}

template<>
int NumPy_TypeCode< long long >()
{
  return NPY_LONGLONG;
}

template<>
int NumPy_TypeCode< unsigned long long >()
{
  return NPY_ULONGLONG;
}

template<>
int NumPy_TypeCode< float >()
{
  return NPY_FLOAT;
}

template<>
int NumPy_TypeCode< double >()
{
  return NPY_DOUBLE;
}

template<>
int NumPy_TypeCode< long double >()
{
  return NPY_LONGDOUBLE;
}

template<>
int NumPy_TypeCode< std::complex< float > >()
{
  return NPY_CFLOAT;
}

template<>
int NumPy_TypeCode< std::complex< double > >()
{
  return NPY_CDOUBLE;
}

template<>
int NumPy_TypeCode< std::complex< long double > >()
{
  return NPY_CLONGDOUBLE;
}

}
