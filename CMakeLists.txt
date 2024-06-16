cmake_minimum_required(VERSION 3.10)
project(SimpleBuildExample)

# ソースディレクトリの設定
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(ANALYSIS_DIR "${CMAKE_SOURCE_DIR}/analysis")
set(SYS_DIR "${CMAKE_SOURCE_DIR}/sys")
set(BIN_DIR "${CMAKE_BINARY_DIR}/bin")
set(OBJ_DIR "${CMAKE_BINARY_DIR}/obj")

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
add_executable(my_executable "")
set_target_properties(my_executable PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

# ビルドターゲットの設定
if(DEFINED ENV{sys})
    set(SYS_TARGET $ENV{sys})
    target_sources(my_executable PRIVATE ${ANALYSIS_C_FILES} ${ANALYSIS_CPP_FILES} ${SYS_DIR}/${SYS_TARGET})
else()
    target_sources(my_executable PRIVATE ${ANALYSIS_C_FILES} ${ANALYSIS_CPP_FILES})
endif()
target_link_libraries(my_executable src_lib)
