cmake_minimum_required(VERSION 3.10)
project(SimpleBuildExample)

# クロスコンパイル用のツールチェインを指定
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# クロスコンパイル用のコンパイラを指定
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

# ソースディレクトリの設定
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(ANALYSIS_DIR "${CMAKE_SOURCE_DIR}/analysis")
set(SYS_DIR "${CMAKE_SOURCE_DIR}/sys")
set(BIN_DIR "${CMAKE_BINARY_DIR}/bin")
set(OBJ_DIR "${CMAKE_BINARY_DIR}/obj")

# main.cからバージョン情報を取得
file(READ "${ANALYSIS_DIR}/main.c" MAIN_C_CONTENTS)
string(REGEX MATCH "Rev\\.([0-9a-zA-Z]+)" VERSION_MATCH ${MAIN_C_CONTENTS})

if(VERSION_MATCH)
    set(VERSION_STRING ${CMAKE_MATCH_1})
    set(EXECUTABLE_NAME "Analysis_Rev${VERSION_STRING}")
else()
    message(FATAL_ERROR "Version string 'Rev.**' not found in ${ANALYSIS_DIR}/main.c")
endif()

# ビルドディレクトリの作成
file(MAKE_DIRECTORY ${BIN_DIR})
file(MAKE_DIRECTORY ${OBJ_DIR}/src)
file(MAKE_DIRECTORY ${OBJ_DIR}/analysis)
file(MAKE_DIRECTORY ${OBJ_DIR}/sys)

# コンパイルオプションの設定
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -MMD -MP -DMACROA")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MMD -MP -DMACROA")
set(CMAKE_C_FLAGS_SRC "-O0 -g")
set(CMAKE_C_FLAGS_ANALYSIS "-O3 -march=native")

# srcディレクトリ内のファイルを静的ライブラリとしてビルド
file(GLOB SRC_FILES "${SRC_DIR}/*.c")
add_library(src_lib STATIC ${SRC_FILES})
set_target_properties(src_lib PROPERTIES
    ARCHIVE_CREATE "${CMAKE_AR} rcs <TARGET> <OBJECTS>"
)

# analysisディレクトリ内のファイルをオブジェクトファイルとしてビルド
file(GLOB ANALYSIS_C_FILES "${ANALYSIS_DIR}/*.c")
foreach(SRC_FILE ${ANALYSIS_C_FILES})
    get_filename_component(FILE_NAME ${SRC_FILE} NAME_WE)
    add_library(${FILE_NAME} OBJECT ${SRC_FILE})
    set_target_properties(${FILE_NAME} PROPERTIES
        COMPILE_FLAGS "${CMAKE_C_FLAGS_ANALYSIS}"
        OUTPUT_NAME "${OBJ_DIR}/analysis/${FILE_NAME}"
    )
endforeach()

# analysisディレクトリ内のC++ファイルをオブジェクトファイルとしてビルド
file(GLOB ANALYSIS_CPP_FILES "${ANALYSIS_DIR}/*.cpp")
foreach(SRC_FILE ${ANALYSIS_CPP_FILES})
    get_filename_component(FILE_NAME ${SRC_FILE} NAME_WE)
    add_library(${FILE_NAME} OBJECT ${SRC_FILE})
    set_target_properties(${FILE_NAME} PROPERTIES
        COMPILE_FLAGS "${CMAKE_C_FLAGS_ANALYSIS}"
        OUTPUT_NAME "${OBJ_DIR}/analysis/${FILE_NAME}"
    )
endforeach()

# sysディレクトリ内のファイルをオブジェクトファイルとしてビルド
file(GLOB SYS_FILES "${SYS_DIR}/*.c")
foreach(SRC_FILE ${SYS_FILES})
    get_filename_component(FILE_NAME ${SRC_FILE} NAME_WE)
    add_library(${FILE_NAME} OBJECT ${SRC_FILE})
    set_target_properties(${FILE_NAME} PROPERTIES
        OUTPUT_NAME "${OBJ_DIR}/sys/${FILE_NAME}"
    )
endforeach()

# コンパイル時のマルチコアオプションの設定
include(ProcessorCount)
ProcessorCount(N)
if(N EQUAL 0)
    set(N 1)
endif()
set(CMAKE_BUILD_FLAGS "-j${N}")

# 実行ファイルの作成
add_executable(${EXECUTABLE_NAME} "")
set_target_properties(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

# ビルドターゲットの設定
if(DEFINED ENV{sys})
    set(SYS_TARGET $ENV{sys})
    target_sources(${EXECUTABLE_NAME} PRIVATE ${ANALYSIS_C_FILES} ${ANALYSIS_CPP_FILES} ${SYS_DIR}/${SYS_TARGET})
else()
    target_sources(${EXECUTABLE_NAME} PRIVATE ${ANALYSIS_C_FILES} ${ANALYSIS_CPP_FILES})
endif()
target_link_libraries(${EXECUTABLE_NAME} src_lib)
