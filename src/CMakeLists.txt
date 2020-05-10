# CMakeのバージョンを設定
cmake_minimum_required(VERSION 2.8)

# サブディレクトリのCMakeLists.txtを見に行く
# add_subdirectory(rttask)

# cppファイルを検索
## 同じ階層にあるcppファイルをSRCに格納
file(GLOB SRC *.cpp)

## ./rttaskにあるcppファイルをSRC_RTTASKに格納
file(GLOB SRC_RTTASK rttask/*.cpp)


# hファイルをMY_HEADERに追加
file(GLOB MY_HEADER *.h)
file(GLOB HEADER_RTTASK rttask/*.h)

# 静的ライブラリlibyal.aを作成
add_library(yal STATIC
  ${SRC}
  ${SRC_RTTASK}
)

# ヘッダーファイルのパスを教えてあげる
target_link_libraries(yal
  pthread
)
target_include_directories(yal
  PUBLIC ${PROJECT_SOURCE_DIR}/src
)

# 各ファイルを/usr/localに配置
install(FILES yal.h
  DESTINATION /usr/local/include
)
install(FILES ${MY_HEADER}
  DESTINATION /usr/local/include/yal
)
install(FILES ${HEADER_RTTASK}
  DESTINATION /usr/local/include/ktl/rttask
)
install(FILES ${PROJECT_SOURCE_DIR}/build/src/libyal.a
  DESTINATION /usr/local/lib  
)
