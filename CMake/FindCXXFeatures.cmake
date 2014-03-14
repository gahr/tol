#.rst:
# FindCXXFeatures
# ---------------
#
# Check which features of the C++ standard the compiler supports
#
# When found it will set the following variables::
#
#  CXX11_COMPILER_FLAGS                      - the compiler flags needed to get C++11 features
#
#  CXXFeatures_auto_FOUND                    - auto keyword
#  CXXFeatures_class_override_final_FOUND    - override and final keywords for classes and methods
#  CXXFeatures_constexpr_FOUND               - constexpr keyword
#  CXXFeatures_cstdint_header_FOUND          - cstdint header
#  CXXFeatures_decltype_FOUND                - decltype keyword
#  CXXFeatures_defaulted_functions_FOUND     - default keyword for functions
#  CXXFeatures_delegating_constructors_FOUND - delegating constructors
#  CXXFeatures_deleted_functions_FOUND       - delete keyword for functions
#  CXXFeatures_func_identifier_FOUND         - __func__ preprocessor constant
#  CXXFeatures_initializer_list_FOUND        - initializer list
#  CXXFeatures_lambda_FOUND                  - lambdas
#  CXXFeatures_long_long_FOUND               - long long signed & unsigned types
#  CXXFeatures_nullptr_FOUND                 - nullptr
#  CXXFeatures_rvalue_references_FOUND       - rvalue references
#  CXXFeatures_sizeof_member_FOUND           - sizeof() non-static members
#  CXXFeatures_static_assert_FOUND           - static_assert()
#  CXXFeatures_variadic_templates_FOUND      - variadic templates

#=============================================================================
# Copyright 2011-2013 Rolf Eike Beer <eike@sf-mail.de>
# Copyright 2012 Andreas Weis
# Copyright 2013 Jan Kundrát
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

if (NOT CMAKE_CXX_COMPILER_LOADED)
    message(FATAL_ERROR "CXXFeatures modules only works if language CXX is enabled")
endif ()

#
### Check for needed compiler flags
#
include(${CMAKE_CURRENT_LIST_DIR}/CheckCXXCompilerFlag.cmake)

function(test_set_flag FLAG NAME)
    check_cxx_compiler_flag("${FLAG}" _HAS_${NAME}_FLAG)
    if (_HAS_${NAME}_FLAG)
        set(CXX11_COMPILER_FLAGS "${FLAG}" PARENT_SCOPE)
    endif ()
endfunction()

if (CMAKE_CXX_COMPILER_ID STREQUAL "XL")
    test_set_flag("-qlanglvl=extended0x" CXX0x)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "(Borland|Watcom)")
    # No C++11 flag for those compilers, but check_cxx_compiler_flag()
    # can't detect because they either will not always complain (Borland)
    # or will hang (Watcom).
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel" AND WIN32)
    # The Intel compiler on Windows may use these flags.
    test_set_flag("/Qstd=c++11" CXX11)
    if (NOT CXX11_COMPILER_FLAGS)
        test_set_flag("/Qstd=c++0x" CXX0x)
    endif ()
else ()
    test_set_flag("-std=c++11" CXX11)
    if (NOT CXX11_COMPILER_FLAGS)
        test_set_flag("-std=c++0x" CXX0x)
    endif ()
endif ()

function(cxx_check_feature FEATURE_NAME)
    set(RESULT_VAR "CXXFeatures_${FEATURE_NAME}_FOUND")
    if (DEFINED ${RESULT_VAR})
        return()
    endif()

    set(_bindir "${CMAKE_CURRENT_BINARY_DIR}/cxx_${FEATURE_NAME}")

    set(_SRCFILE_BASE ${CMAKE_CURRENT_LIST_DIR}/FindCXXFeatures/cxx11-${FEATURE_NAME})
    set(_LOG_NAME "\"${FEATURE_NAME}\"")
    message(STATUS "Checking C++ support for ${_LOG_NAME}")

    set(_SRCFILE "${_SRCFILE_BASE}.cxx")
    set(_SRCFILE_FAIL_COMPILE "${_SRCFILE_BASE}_fail_compile.cxx")

    try_compile(${RESULT_VAR} "${_bindir}" "${_SRCFILE}"
                COMPILE_DEFINITIONS "${CXX11_COMPILER_FLAGS}"
                OUTPUT_VARIABLE _SRCFILE_COMPILE_PASS_OUTPUT)

    if (${RESULT_VAR} AND EXISTS ${_SRCFILE_FAIL_COMPILE})
        try_compile(_TMP_RESULT "${_bindir}_fail_compile" "${_SRCFILE_FAIL_COMPILE}"
                    COMPILE_DEFINITIONS "${CXX11_COMPILER_FLAGS}"
                    OUTPUT_VARIABLE _SRCFILE_COMPILE_FAIL_OUTPUT)
        if (_TMP_RESULT)
            set(${RESULT_VAR} FALSE)
        else ()
            set(${RESULT_VAR} TRUE)
        endif ()
    endif ()

    if (${RESULT_VAR})
        message(STATUS "Checking C++ support for ${_LOG_NAME}: works")
        file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log
          "Checking C++ support for ${_LOG_NAME} passed.\n"
          "Compile pass output:\n${_SRCFILE_COMPILE_PASS_OUTPUT}\n"
          "Compile fail output:\n${_SRCFILE_COMPILE_FAIL_OUTPUT}\n")
    else ()
        message(STATUS "Checking C++ support for ${_LOG_NAME}: not supported")
        file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log
          "Checking C++ support for ${_LOG_NAME} failed.\n"
          "Compile pass output:\n${_SRCFILE_COMPILE_PASS_OUTPUT}\n"
          "Compile fail output:\n${_SRCFILE_COMPILE_FAIL_OUTPUT}\n")
    endif ()
    set(${RESULT_VAR} "${${RESULT_VAR}}" CACHE INTERNAL "C++ support for ${_LOG_NAME}")
endfunction(cxx_check_feature)

set(_CXX_ALL_FEATURES
    auto
    class_override_final
    constexpr
    cstdint_header
    decltype
    defaulted_functions
    delegating_constructors
    deleted_functions
    func_identifier
    initializer_list
    lambda
    long_long
    nullptr
    rvalue_references
    sizeof_member
    static_assert
    variadic_templates
)

if (CXXFeatures_FIND_COMPONENTS)
    foreach (_cxx_feature IN LISTS CXXFeatures_FIND_COMPONENTS)
        list(FIND _CXX_ALL_FEATURES "${_cxx_feature}" _feature_index)
        if (_feature_index EQUAL -1)
            message(FATAL_ERROR "Unknown component: '${_cxx_feature}'")
        endif ()
    endforeach ()
    unset(_feature_index)
else ()
    set(CXXFEATURES_FIND_COMPONENTS ${_CXX_ALL_FEATURES})
endif ()

foreach (_cxx_feature IN LISTS CXXFEATURES_FIND_COMPONENTS)
    cxx_check_feature(${_cxx_feature} ${FEATURE_NAME})
endforeach (_cxx_feature)

include(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)
set(DUMMY_VAR TRUE)
find_package_handle_standard_args(CXXFeatures REQUIRED_VARS DUMMY_VAR HANDLE_COMPONENTS)
unset(DUMMY_VAR)
unset(_CXX_ALL_FEATURES)
