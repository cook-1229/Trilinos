

TRIBITS_INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

TRIBITS_INCLUDE_DIRECTORIES(REQUIRED_DURING_INSTALLATION_TESTING ${CMAKE_CURRENT_SOURCE_DIR}/../../utils)

IF(NOX_ENABLE_ABSTRACT_IMPLEMENTATION_LAPACK)

  TRIBITS_ADD_EXECUTABLE_AND_TEST(
    rosenbrock
    SOURCES Rosenbrock.C
    TESTONLYLIBS noxtestutils
    COMM serial mpi
    NUM_MPI_PROCS 1
    PASS_REGULAR_EXPRESSION "Test passed!"
    )

  TRIBITS_ADD_EXECUTABLE_AND_TEST(
    broyden
    SOURCES Broyden.C
    TESTONLYLIBS noxtestutils
    COMM serial mpi
    NUM_MPI_PROCS 1
    PASS_REGULAR_EXPRESSION "Test passed!"
    )

ENDIF()
