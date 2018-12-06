# cmake needs this line
cmake_minimum_required(VERSION 3.5.1)

# Enable C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Define project name
project(SVM)

file(GLOB_RECURSE SVM_SOURCES "sources/*.cpp")
file(GLOB_RECURSE SVM_HEADERS "headers/*.h")

# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
find_package(OpenCV REQUIRED)

set(SVM_INCLUDE_DIRS "")
foreach(_headerFile ${SVM_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND SVM_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES SVM_INCLUDE_DIRS)

# Declare the executable target built from your sources
add_executable(SVM ${SVM_SOURCES})

# Link your application with OpenCV libraries
target_link_libraries( SVM ${OpenCV_LIBS} )
target_include_directories(SVM PRIVATE ${SVM_INCLUDE_DIRS})
