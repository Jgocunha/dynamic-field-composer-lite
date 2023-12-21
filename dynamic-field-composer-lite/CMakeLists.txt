cmake_minimum_required(VERSION 3.15)

project(dynamic-neural-field-composer
        DESCRIPTION "A lighter version of a C++ application to develop Dynamic Neural Field Architectures"
        LANGUAGES CXX
)

# Define library version
set(DNF_COMPOSER_VERSION_MAJOR 1 CACHE STRING "major version" FORCE)
set(DNF_COMPOSER_VERSION_MINOR 0 CACHE STRING "minor version" FORCE)
set(DNF_COMPOSER_VERSION ${DNF_COMPOSER_VERSION_MAJOR}.${DNF_COMPOSER_VERSION_MINOR} CACHE STRING "version" FORCE)

set(DNF_COMPOSER_LIB_PREFIX "")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DNF_COMPOSER_LIB_PREFIX "debug")
endif()

set(DNF_COMPOSER_INC_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(DNF_COMPOSER_RUNTIME_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/bin")
set(DNF_COMPOSER_LIBRARY_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib/${DNF_COMPOSER_LIB_PREFIX}")
set(DNF_COMPOSER_ARCHIVE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib/${DNF_COMPOSER_LIB_PREFIX}")
set(DNF_COMPOSER_FRAMEWORK_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/bin")
set(DNF_COMPOSER_CMAKE_CONFIG_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/${CMAKE_PROJECT_NAME}")
set(DNF_COMPOSER_ADDITIONAL_FILES_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/${CMAKE_PROJECT_NAME}")

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check whether VCPKG is set up in your system
if(NOT DEFINED ENV{VCPKG_ROOT})
    message(FATAL_ERROR "ERROR: This project requires VCPKG.\n")
endif()

# Set VCPKG root directory
set(VCPKG_ROOT $ENV{VCPKG_ROOT})

# Include VCPKG toolchain
include(${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

# Set the project directory
set(PROJECT_DIR "${CMAKE_SOURCE_DIR}")
# Pass the PROJECT_DIR as a preprocessor definition
add_compile_definitions(PROJECT_DIR="${PROJECT_DIR}")

# Set the desired data output directory
set(OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/data")
# Pass the OUTPUT_DIRECTORY as a preprocessor definition
add_compile_definitions(OUTPUT_DIRECTORY="${OUTPUT_DIRECTORY}")

# Set header files grouped by directories
set(simulation_headers
    "include/simulation/simulation.h"
    "include/simulation/visualization.h"
)
set(application_headers
    "include/application/application.h"
)
set(elements_headers
    "include/elements/element_factory.h"
    "include/elements/element.h"
    "include/elements/field_coupling.h"
    "include/elements/neural_field.h"
    "include/elements/gauss_stimulus.h"
    "include/elements/mexican_hat_kernel.h"
    "include/elements/gauss_kernel.h"
    "include/elements/normal_noise.h"
    "include/elements/gauss_field_coupling.h"
    "include/elements/kernel.h"
    "include/elements/activation_function.h"
    "include/elements/element_parameters.h"
)
set(mathtools_headers
    "include/mathtools/mathtools.h"
)
set(exceptions_headers
    "include/exceptions/exception.h"
)
set(user_interface_headers
    "include/user_interface/user_interface.h"
    "include/user_interface/user_interface_window.h"
    "include/user_interface/plot_window.h"
    "include/user_interface/simulation_window.h"
    "include/user_interface/logger_window.h"
)
set(wizards_headers
    "include/wizards/learning_wizard.h"
)
set(logging_headers
    "include/logging/logger.h"
)
set(utilities_headers
    "include/utilities/utilities.h"
)

set(header 
    ${simulation_headers}
    ${application_headers}
    ${elements_headers}
    ${mathtools_headers}
    ${exceptions_headers}
    ${user_interface_headers}
    ${user_interface_headers}
    ${wizards_headers}
    ${logging_headers}
    ${utilities_headers}
)

# Set source files
set(src 
    "src/simulation/simulation.cpp"
    "src/simulation/visualization.cpp"

    "src/application/application.cpp"
    
    "src/elements/element_factory.cpp"
    "src/elements/element.cpp"
    "src/elements/field_coupling.cpp"
    "src/elements/neural_field.cpp" 
    "src/elements/gauss_stimulus.cpp"  
    "src/elements/mexican_hat_kernel.cpp"
    "src/elements/gauss_kernel.cpp"
    "src/elements/normal_noise.cpp"
    "src/elements/gauss_field_coupling.cpp" 
    "src/elements/kernel.cpp"
    "src/elements/activation_function.cpp"
    "src/elements/element_parameters.cpp"
    
    "src/mathtools/mathtools.cpp"

    "src/exceptions/exception.cpp"
    
    "src/user_interface/user_interface.cpp"
    "src/user_interface/user_interface_window.cpp"
    "src/user_interface/plot_window.cpp"
    "src/user_interface/simulation_window.cpp"
    "src/user_interface/logger_window.cpp"

    "src/wizards/learning_wizard.cpp"

    "src/logging/logger.cpp"

    "src/utilities/utilities.cpp"
)

configure_file(./resources/resources.rc.in ./resources/resources.rc)


# Define library target
add_library(${CMAKE_PROJECT_NAME} 
    ${header}
    ${src}
    ./resources/resources.rc
)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> 
    $<INSTALL_INTERFACE:${DNF_COMPOSER_INC_INSTALL_DIR}> 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Setup imgui
find_package(imgui CONFIG REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE imgui::imgui "d3d12.lib" "dxgi.lib" "d3dcompiler.lib")

# Setup implot
find_package(implot CONFIG REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE implot::implot)

target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC
    DNF_COMPOSER=1
    DNF_COMPOSER_VERSION_MAJOR=${DNF_COMPOSER_VERSION_MAJOR}
    DNF_COMPOSER_VERSION_MINOR=${DNF_COMPOSER_VERSION_MINOR}
)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "${CMAKE_PROJECT_NAME}-${DNF_COMPOSER_VERSION}"
    POSITION_INDEPENDENT_CODE ON
)

install(TARGETS ${CMAKE_PROJECT_NAME} EXPORT ${CMAKE_PROJECT_NAME}Targets
    RUNTIME       DESTINATION ${DNF_COMPOSER_RUNTIME_INSTALL_DIR}
    LIBRARY       DESTINATION ${DNF_COMPOSER_LIBRARY_INSTALL_DIR}
    ARCHIVE       DESTINATION ${DNF_COMPOSER_ARCHIVE_INSTALL_DIR}
    FRAMEWORK     DESTINATION ${DNF_COMPOSER_FRAMEWORK_INSTALL_DIR}
)

include(CMakePackageConfigHelpers)
set(version_file "${CMAKE_CURRENT_BINARY_DIR}/cmake/${CMAKE_PROJECT_NAME}-config-version.cmake")
write_basic_package_version_file(${version_file}
    VERSION ${DNF_COMPOSER_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Install configuration-specific headers (debug/release)
install(FILES ${simulation_headers} DESTINATION ${DNF_COMPOSER_INC_INSTALL_DIR}/simulation)
install(FILES ${application_headers} DESTINATION ${DNF_COMPOSER_INC_INSTALL_DIR}/application)
install(FILES ${elements_headers} DESTINATION ${DNF_COMPOSER_INC_INSTALL_DIR}/elements)
install(FILES ${mathtools_headers} DESTINATION ${DNF_COMPOSER_INC_INSTALL_DIR}/mathtools)
install(FILES ${exceptions_headers} DESTINATION ${DNF_COMPOSER_INC_INSTALL_DIR}/exceptions)
install(FILES ${user_interface_headers} DESTINATION ${DNF_COMPOSER_INC_INSTALL_DIR}/user_interface)
install(FILES ${architecture_headers} DESTINATION ${DNF_COMPOSER_INC_INSTALL_DIR}/architecture)

install(EXPORT ${CMAKE_PROJECT_NAME}Targets DESTINATION ${DNF_COMPOSER_CMAKE_CONFIG_INSTALL_DIR}
    FILE ${CMAKE_PROJECT_NAME}-config.cmake
)


# Add executable project
set(EXE_PROJECT ${CMAKE_PROJECT_NAME}-exe)
add_executable(${EXE_PROJECT} "src/dynamic-field-composer-lite.cpp" ./resources/resources.rc)
target_include_directories(${EXE_PROJECT} PRIVATE include)
target_link_libraries(${EXE_PROJECT} PRIVATE imgui::imgui ${CMAKE_PROJECT_NAME} )

# Add executable example project run_sim
set(EXE_PROJECT_EXAMPLE_RUN_SIM ${CMAKE_PROJECT_NAME}-example-run_sim)
add_executable(${EXE_PROJECT_EXAMPLE_RUN_SIM} "examples/run_sim.cpp" ./resources/resources.rc)
target_include_directories(${EXE_PROJECT_EXAMPLE_RUN_SIM} PRIVATE include)
target_link_libraries(${EXE_PROJECT_EXAMPLE_RUN_SIM} PRIVATE imgui::imgui ${CMAKE_PROJECT_NAME})

# Add executable example project run_sim_headless
set(EXE_PROJECT_EXAMPLE_RUN_SIM_HEADLESS ${CMAKE_PROJECT_NAME}-example-run_sim_headless)
add_executable(${EXE_PROJECT_EXAMPLE_RUN_SIM_HEADLESS} "examples/run_sim_headless.cpp" ./resources/resources.rc)
target_include_directories(${EXE_PROJECT_EXAMPLE_RUN_SIM_HEADLESS} PRIVATE include)
target_link_libraries(${EXE_PROJECT_EXAMPLE_RUN_SIM_HEADLESS} PRIVATE imgui::imgui ${CMAKE_PROJECT_NAME})

# Add executable example project run_learning_wizard
set(EXE_PROJECT_EXAMPLE_RUN_SIM_HEADLESS ${CMAKE_PROJECT_NAME}-example-run_learning_wizard)
add_executable(${EXE_PROJECT_EXAMPLE_RUN_SIM_HEADLESS} "examples/run_learning_wizard.cpp" ./resources/resources.rc)
target_include_directories(${EXE_PROJECT_EXAMPLE_RUN_SIM_HEADLESS} PRIVATE include)
target_link_libraries(${EXE_PROJECT_EXAMPLE_RUN_SIM_HEADLESS} PRIVATE imgui::imgui ${CMAKE_PROJECT_NAME})

# Add executable example project run_sim_element_factory
set(EXE_PROJECT_EXAMPLE_RUN_SIM_ELEMENT_FACTORY ${CMAKE_PROJECT_NAME}-example-run_sim_element_factory)
add_executable(${EXE_PROJECT_EXAMPLE_RUN_SIM_ELEMENT_FACTORY} "examples/run_sim_element_factory.cpp" ./resources/resources.rc)
target_include_directories(${EXE_PROJECT_EXAMPLE_RUN_SIM_ELEMENT_FACTORY} PRIVATE include)
target_link_libraries(${EXE_PROJECT_EXAMPLE_RUN_SIM_ELEMENT_FACTORY} PRIVATE imgui::imgui ${CMAKE_PROJECT_NAME})

# Setup Catch2
enable_testing()
find_package(Catch2 CONFIG REQUIRED)
include(CTest)
include(Catch)

# Add test project
set(TEST_PROJECT ${CMAKE_PROJECT_NAME}-test)
add_executable(${TEST_PROJECT} 
    tests/test.cpp 
    tests/test_application.cpp
    tests/test_element.cpp 
    tests/test_gauss_kernel.cpp 
    tests/test_gauss_stimulus.cpp 
    tests/test_mexican_hat_kernel.cpp 
    tests/test_neural_field.cpp 
    tests/test_normal_noise.cpp
    tests/test_simulation.cpp 
    tests/test_user_interface.cpp 
    tests/test_visualization.cpp 
    tests/test_architecture_builder.cpp
    tests/test_architecture_file_handler.cpp
    tests/test_learning_wizard.cpp
    tests/test_element_factory.cpp
    tests/test_kernel.cpp
    tests/test_field_coupling.cpp
    tests/test_gauss_field_coupling.cpp
)
target_include_directories(${TEST_PROJECT} PRIVATE include)
target_link_libraries(${TEST_PROJECT} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain ${CMAKE_PROJECT_NAME})
# Automatically discover and add tests
catch_discover_tests(${TEST_PROJECT})