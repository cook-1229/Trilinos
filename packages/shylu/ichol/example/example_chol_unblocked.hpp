#pragma once
#ifndef __EXAMPLE_CHOL_UNBLOCKED_HPP__
#define __EXAMPLE_CHOL_UNBLOCKED_HPP__

#include <Kokkos_Core.hpp>
#include <impl/Kokkos_Timer.hpp>

#include "util.hpp"

#include "crs_matrix_base.hpp"
#include "crs_matrix_view.hpp"
#include "crs_row_view.hpp"

#include "team_view.hpp"
#include "task_view.hpp"

#include "parallel_for.hpp"

#include "team_factory.hpp"
#include "task_factory.hpp"
#include "task_team_factory.hpp"

#include "chol.hpp"

namespace Example {

  using namespace std;

  template<typename ValueType,
           typename OrdinalType,
           typename SizeType = OrdinalType,
           typename SpaceType = void,
           typename MemoryTraits = void>
  KOKKOS_INLINE_FUNCTION
  int exampleCholUnblocked(const string file_input,
                            const int max_task_dependence,
                            const int team_size,
                            const int variant,
                            const bool verbose) {
    typedef ValueType   value_type;
    typedef OrdinalType ordinal_type;
    typedef SizeType    size_type;

    typedef CrsMatrixBase<value_type,ordinal_type,size_type,SpaceType,MemoryTraits> CrsMatrixBaseType;
    typedef CrsMatrixView<CrsMatrixBaseType> CrsMatrixViewType;

    typedef TaskTeamFactory<Kokkos::Experimental::TaskPolicy<SpaceType>,
      Kokkos::Experimental::Future<int,SpaceType>,
      Kokkos::Impl::TeamThreadRangeBoundariesStruct> TaskFactoryType;

    typedef ParallelFor ForType;
    
    typedef TaskView<CrsMatrixViewType,TaskFactoryType> CrsTaskViewType;
    
    int r_val = 0;

    Kokkos::Impl::Timer timer;
    double t = 0.0;

    cout << "CholUnblocked:: import input file = " << file_input << endl;        
    CrsMatrixBaseType AA("AA"), UU("UU");    
    {
      timer.reset();

      ifstream in;
      in.open(file_input);
      if (!in.good()) {
        cout << "Failed in open the file: " << file_input << endl;
        return ++r_val;
      }
      AA.importMatrixMarket(in);

      UU.copy(Uplo::Upper, AA);

      t = timer.seconds();

      if (verbose)
        cout << UU << endl;
    }
    cout << "CholUnblocked:: import input file::time = " << t << endl;        

#ifdef __USE_FIXED_TEAM_SIZE__
    typename TaskFactoryType::policy_type policy(max_task_dependence);
#else
    typename TaskFactoryType::policy_type policy(max_task_dependence, team_size);
#endif
    TaskFactoryType::setMaxTaskDependence(max_task_dependence);
    TaskFactoryType::setPolicy(&policy);

    cout << "CholUnblocked:: factorize the matrix" << endl;
    CrsTaskViewType U(&UU);
    U.fillRowViewArray();
    {
      timer.reset();
    
      typename TaskFactoryType::future_type future;
      switch (variant) {
      case AlgoChol::UnblockedOpt1: {
        future = TaskFactoryType::Policy().create_team(Chol<Uplo::Upper,AlgoChol::UnblockedOpt1>
                                                       ::TaskFunctor<ForType,CrsTaskViewType>(U), 0);
        break;
      }
      case AlgoChol::UnblockedOpt2: {
        future = TaskFactoryType::Policy().create_team(Chol<Uplo::Upper,AlgoChol::UnblockedOpt2>
                                                       ::TaskFunctor<ForType,CrsTaskViewType>(U), 0);
        break;
      }
      case AlgoChol::Dummy: {
        future = TaskFactoryType::Policy().create_team(Chol<Uplo::Upper,AlgoChol::Dummy>
                                                       ::TaskFunctor<ForType,CrsTaskViewType>(U), 0);
        break;
      }
      default:
        ERROR(">> Not supported algorithm variant");
        break;
      }
      TaskFactoryType::Policy().spawn(future);
      Kokkos::Experimental::wait(TaskFactoryType::Policy());

      t = timer.seconds();

      if (verbose)
        cout << UU << endl;
    }   
    cout << "CholUnblocked:: factorize the matrix::time = " << t << endl; 
    
    return r_val;
  }
}

#endif
