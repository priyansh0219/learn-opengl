cmake_minimum_required(VERSION 4.0.0)

project(LearnOpengl)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME})

add_subdirectory(vendor/glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_include_directories(${PROJECT_NAME} PRIVATE vendor/glfw/include)

find_program(PYTHON python.exe)

# Find python
if(PYTHON)
    execute_process(
        COMMAND ${PYTHON} --version
        OUTPUT_VARIABLE python_version
    )
    message(STATUS "Python found at ${PYTHON}. Version: ${python_version}")
else()
    message(FATAL_ERROR "Error. Python not found.")
endif()

# Create virtual environment
if(NOT IS_DIRECTORY ".venv")
    execute_process(
        COMMAND ${PYTHON} -m venv "${CMAKE_SOURCE_DIR}/.venv"
    )
endif()

# activate virtual environment
# find_program(PYTHON_VENV python.exe ${CMAKE_SOURCE_DIR}/.venv/Scripts/python.exe)
set(PYTHON_VENV "${CMAKE_SOURCE_DIR}/.venv/Scripts/python.exe")

if(PYTHON_VENV)
    execute_process(
        COMMAND ${PYTHON_VENV} --version
        OUTPUT_VARIABLE python_venv_version
    )
    message(STATUS "Python venv found at ${PYTHON_VENV}. Version: ${python_venv_version}")
else()
    message(FATAL_ERROR "Error. Python venv not found.")
endif()

# add glad
set(GLAD_PROFILE "core")
set(GLAD_API "gl=3.3")
set(GLAD_GENERATOR "${CMAKE_SOURCE_DIR}/vendor/glad/glad/__main__.py")

set(GLAD_OUTPUT_DIR "${CMAKE_BINARY_DIR}/generated/glad")

add_custom_command(
    OUTPUT ${GLAD_OUTPUT_DIR}/src/gl.c
    COMMAND ${CMAKE_COMMAND} -E make_directory ${GLAD_OUTPUT_DIR}
    COMMAND ${PYTHON_VENV} -m pip install glad
    COMMAND ${PYTHON_VENV} -m glad
    --generator=c
    --out-path=${GLAD_OUTPUT_DIR}
    --api=${GLAD_API}
    --profile=${GLAD_PROFILE}
    DEPENDS ${GLAD_GENERATOR}
    COMMENT "Generating GLAD2 loader"
    VERBATIM
)

add_custom_target(generate_glad DEPENDS ${GLAD_OUTPUT_DIR}/src/glad.c)
add_library(glad STATIC ${GLAD_OUTPUT_DIR}/src/glad.c)
target_include_directories(glad PUBLIC ${GLAD_OUTPUT_DIR}/include)
add_dependencies(glad generate_glad)

target_link_libraries(${PROJECT_NAME} PRIVATE glad)

# Add source file subdirectory
add_subdirectory(exercises/2.4.exercise2)
