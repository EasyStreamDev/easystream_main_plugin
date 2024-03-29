cmake_minimum_required(VERSION 3.20)

# Change obs-plugintemplate to your plugin's name in a machine-readable format
# (e.g.: obs-myawesomeplugin) and set
project(obs-easystream-main-plugin VERSION 1.0.0)

# Replace `Your Name Here` with the name (yours or your organization's) you want
# to see as the author of the plugin (in the plugin's metadata itself and in the installers)
set(PLUGIN_AUTHOR "EasyStream")

# Replace `com.example.obs-plugin-template` with a unique Bundle ID for macOS releases
# (used both in the installer and when submitting the installer for notarization)
set(MACOS_BUNDLEID "io.easystream.obs-easystream-main-plugin")

# Replace `me@contoso.com` with the maintainer email address you want to put in Linux packages
set(LINUX_MAINTAINER_EMAIL "contact@easystream.io")

# TAKE NOTE: No need to edit things past this point
list(APPEND CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/build/")

# set(CMAKE_PREFIX_PATH "${QTDIR}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTOUIC ON)

# In case you need C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: Debug Release" FORCE)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(BUILD_TYPE 2)
else()
    set(BUILD_TYPE 1)
endif()

# message(${CMAKE_BUILD_TYPE})
SET(PACKAGE_TYPE "ALONE" CACHE STRING "With or with not OBS" FORCE)

if(UNIX OR APPLE)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage -shared -fPIC")
        set(CMAKE_CXX_LINKER_FLAGS "-lgcov")
    endif()
endif()

# if(WIN32)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
set(CONAN_DISABLE_CHECK_COMPILER True CACHE BOOL "" FORCE)
conan_basic_setup()

# endif()

# set(LibObs_DIR "${CMAKE_SOURCE_DIR}/../obsBuildDir/libobs")
include_directories(${CMAKE_SOURCE_DIR}/include/)

if(${BUILD_UBUNTU})
    set(LibObs_DIR "/usr/local/lib/cmake/libobs")
    include_directories("/usr/local/include/obs")
elseif(${BUILD_ARCH})
    set(LibObs_DIR "/usr/lib/cmake/libobs")
    include_directories("/usr/include/obs")
elseif(WIN32)
    include_directories(${CMAKE_SOURCE_DIR}/compileResource/obs-studio/libobs/)
    include_directories(${CMAKE_SOURCE_DIR}/compileResource/obs-studio/UI/obs-frontend-api/)
endif()

# if(UNIX OR APPLE)
# if(NOT CMAKE_BUILD_TYPE)
# set(CMAKE_BUILD_TYPE Debug)
# endif()
# endif()

# set(AUDIOPATH "${CMAKE_SOURCE_DIR}/output.wav")
# set(TRANSCRIPTPATH "${CMAKE_SOURCE_DIR}/src/Transcription/Ubuntu/main.py")

# if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
# set(ECHOSTRA_PATH "./Echostra")
# else
# set(ECHOSTRA_PATH "python -m src.main")
# endif()
# set(BUILD_MODE ${CMAKE_BUILD_TYPE})
configure_file(${CMAKE_SOURCE_DIR}/include/Variables.h.in ${CMAKE_SOURCE_DIR}/include/Variables.h)

# if(WIN32 OR APPLE)
# include(external/FindLibObs.cmake)
# endif()

# set(Boost_USE_STATIC_LIBS OFF)
# set(Boost_USE_MULTITHREADED ON)

# set(Boost_USE_STATIC_RUNTIME OFF)
# if(WIN32)
# set(Boost_USE_STATIC_LIBS ON)
# set(Boost_COMPILER "-vc143")
# set(BOOST_ROOT "${CMAKE_SOURCE_DIR}/compileResource/boostFolder")
# set(BOOST_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/compileResource/boostFolder/include/boost-1_81/boost")
# set(BOOST_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/compileResource/boostFolder/lib")
# endif()
if(UNIX)
    find_package(LibObs REQUIRED)
endif()

if(WIN32 OR WIN64)
    add_library(LibObs SHARED IMPORTED)
    set_property(TARGET LibObs PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/compileResource/obs-studio/build_x64/libobs/${CMAKE_BUILD_TYPE}/obs.dll")
    set_property(TARGET LibObs PROPERTY IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/compileResource/obs-studio/build_x64/libobs/${CMAKE_BUILD_TYPE}/obs.lib")

    # obs-frontend
    add_library(OBS_FRONTEND_LIB SHARED IMPORTED)
    set_property(TARGET OBS_FRONTEND_LIB PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/compileResource/obs-studio/build_x64/UI/obs-frontend-api/${CMAKE_BUILD_TYPE}/obs-frontend-api.dll")
    set_property(TARGET OBS_FRONTEND_LIB PROPERTY IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/compileResource/obs-studio/build_x64/UI/obs-frontend-api/${CMAKE_BUILD_TYPE}/obs-frontend-api.lib")
endif()

find_package(Qt6 REQUIRED COMPONENTS Core)

# if(WIN32)
find_package(asio REQUIRED)

# if(ASIO_FOUND)
# include_directories(${ASIO_INCLUDE_DIRS})
# else()
# message(FATAL_ERROR "Asio not found")
# endif()
# endif()
# find_package(cpprestsdk REQUIRED)
# if(WIN32)
include_directories(${CONAN_INCLUDE_DIRS})

# endif()

# if(NOT CMAKE_BUILD_TYPE)
# set(CMAKE_BUILD_TYPE Debug)
# endif()
configure_file(
    src/plugin-macros.h.in
    ../src/plugin-macros.generated.h
)
configure_file(
    installer/installer-macOS.pkgproj.in
    ../installer/installer-macOS.generated.pkgproj
)
configure_file(
    installer/installer-Windows.iss.in
    ../installer/installer-Windows.generated.iss
)

configure_file(
    ci/ci_includes.sh.in
    ../ci/ci_includes.generated.sh
)
configure_file(
    ci/ci_includes.cmd.in
    ../ci/ci_includes.generated.cmd
)

file(GLOB_RECURSE PLUGIN_SOURCES src/*.cpp)
file(GLOB_RECURSE TEST_SOURCES Tests/*.cpp)

set(PLUGIN_HEADERS
    src/plugin-macros.generated.h)

include_directories(${CMAKE_SOURCE_DIR}/Tests/include)

# --- Platform-specific settings ---
if(APPLE)
    find_library(COCOA Cocoa)

    if(BUILD_OUT_OF_TREE)
        find_package(Qt6MacExtras REQUIRED)
    endif()

    include_directories(${COCOA})
endif()

if(WIN32)
    set(obs_main_plugin_PLATFORM_SOURCES
        src/utils/WindowWin.cpp)
elseif(APPLE)
    set(obs_main_plugin_PLATFORM_SOURCES
        src/utils/WindowMacOS.mm)
    set_source_files_properties(src/utils/WindowMacOS.mm
        PROPERTIES COMPILE_FLAGS "-fobjc-arc")
    set(obs_main_plugin_PLATFORM_LIBS ${COCOA})
else()
    set(obs_main_plugin_PLATFORM_SOURCES
        src/utils/WindowLinux.cpp)
    set(obs_main_plugin_PLATFORM_LIBS Xss ${PROCPS_LIBRARY})
endif()

# --- Platform-independent build settings ---
add_compile_definitions("DEBUG=$<CONFIG:Debug>")

if(WIN32)
    add_library(${CMAKE_PROJECT_NAME} SHARED ${PLUGIN_SOURCES} ${PLUGIN_HEADERS} ${obs_main_plugin_PLATFORM_SOURCES})
    target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC ${CONAN_LIBS} LibObs OBS_FRONTEND_LIB asio::asio Qt6::Core)

else()
    add_library(${CMAKE_PROJECT_NAME} SHARED ${PLUGIN_SOURCES} ${PLUGIN_HEADERS} ${obs_main_plugin_PLATFORM_SOURCES})
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${CONAN_LIBS} Qt6::Core asio::asio)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    list(APPEND CPACK_SOURCE_IGNORE_FILES "/\.git/")
    list(APPEND CPACK_SOURCE_IGNORE_FILES "/\.vscode/")
    list(APPEND CPACK_SOURCE_IGNORE_FILES "/release/")
    list(APPEND CPACK_SOURCE_IGNORE_FILES "/installer/")
    list(APPEND CPACK_SOURCE_IGNORE_FILES "/build/")
    list(APPEND CPACK_SOURCE_IGNORE_FILES "/data/")
    list(APPEND CPACK_SOURCE_IGNORE_FILES "/\.github/")
    list(APPEND CPACK_SOURCE_IGNORE_FILES "/output\.wav/")
    list(APPEND CPACK_SOURCE_IGNORE_FILES "/compileResource/")
    list(APPEND CPACK_SOURCE_IGNORE_FILES "/\.gitignore.*")
    install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION .)
    set(CPACK_PACKAGE_NAME "Easystream")
    set(CPACK_PACKAGE_VENDOR "Easystream")

    if(WIN32)
        set(CPACK_PACKAGE_INSTALL_DIRECTORY "")
        set(CPACK_NSIS_INSTALL_ROOT "C:\\\\Program Files\\\\obs-studio\\\\obs-plugins\\\\64bit")
        set(CPACK_GENERATOR "NSIS;ZIP")
        set(CPACK_SOURCE_GENERATOR "ZIP")
    elseif(UNIX)
        set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Obs plugin" CACHE STRING "Package description for the package metadata")
        set(CPACK_VERBATIM_VARIABLES YES)
        set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/local/lib/obs-plugins")
        set(CPACK_DEBIAN_PACKAGE_DEPENDS "obs-studio")
        set(CPACK_DEB_COMPONENT_INSTALL YES)

        # set(PREINST_PATH "${CMAKE_SOURCE_DIR}/ci/linux/preinst")
        set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Easystream")
        set(CPACK_SET_DESTDIR ON)
        set(CPACK_GENERATOR "DEB")
        set(CPACK_SOURCE_GENERATOR "TGZ")

        # set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${PREINST_PATH}")
    endif()

    if(${PACKAGE_TYPE} STREQUAL "ALL")
        if(WIN32)
            set(CPACK_PACKAGE_NAME "EasystreamWithObs")
            list(APPEND CPACK_NSIS_EXTRA_PREINSTALL_COMMANDS "ExecWait \\\"'curl' -L -O https://github.com/obsproject/obs-studio/releases/download/29.1.1/OBS-Studio-29.1.1-Full-Installer-x64.exe\\\"")
            list(APPEND CPACK_NSIS_EXTRA_PREINSTALL_COMMANDS "ExecWait ./OBS-Studio-29.1.1-Full-Installer-x64.exe")
            string(REPLACE ";" "\n" CPACK_NSIS_EXTRA_PREINSTALL_COMMANDS "${CPACK_NSIS_EXTRA_PREINSTALL_COMMANDS}")
        endif()
    endif()

    include(CPack)
endif()

# find_package(GTest REQUIRED)
# include_directories(${GTEST_INCLUDE_DIRS})

# enable_testing()

# include_directories(
# "${LIBOBS_INCLUDE_DIR}/../UI/obs-frontend-api"
# ${Qt6Core_INCLUDES}
# ${Qt6Widgets_INCLUDES}
# )

# target_link_libraries(${CMAKE_PROJECT_NAME}
# ${obs_main_plugin_PLATFORM_LIBS}
# # libobs
# Qt6::Core
# Qt6::Widgets
# )

# add_executable(ServerTests ${TEST_SOURCES})
# target_link_libraries(ServerTests
# Boost::system
# Boost::thread
# GTest::gtest_main
# pthread
# )

# include(GoogleTest)
# gtest_discover_tests(ServerTests)

# --- End of section ---

# --- Windows-specific build settings and tasks ---
if(WIN32)
    # if(NOT DEFINED OBS_FRONTEND_LIB)
    # set(OBS_FRONTEND_LIB "OBS_FRONTEND_LIB-NOTFOUND" CACHE FILEPATH "OBS frontend library")
    # message(FATAL_ERROR "Could not find OBS Frontend API\'s library !")
    # endif()

    # Enable Multicore Builds and disable FH4 (to not depend on VCRUNTIME140_1.DLL when building with VS2019)
    if(MSVC)
        add_definitions(/MP /d2FH4-)
    endif()

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ARCH_NAME "64bit")
        set(OBS_BUILDDIR_ARCH "build64")

    # if(MSVC)
    # # Hardcoded path to w32-pthreads atm because find_library not properly finding w32-pthreads.lib
    # target_link_libraries(
    # ${CMAKE_PROJECT_NAME}
    # ${CMAKE_CURRENT_SOURCE_DIR}/obs-studio/build64/deps/w32-pthreads/RelWithDebInfo/w32-pthreads.lib)
    # endif()
    else()
        set(ARCH_NAME "32bit")
        set(OBS_BUILDDIR_ARCH "build32")

        if(MSVC)
            # Hardcoded path to w32-pthreads atm because find_library not properly finding w32-pthreads.lib
            target_link_libraries(
                ${CMAKE_PROJECT_NAME}
                ${CMAKE_CURRENT_SOURCE_DIR}/obs-studio/build32/deps/w32-pthreads/RelWithDebInfo/w32-pthreads.lib)
        endif()
    endif()

    include_directories(
        "${LIBOBS_INCLUDE_DIR}/../${OBS_BUILDDIR_ARCH}/UI"
    )

    # target_link_libraries(${CMAKE_PROJECT_NAME}
    # "${OBS_FRONTEND_LIB}"
    # )

    # --- Release package helper ---
    # The "release" folder has a structure similar OBS' one on Windows
    set(RELEASE_DIR "${PROJECT_SOURCE_DIR}/release")

    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD

        # If config is Release or RelWithDebInfo, package release files
        COMMAND if $<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>==1(
        "${CMAKE_COMMAND}" -E make_directory
        "${RELEASE_DIR}/data/obs-plugins/${CMAKE_PROJECT_NAME}"
        "${RELEASE_DIR}/obs-plugins/${ARCH_NAME}"
        )

        COMMAND if $<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>==1(
        "${CMAKE_COMMAND}" -E copy_directory
        "${PROJECT_SOURCE_DIR}/data"
        "${RELEASE_DIR}/data/obs-plugins/${CMAKE_PROJECT_NAME}"
        )

        COMMAND if $<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>==1(
        "${CMAKE_COMMAND}" -E copy
        "$<TARGET_FILE:${CMAKE_PROJECT_NAME}>"
        "${RELEASE_DIR}/obs-plugins/${ARCH_NAME}"
        )

        # If config is RelWithDebInfo, copy the pdb file
        COMMAND if $<CONFIG:RelWithDebInfo>==1(
        "${CMAKE_COMMAND}" -E copy
        "$<TARGET_PDB_FILE:${CMAKE_PROJECT_NAME}>"
        "${RELEASE_DIR}/obs-plugins/${ARCH_NAME}"
        )

        # Copy to obs-studio dev environment for immediate testing
        COMMAND if $<CONFIG:Debug>==1(
        "${CMAKE_COMMAND}" -E copy
        "$<TARGET_FILE:${CMAKE_PROJECT_NAME}>"
        "${LIBOBS_INCLUDE_DIR}/../${OBS_BUILDDIR_ARCH}/rundir/$<CONFIG>/obs-plugins/${ARCH_NAME}"
        )

        COMMAND if $<CONFIG:Debug>==1(
        "${CMAKE_COMMAND}" -E copy
        "$<TARGET_PDB_FILE:${CMAKE_PROJECT_NAME}>"
        "${LIBOBS_INCLUDE_DIR}/../${OBS_BUILDDIR_ARCH}/rundir/$<CONFIG>/obs-plugins/${ARCH_NAME}"
        )

        COMMAND if $<CONFIG:Debug>==1(
        "${CMAKE_COMMAND}" -E make_directory
        "${LIBOBS_INCLUDE_DIR}/../${OBS_BUILDDIR_ARCH}/rundir/$<CONFIG>/data/obs-plugins/${CMAKE_PROJECT_NAME}"
        )

        COMMAND if $<CONFIG:Debug>==1(
        "${CMAKE_COMMAND}" -E copy_directory
        "${PROJECT_SOURCE_DIR}/data"
        "${LIBOBS_INCLUDE_DIR}/../${OBS_BUILDDIR_ARCH}/rundir/$<CONFIG>/data/obs-plugins/${CMAKE_PROJECT_NAME}"
        )
    )

    # --- End of sub-section ---
endif()

# --- End of section ---

# --- Linux-specific build settings and tasks ---
if(UNIX OR APPLE)
    if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -g3")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    endif()
endif()

if(UNIX AND NOT APPLE)
    include(GNUInstallDirs)

    # set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES PREFIX "")
    # target_link_libraries(${CMAKE_PROJECT_NAME} obs-frontend-api)
    file(GLOB locale_files data/locale/*.ini)

    if(${BUILD_UBUNTU})
        install(TARGETS ${CMAKE_PROJECT_NAME}
            LIBRARY DESTINATION "/usr/local/lib/obs-plugins")
        install(TARGETS ${CMAKE_PROJECT_NAME}
            LIBRARY DESTINATION "/usr/lib/x86_64-linux-gnu/obs-plugins")
        install(FILES ${locale_files}
            DESTINATION "/usr/share/obs/obs-plugins/${CMAKE_PROJECT_NAME}/locale")
    elseif(${BUILD_ARCH})
        install(TARGETS ${CMAKE_PROJECT_NAME}
            LIBRARY DESTINATION "/usr/lib/obs-plugins")
        install(TARGETS ${CMAKE_PROJECT_NAME}
            LIBRARY DESTINATION "/usr/lib/x86_64-linux-gnu/obs-plugins")
        install(FILES ${locale_files}
            DESTINATION "/usr/share/obs/obs-plugins/${CMAKE_PROJECT_NAME}/locale")
    else()
        install(TARGETS ${CMAKE_PROJECT_NAME}
            LIBRARY DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}/obs-plugins")

        install(FILES ${locale_files}
            DESTINATION "${CMAKE_INSTALL_FULL_DATAROOTDIR}/obs/obs-plugins/${CMAKE_PROJECT_NAME}/locale")
    endif()
endif()

# --- End of section ---

# -- OS X specific build settings and tasks --
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -fvisibility=default")

    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES PREFIX "")
    target_link_libraries(${CMAKE_PROJECT_NAME} "${OBS_FRONTEND_LIB}")
endif()

# -- End of section --