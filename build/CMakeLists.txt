cmake_minimum_required(VERSION 3.19)
project(archiver)

set(CMAKE_CXX_STANDARD 17)

add_executable(archiver ../ArgumentParser.cpp ../Reader.cpp ../Writer.cpp ../Trie.cpp ../Encoder.cpp ../Decoder.cpp ../main.cpp)

