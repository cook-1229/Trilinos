#ifndef __TACHOEXP_SUPERNODE_INFO_HPP__
#define __TACHOEXP_SUPERNODE_INFO_HPP__

#include "TachoExp_Util.hpp"

/// \file TachoExp_SuperNodeInfo.hpp
/// \author Kyungjoo Kim (kyukim@sandia.gov)

namespace Tacho {

  namespace Experimental {

    template<typename ValueType, typename ExecSpace>
    struct SuperNodeInfo {
      typedef ValueType value_type;
      typedef ExecSpace exec_space;

      typedef Kokkos::View<ordinal_type*,exec_space> ordinal_type_array;
      typedef Kokkos::View<size_type*,   exec_space> size_type_array;
      typedef Kokkos::View<value_type*,  exec_space> value_type_array;

      typedef Kokkos::View<value_type**,Kokkos::LayoutLeft,exec_space> value_type_matrix;

      typedef Kokkos::Future<int> future_type;
      typedef Kokkos::View<future_type*,exec_space> future_type_array;      

      // supernodes input
      ConstUnmanagedViewType<ordinal_type_array> _supernodes;

      // dof mapping to sparse matrix
      ConstUnmanagedViewType<size_type_array> _gid_super_panel_ptr;
      ConstUnmanagedViewType<ordinal_type_array> _gid_super_panel_colidx;

      // supernode map and panel size configuration
      ConstUnmanagedViewType<size_type_array> _sid_super_panel_ptr;
      ConstUnmanagedViewType<ordinal_type_array> _sid_super_panel_colidx, _blk_super_panel_colidx;

      // supernode tree
      ConstUnmanagedViewType<size_type_array> _stree_ptr;
      ConstUnmanagedViewType<ordinal_type_array> _stree_children;

      // output : factors
      ConstUnmanagedViewType<size_type_array> _super_panel_ptr;
      UnmanagedViewType<value_type_array> _super_panel_buf;


      // parallel version; fugure list corresponding to each supernode 
      // temporal memory allocation depends on kokkos memory pool
      UnmanagedViewType<future_type_array> _supernodes_future;

      // static work space for serial execution (not sure if this is necessary)
      UnmanagedViewType<value_type_array> _super_panel_serial_work;

      KOKKOS_INLINE_FUNCTION
      SuperNodeInfo() = default;

      KOKKOS_INLINE_FUNCTION
      SuperNodeInfo(const SuperNodeInfo &b) = default;

      KOKKOS_INLINE_FUNCTION
      void
      getSuperPanelSize(const ordinal_type sid,
                        /* */ ordinal_type &m,
                        /* */ ordinal_type &n) {
        m = _supernodes(sid+1) - _supernodes(sid);
        n = _blk_super_panel_colidx(_sid_super_panel_ptr(sid+1)-1);
      }

      KOKKOS_INLINE_FUNCTION
      void
      getSuperPanel(const ordinal_type sid, 
                    const ordinal_type m, 
                    const ordinal_type n, 
                    /* */ UnmanagedViewType<value_type_array> &A) {
        A = supernode_info_type::value_type_matrix(&_super_panel_buf(_super_panel_ptr(sid)), m, n);  
      }

      KOKKOS_INLINE_FUNCTION
      value_type*
      getSuperPanelPtr(const ordinal_type sid) {
        return &_super_panel_buf(_super_panel_ptr(sid));
      }
    };

  }
}

#endif
