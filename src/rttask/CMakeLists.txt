# # CMakeのバージョンを設定
# cmake_minimum_required(VERSION 2.8)

# # cppファイルをMY_SOURCEに追加
# file(GLOB MY_SOURCE *.cpp)
# # hファイルをMY_HEADERに追加
# file(GLOB MY_HEADER *.h)

# # 静的ライブラリlibyal.aを作成
# add_library(yalrt STATIC
#   ${MY_SOURCE}
# )

# # ヘッダーファイルのパスを教えてあげる
# target_include_directories(yalrt
#   PUBLIC ${PROJECT_SOURCE_DIR}/src/rttask
# )

# # 各ファイルを/usr/localに配置
# install(FILES ${MY_HEADER}
#   DESTINATION /usr/local/include/yal/rttask
# )
# install(FILES ${PROJECT_SOURCE_DIR}/build/src/rttask/libyalrt.a
#   DESTINATION /usr/local/lib  
# )
