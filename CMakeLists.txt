cmake_minimum_required(VERSION 3.21)
project(serialPP)

set(CMAKE_CXX_STANDARD 14)

# add_library(SerialPP SHARED Serial.cpp)

# Define o diretório de saída para o binário
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

add_executable(serialpp.out
               app/main.cpp 
               app/main.h 
               app/framing.cpp
               app/framing.h
               app/io.cpp
               app/io.h
               libs/crc16.h
               libs/serial.h
               libs/serial.cpp
               libs/poller.h
               libs/poller.cpp
) 

add_custom_target(run
    COMMAND serialpp.out
    DEPENDS serialpp.out
    
  # Define o diretório de trabalho para ./out
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/out
)