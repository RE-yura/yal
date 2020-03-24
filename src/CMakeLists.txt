# CMakeのバージョンを設定
cmake_minimum_required(VERSION 2.8)

add_subdirectory(rttask)

# cppファイルをMY_SOURCEに追加
file(GLOB MY_SOURCE *.cpp)
# hファイルをMY_HEADERに追加
file(GLOB MY_HEADER *.h)

# 静的ライブラリlibyal.aを作成
add_library(yal STATIC
  ${MY_SOURCE}
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
install(FILES ${PROJECT_SOURCE_DIR}/build/src/libyal.a
  DESTINATION /usr/local/lib  
)
