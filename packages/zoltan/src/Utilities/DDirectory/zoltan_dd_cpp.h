// *****************************************************************************
// * Zoltan Library for Parallel Applications                                  *
// * Copyright (c) 2000,2001,2002, Sandia National Laboratories.               *
// * This software is distributed under the GNU Lesser General Public License. *
// * For more info, see the README file in the top-level Zoltan directory.     *
// *****************************************************************************
// *****************************************************************************
// * CVS File Information :
// *    $RCSfile$
// *    $Author$
// *    $Date$
// *    $Revision$
// *****************************************************************************

// ************************************************************************
// 
// C++ wrappers for Zoltan's Distributed Directory utility
//
// Two styles of initialization: 
//
//   C++ style:  Zoltan_DD dd(comm, num_gid, num_lid, len1, len2, debug);
//
//   C style: Zoltan_DD dd;
//            dd.Create(comm, num_gid, num_lid, len1, len2, debug);
//
// ************************************************************************

#ifndef ZOLTAN_DD_CPP_H_
#define ZOLTAN_DD_CPP_H_

#include "zoltan.h"

class Zoltan_DD {

public:

  Zoltan_DD(MPI_Comm comm, int num_gid, int num_lid, int user_length,  
            int table_length, int debug_level) 
    {
    Zoltan_DD_Create (&this->DD, comm, num_gid, 
                  num_lid, user_length,  table_length, debug_level);
    }

  Zoltan_DD()
    {
    this->DD = NULL;

    // Creator of this object must call Zoltan_DD::Create to finish
    // initialization.
    }

  int Create(MPI_Comm comm, int num_gid, int num_lid, int user_length,  
            int table_length, int debug_level) 
    {
    if (this->DD)
      {
      Zoltan_DD_Destroy(&this->DD);
      this->DD = NULL;
      }

    int rc =  Zoltan_DD_Create (&this->DD, comm, num_gid, 
                  num_lid, user_length,  table_length, debug_level);

    return rc;
    }

  ~Zoltan_DD()
    {
    Zoltan_DD_Destroy (&this->DD) ;
    }
 
  int Update (ZOLTAN_ID_PTR gid, ZOLTAN_ID_PTR lid, 
                        ZOLTAN_ID_PTR user, int *partition, int count) 
    {
    return Zoltan_DD_Update (this->DD, gid, lid, user, partition, count) ;
    }
  
  int Find (ZOLTAN_ID_PTR gid, ZOLTAN_ID_PTR lid, ZOLTAN_ID_PTR data, 
                   int *partition, int count, int *owner) 
    {
    return Zoltan_DD_Find (this->DD, gid, lid, data, partition, count, owner);
    }
  
  int Remove (ZOLTAN_ID_PTR gid, int count)
    {
    return Zoltan_DD_Remove (this->DD, gid, count);
    }
  
  int Set_Hash_Fn (unsigned int (*hash) (ZOLTAN_ID_PTR, int, unsigned int))
    {
    return Zoltan_DD_Set_Hash_Fn (this->DD, hash);
    } 
  
  void Stats ()
    {
    return Zoltan_DD_Stats (this->DD) ;
    }
  
  int Print ()
    {
    return Zoltan_DD_Print (this->DD) ;
    }

private:

  Zoltan_DD_Directory *DD;
};

#endif
