# C++ Library for Robot Control written by RE-yura
ロボット制御用C++ライブラリ

## Clone & Compile & Install
```
$ git clone git@github.com:RE-yura/yal.git
$ cd yal/build/
$ cmake ..
$ make
$ sudo make install
```

## Contents List
- 線形代数(ベクトル, 行列)
- 各種フィルタ(ローパス, ハイパス, 閾値)
- UDP通信
- プロセス間通信(FIFO)
- 軌道生成
- print(まるでPythonのように簡単に)
- インピーダンス制御(インピーダンス提示型, アドミタンス提示型)
- ボールねじ計算
- シェルコマンド実行

## Usage
1. まずは，`#include <yal.h>`をする．
1. ビルド時は`g++ hogehoge.cpp -lyal -lyalrt -pthread`でリンクする．
1. 各機能を用いた簡単なサンプルが/samplesに置いてあるので，そちらを見ながら実装．

