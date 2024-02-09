cmake_minimum_required(VERSION 3.5)

# ============================================================================ #
# lanugage definition

project(NoFancyShitExtensions LANGUAGES
    VERSION 1.0.0
    CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================================ #
# library definition

add_library(NFSE-lib STATIC
    codestyle.astylerc

    src/nfse.h src/nfse.cpp
)

target_include_directories(NFSE-lib PUBLIC
    src/
)

set_target_properties(NFSE-lib PROPERTIES
    VERSION   ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION}
    PUBLIC_HEADER src/nfse.h
)

install(TARGETS NFSE-lib
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ============================================================================ #
# unit test

enable_testing()
add_definitions(-DGTEST_LANGUAGE_CXX11)

find_package(Threads REQUIRED)

if ($ENV{GOOGLETEST_DIR})
    set(GOOGLETEST_DIR $ENV{GOOGLETEST_DIR})
else ()
    if (NOT "" STREQUAL "")
        message(WARNING "Using googletest src dir specified at Qt Creator wizard")
    endif ()
    set(GOOGLETEST_DIR "")
endif ()

if (EXISTS ${GOOGLETEST_DIR})
    set(GTestSrc ${GOOGLETEST_DIR}/googletest)
    set(GMockSrc ${GOOGLETEST_DIR}/googlemock)
elseif (UNIX AND EXISTS /usr/src/gtest)
    set(GTestSrc /usr/src/gtest)
    message(WARNING "Using gtest from system")
    if (EXISTS /usr/src/gmock)
        set(GMockSrc /usr/src/gmock)
    endif ()
else ()
    message( FATAL_ERROR "No googletest src dir found - set GOOGLETEST_DIR to enable!")
endif ()

set(GTestFiles ${GTestSrc}/src/gtest-all.cc)
set(GTestIncludes ${GTestSrc} ${GTestSrc}/include)
if (NOT ${GMockSrc} STREQUAL "")
    list(APPEND GTestFiles ${GMockSrc}/src/gmock-all.cc)
    list(APPEND GTestIncludes ${GMockSrc} ${GMockSrc}/include)
endif ()

include_directories(${GTestIncludes})

# ============================================================================ #
# test files

add_executable(NFSE-test
    main.cpp

    ${GTestFiles}
)

target_include_directories(NFSE-test
    # paths relative to build directory
    PRIVATE src/
)

target_link_libraries(NFSE-test
    PRIVATE Threads::Threads
    PRIVATE NFSE-lib
)

add_test(NAME NFSE-test COMMAND NoFancyShitTest)
