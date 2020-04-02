# CMakeのバージョンを設定
cmake_minimum_required(VERSION 2.8)

# プロジェクト名と使用する言語を設定
project(yal_cmake CXX)

# C++11を使用
add_definitions("-Wall -std=c++14")

# ソースコードのディレクトリを設定
set(SRC_DIR src)

# サブディレクトリを追加
add_subdirectory(src)

# hello.cppとgood_morning.cppをコンパイルして静的ライブラリlibgreeting.aを作成
# add_library(greetings STATIC ${SRC_DIR}/hello.cpp ${SRC_DIR}/good_morning.cpp)

# install(DIRECTORY ${SRC_DIR} 
  # DESTINATION /usr/local/include )

# a.outという実行ファイルをmain.cppとhello.cppから作成
# add_executable(a.out main.cpp)

# a.outを作成する際にlibgreeting.aをリンク
# target_link_libraries(a.out greetings)

# add_executable(Main main.cpp)
# target_link_libraries(Main yal)

