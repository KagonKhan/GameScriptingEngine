# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
function(print_header header)
    message("\n\n=======================   ${header}   =======================\n\n")
endfunction()

function(add_gtest targetName)
    add_executable(${targetName} ${ARGN})

    target_link_libraries(${targetName} PRIVATE gtest::gtest)
    target_include_directories(${targetName} PUBLIC ${PROJECT_SOURCE_DIR}/external/include)

    gtest_discover_tests(${targetName})
endfunction()

function(install_conan)
    print_header("INSTALLING CONAN DEPS")
    execute_process(COMMAND conan install . --output-folder=out --build=missing --settings=build_type=Debug --format=json
        RESULT_VARIABLE return_code
        OUTPUT_VARIABLE conan_stdout
        ERROR_VARIABLE conan_stderr
        ECHO_ERROR_VARIABLE # show the text output regardless
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/ "
    )
    print_header("CONAN DEPS INSTALLED")
endfunction() 


cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("GameScriptingEngine")



#INSTALL_CONAN()
find_package(glfw3 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(glm REQUIRED)
find_package(spdlog REQUIRED)
find_package(FMT REQUIRED)
find_package(magic_enum REQUIRED)
find_package(TBB REQUIRED)

# Include sub-projects.
add_subdirectory(external/src/ImGui)
add_subdirectory ("GameScriptingEngine")


