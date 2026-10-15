cmake_minimum_required(VERSION 3.16)

project(SWDReconstruction)

  set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ROOT
  find_package(ROOT REQUIRED COMPONENTS
      Core
      RIO
      Tree
      Hist
      )

# ROOT include directories and compile flags
include(${ROOT_USE_FILE})

# Header files
include_directories(
    ${PROJECT_SOURCE_DIR}/inc
    )

# Reconstruction library
  file(GLOB SRC_FILES
      ${PROJECT_SOURCE_DIR}/src/*.cxx
                                 )

                                 add_library(reconstruction
                                 ${SRC_FILES}
                                 )

                                 target_link_libraries(reconstruction
                                 PUBLIC
                                 ROOT::Core
                                 ROOT::RIO
                                 ROOT::Tree
                                 ROOT::Hist
                                 )

# Applications
file(GLOB APP_FILES
${PROJECT_SOURCE_DIR}/app/*.cxx
)

foreach(APP_FILE ${APP_FILES})

get_filename_component(APP_NAME ${APP_FILE} NAME_WE)

add_executable(
${APP_NAME}
${APP_FILE}
)

target_link_libraries(
${APP_NAME}
PRIVATE
reconstruction
)

endforeach()
