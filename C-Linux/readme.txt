
以下、C-Linux サンプルディレクトリがカレントディレクトリ
になっているものとします。



APIファイル（共有ライブラリ）の配置
====================================

lib フォルダ内の arm/intelフォルダのいずれかにある　libmxnet2api.so 
を /usr/local/lib にコピー ( arm版ライブラリは armv8, intel版は
amd64 対応 LIBC >= 2.31) 


	例：
	sudo cp lib/intel/libmxnet2api.so /usr/lib



ビルド
======

	>make

apitest という実行ファイルがビルドされます

/usr/lib にコピーした mxnet2api が見つからないというエラーがでたら
ライブラリディレクトリのキャッシュを更新してみてください

	>sudo ldconfig /usr/lib



設定ファイル(mxnetapi,ini) 編集
===============================

実行ファイル apitest と同じディレクトリに mxnetapi.ini がなければ
なりません。そのファイルにMxNet2サーバの IPと Port をセットしてください



実行
====

複数の apitest を同時に実行することを前提としているため、
プログラムの出力は画面ではなくログに書き出されます。

バックグラウンドで実行してください。

	> ./apitest &

複数のインスタンスを実行できます。

	> ./apitest &
	> ./apitest &
	> ./apitest &
	> ./apitest &
	> ./apitest &
	> ./apitest &


ログを逐次表示

	>journalctl -f


複数のインスタンスを区別するため /tmp/apitest というファイル
でインスタンス番号を管理します。 ログでは[] 内にインスタンス番号
が見つかります

10月 01 12:02:18 mxnet2.local apitest[5450]: [6] 2.login=280
10月 01 12:02:19 mxnet2.local apitest[5490]: [19] 1.login=280
10月 01 12:02:19 mxnet2.local apitest[5453]: [7] 2.login=280
10月 01 12:02:19 mxnet2.local apitest[5456]: [8] 2.login=280
10月 01 12:02:20 mxnet2.local apitest[5459]: [9] 2.login=280
10月 01 12:02:20 mxnet2.local apitest[5433]: [1] 3.login=280
10月 01 12:02:23 mxnet2.local apitest[5447]: [5] 3.login=280


シグナルで終了します。以下コマンドで全インスタンスを終了


	>sudo pkill -x apitest


引数に reset を与えると、インスタンス番号を 0 にリセットします

	> ./apitest reset &
	> ./apitest &


画面に出力するにはソースを変更してください。

