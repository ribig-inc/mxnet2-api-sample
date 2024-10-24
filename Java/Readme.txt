ソース内の USERCODE,APPSLOT, INTERVALの変更
===========================================
お客様に割り当てられたユーザコードに
変更してください。

    Demo.java
    private final int USERCODE = 1234;

APPSLOT,INTERVAL( ライセンス更新の間隔）は適宜
変更してください



コンパイル
==========
javac -encoding UTF-8  Demo.java



設定ファイル mxnetapi.ini の配置
================================

Windows
~~~~~~~
ProgramData\RiBiG\mxnet2 フォルダに
mxnetapi.ini を置いてください。

コード内で rInit_MatrixAPI 呼び出し前に
IP/Portを設定すると mxnetapi.ini を
配置しなくてサーバに接続できます。


Linux
~~~~~
/opt/mxnet2api フォルダに mxnetapi.ini を
置いてください



実行
====
java Demo


Windows
~~~~~~~
フォルダ内のmxnet2api.dll は64ビット版です。
64ビット Windowsで実行してください。

３２ビットWindowsで実行するには３２ビットDLL
と差し替えてください（フォルダ内のファイル名は
mxnet2api.dll でなければなりません。
mxnet2_api.dllではありません）


Linux
~~~~~
共有ライブラリ libmxnet2api.so (intel版/arm版のどちらか）を
/usr/lib にコピ（ C-Linuxフォルダ内の readme.txt をお読みください )


