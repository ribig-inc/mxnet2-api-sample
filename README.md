# mxnet2-api-sample

MxNet2 ライセンスサーバ（　ネットワークドングル ) 用のクライアントAPIを用いたサンプルプログラム（C / C++ / Java / .Net / .Net Framework )です。
プログラムをビルドするために必要な　APIファイル（ライブラリ）も含まれます


## MxNet2ライセンスサーバでライセンス管理する方法

クライアントは rLogIn_MatrixNet API を呼び出してライセンスを取得します。取得可能なライセンス数はサーバに接続されている USBセキュリティキー内部に
保存されています。例えば、プログラム A を同時に 5つ使用できるライセンスを付与するには USBキーの AppSlotX に 5　をセットします。
クライアントが rLogIn_MatrixNet を呼び出す毎に MxNet2サーバは　AppSlotXのカウンターを減少させ、カウンターが 0 であればライセンス残無エラーを返します。
	
USBキーにはユーザ毎に異なるユーザコードがセットされています。USBキーにアクセスするには正しいユーザコードを指定してなけれなりません（ユーザ A の USB キーには
他ユーザはアクセス不可になります　）。 rLogIn_MatrixNet API　呼び出しにはユーザコードが必要です。
	
MxNet2サーバから１度ライセンスを取得できたとしても、タイムアウト時間経過後、自動ログアウトします。このため定期的にrLogIn_MatrixNet API　を呼び出してライセンスを
保持しなければなりません。
	

## サンプル共通事項（USERCODE, APPSLOT, INTERVALの変更）

rLogIn_MatrixNet APIはユーザコードと AppSlot番号を指定して呼び出します。
	
rLogin_MatrixNet( usercode, applot, 1 )　// 1 はドングル番号
	
サンプルプログラムでは、USERCODE, APPSLOT にユーザコードと AppSlot番号を指定します。
定期的な呼び出し間隔は INTERVAL で定義します。
	
プログラムが正しく動作するには指定した USBキーの AppSlot にライセンス数がセットされていなければ
なりません。 0 になっていると rLogIn_MatrixNetはエラー（-31 ライセンス残無）を返します。
	
#### USERCODE

お客様に割り当てられたコードに変更してください。

#### APPSLOT

プログラムで指定するドングルの APPSLOT にライセンス数宇が書き込まれていなければなりません
Webインターフェースの”管理画面２” のドングル設定で AppSlot にライセンス数をセットします。

#### INTERVAL

時間間隔を適宜変更してください。


#### 別スレッドでの API 呼び出し

MxNet2 API はネットワーク経由で MxNet2ライセンスサーバに接続します。接続できないと API呼び出しはブロックします。
メイン（UI)スレッドでクライアント API を呼び出すとプログラムが止まった状態になるため、メインスレッドとは別のスレッドで API を呼び出しています


## サーバログのリアルタイム表示 (アプライアンス版 MxNet2　）

クライアント側プログラム動作中、サーバのログをリアルタイムで表示できると便利です。
リモートログを有効にすると、転送先でサーバのログをリアルタイムで表示できます。
	
仮想 Linuxマシンなどで　rsyslog で受信側の設定を行います。受信ファイルが syslog
の場合、
	
tail -f syslog | grep mxnet2
	
で mxnet2 関連のログ追記を監視できます。

サーバログでは Login / Logout で AppSlot x( y ) と表示される y の部分は
ライセンス取得数を表します。クライアントの Login表示はライセンス残を表します。
異なっていますのでご留意ください。
	
	
## C　サンプル( Visual Studio 2022プロジェクト )

	起動後、MxNet2サーバに接続してライセンスを取得、
	その後、バックグラウンドでライセンス更新。
	フォアグラウンドではキー入力待ち

	test.cpp
		const int   USERCODE = 1234;
		const short APPSLOT = 7;
		const DWORD  INTERVAL = 5000;  //ミリ秒単位

	出来上がったEXEと同じフォルダに設定ファイル mxnetapi.ini を
	配置してください。このファイルからサーバのIP/Portを読み込みます。

	MxNet2 APIスタティックライブラリは MT Release 版です。
	サンプルも MT Release でビルドしてください。
	Debugでビルドするとエラーになります。


## C++　サンプル( Visual Studio 2022プロジェクト / Linux  )

	起動後、MxNet2サーバに接続してライセンスを取得、
	その後、バックグラウンドでライセンス更新。
	フォアグラウンドではキー入力待ち

	他サンプルに比べ、以下の処理追加のため複雑になっています。

	- 呼び出しスレッドとは別スレッドでrLogIn_MatrixNetを呼出
	- その別スレッドは都度作成せず、同じスレッドを使っている
	- エラー時、exit で強制的にプログラム終了させない

	ユーザコードはお客様に割り当てられたコードに変更してください。

	test.cpp
		const int USERCODE = 1234;
		const short APPSLOT = 7;
		const int   INTERVAL = 5;  // 秒単位


	出来上がったEXEと同じフォルダに設定ファイル mxnetapi.ini を
	配置してください。このファイルからサーバのIP/Portを読み込みます。

	MxNet2 APIスタティックライブラリは MT Release 版です。
	サンプルも MT Release でビルドしてください。
	Debugでビルドするとエラーになります。


	Linux
	~~~~~
	C-Linux - lib フォルダの arm/intel の libmxnet2api.so を /usr/libに
	コピー後、ビルド

		g++ -o test test.cpp mxnet2license.cpp -l mxnet2api -l pthread -l dl

	/usr/libにコピーした mxnet2api が見つからないというエラーがでたら
	ライブラリディレクトリのキャッシュを更新してみてください。

		>sudo ldconfig /usr/lib


## C-Linux

	Cサンプルとほぼ同じ動作をします。
	フォルダ内の readme.txt をご覧ください。


## C-Sharp-DotNetFramework( Visual Studio 2022 )

	.Net Framework のフォームで３つのスレッドで同時に
	MxNet2サーバとやり取り

	ユーザコードはお客様に割り当てられたコードに変更してください。
	
	Form1.cs
	        const int MYUSERCODE = 1234;
	        const short APPSLOT = 5;

	定期的にライセンス更新をしませんので、INTERVALは不要です

	出来上がったEXEと同じフォルダに設定ファイル mxnetapi.ini を
	配置してください。このファイルからサーバのIP/Portを読み込みます。

	32bit版 / 64bit版 DLL のいずれかを呼び出さなければなりません。
	参照設定で正しい DLL をセットしてください。プログラム本体と
	DLLのビット数は一致していなければなりません。
	

## WinFormsApp-DotNet( Visual Studio 2022 )

	.Net フォームが開くときに MxNet2サーバに接続してライセンスを取得、
	その後、バックグラウンドでライセンス更新。


	ユーザコードはお客様に割り当てられたコードに変更してください。

	Form1.cs
	        const int MYUSERCODE = 1234;
	        const short APPSLOT = 7;
        	const int INTERVAL = 5000;  //ミリ秒単位


	プロジェクトでは 64 ビット版DLLを参照しています。

	出来上がったEXEと同じフォルダに設定ファイル mxnetapi.ini を
	配置してください。このファイルからサーバのIP/Portを読み込みます。

	32bit版 / 64bit版 DLL のいずれかを呼び出さなければなりません。
	参照設定で正しい DLL をセットしてください。プログラム本体と
	DLLのビット数は一致していなければなりません。

## Java

	フォルダ内の readmd.txt をご覧ください。
	APIファイルを正しく配置することで、同一ソースコードで
	Windows / Linux に対応します。


## Excel

	Excel用DLLを呼び出すことで、MxNet2サーバに接続してライセンスを取得、
	その後、バックグラウンドでライセンス更新。

	フォルダ内の readmd.txt をご覧ください
