find_package(GTest)

set(COMMON_BASE
    main.cpp
)

set(SPLINART_TESTS
    test_circle.cpp
    test_spline.cpp
)

foreach(filename IN LISTS SPLINART_TESTS)
    string(REPLACE ".cpp" "" targetname ${filename})
    add_executable(${targetname} ${COMMON_BASE} ${filename})
    target_link_libraries(${targetname} PRIVATE splinart GTest::gtest_main GTest::gtest)
endforeach()

add_executable(test_splinart ${COMMON_BASE} ${SPLINART_TESTS})
target_link_libraries(test_splinart PRIVATE splinart GTest::gtest_main GTest::gtest)
