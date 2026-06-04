cmake_minimum_required(VERSION 3.16)

project(RPI-BUS LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)
find_package(Threads REQUIRED)

add_executable(RPI-BUS
  main.cpp
  SocketCommunicatieRPIBUS.cpp
  SocketCommunicatieRPIBUS.h
  CentraalBesturingssysteemRPIBUS.h
  CentraalBesturingssysteemRPIBUS.cpp
  SysteemConfig.h
)

target_link_libraries(RPI-BUS PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    Threads::Threads
)

include(GNUInstallDirs)
install(TARGETS RPI-BUS
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)