cmake_minimum_required(VERSION 3.18)

find_package(Ruby REQUIRED)
set(UNITY_RUNNER_GENERATOR "${Ruby_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Unity/auto/generate_test_runner.rb")
separate_arguments(UNITY_RUNNER_GENERATOR NATIVE_COMMAND ${UNITY_RUNNER_GENERATOR})

function(add_test_runner TARGET TEST_SOURCE)
    get_filename_component(TEST_RUNNER ${TEST_SOURCE} NAME_WE)
    set(TEST_RUNNER "${TEST_RUNNER}_Runner.c")
    set(TEST_SOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_SOURCE}")
    set(TEST_RUNNER_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TEST_RUNNER}")
    add_custom_command(
        OUTPUT ${TEST_RUNNER_FILE}
        DEPENDS ${TEST_SOURCE_FILE}
        COMMAND ${UNITY_RUNNER_GENERATOR} ${TEST_SOURCE_FILE} ${TEST_RUNNER_FILE}
    )
    add_executable(${TARGET} ${TEST_RUNNER_FILE} ${TEST_SOURCE_FILE})
    target_include_directories(${TARGET} PRIVATE "${CMAKE_SOURCE_DIR}/src")
    target_link_libraries(${TARGET} PRIVATE unity)
endfunction()

function(add_test_file TARGET TEST_FILE)
    set(TEST_FILE_COPY_FROM_FILE "${CMAKE_CURRENT_SOURCE_DIR}/test_files/${TEST_FILE}")
    set(TEST_FILE_COPY_TO_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TEST_FILE}")
    configure_file(${TEST_FILE_COPY_FROM_FILE} ${TEST_FILE_COPY_TO_FILE} NEWLINE_STYLE LF)
    target_sources(${TARGET} PRIVATE ${TEST_FILE_COPY_TO_FILE})
endfunction()

add_test_runner(TestIO "TestIO.c")
add_test_file(TestIO "file_that_exists.txt")
add_test(TestIORunner TestIO)

