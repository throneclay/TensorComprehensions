include_directories(.)
include_directories(..)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

################################################################################
# Don't ask ... TODO: cleanup gtest
################################################################################
include_directories(${PROJECT_SOURCE_DIR}/third-party/googletest/googletest/include)
set(GTEST_LIBS gtest gtest_main)

set(GOOGLE_LIBS ${GFLAGS_LIBRARIES} ${GLOG_LIBRARIES} ${GTEST_LIBS})

################################################################################
# Finally, add tests
################################################################################

################################################################################
# Basic tests of external functionalities we rely upon
################################################################################
add_executable(test_basic_gpu test_basic_gpu.cc)
set_target_properties(test_basic_gpu PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_CUDA_SDK}" )
add_test(test_basic_gpu test_basic_gpu)
target_link_libraries(test_basic_gpu ${CUDA_LIBRARIES} ${CUDA_CUDA_LIBRARIES} ${CUDA_NVRTC_LIBRARIES} ${GOOGLE_LIBS} )

################################################################################
# Core GPU library tests
################################################################################
add_executable(test_autotuner_utility test_autotuner_utility.cc)
add_test(test_autotuner_utility test_autotuner_utility)
target_link_libraries(
    test_autotuner_utility
    ${GOOGLE_LIBS}
    ${ATEN_LIBRARIES}
    tc_cuda
    tc_autotuner
    tc_aten)

################################################################################
# ATen execution based test
################################################################################
set(ATEN_TEST_FILES
  test_autotuner
  test_compilation_cache
  test_compile_and_run
  test_tc_mapper
  test_tc_mapper_bugs
  test_corner_cases
)
foreach(i ${ATEN_TEST_FILES})
  add_executable(${i} ${i}.cc)
  add_test(${i} ${i})
  target_link_libraries(
      ${i}
      ${GOOGLE_LIBS}
      ${ATEN_LIBRARIES}
      tc_autotuner
      tc_aten)
endforeach()
