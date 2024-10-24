// test.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include "mxtypes.h"
#include <iostream>
#include <string>
#include <future>
#include "mxnet2api.h"
#include "LoginUpdate.h"

#ifdef  _MSC_VER
    #if defined _WIN64
        #pragma comment( lib, "lib\\x64\\mxnet2st_mt2022.lib")
    #else
        #pragma comment( lib, "lib\\x86\\mxnet2st_mt2022.lib")
    #endif
#endif


using namespace std::chrono_literals;

const _mxINT32   USERCODE = 1234;
const short      APPSLOT = 7;
const int       INTERVAL = 5;  // second

std::condition_variable cv;
bool g_keyIn = false;

void exitApp()
{
    g_keyIn = true;
    cv.notify_one();
}

void read_key()
{
    char ch;
    std::cin.get(ch);
    g_keyIn = true;
    cv.notify_one();
}

void waitForKeyPress()
{
    std::cout << "*** Enterで終了 *** " << std::endl;

    //Enterキーが押されるまで止める
    std::thread readKey(read_key);
    readKey.detach();

    std::mutex mtx;
    std::unique_lock<std::mutex> lck(mtx);
    cv.wait(lck, [] { return g_keyIn; });
}

static int app_main()
{
    LoginUpdate updater(USERCODE, APPSLOT, INTERVAL);

    waitForKeyPress();

    // または関数内で終了
    std::cout << "終了します" << std::endl;

    updater.stop();
    updater.join();

    exit(0);
}

bool getLicense()
{
    bool        bInitOk = false;
    short       ret;
    _mxINT32    serNr;

    ret = rInit_MatrixAPI();

    //Init_MatrixAPI失敗
    if (ret < 0)
        goto EXIT1;

    ret = rDongle_Count(85);
    std::cout << "カウント=" << ret << std::endl;
    if (ret <= 0)
        goto EXIT;

    serNr = rDongle_ReadSerNr(USERCODE, 1, 85);
    std::cout << "シリアル番号=" << serNr << std::endl;
    if (serNr <= 0)
        goto EXIT;

    ret = rLogIn_MatrixNet(USERCODE, APPSLOT, 1);
    std::cout << "Login=" << ret << std::endl;
    if (ret < 0)
        goto EXIT;

    bInitOk = true;
    goto EXIT1;

EXIT:
    rRelease_MatrixAPI();

EXIT1:
    return bInitOk;
}

int main()
{
     setlocale(LC_ALL, "");

    // MxNet2 API はサーバに接続できないとブロックするため
    // 別スレッドで呼出
    std::future<bool> detectAsync = std::async(&getLicense);
    std::future_status status;

    status = detectAsync.wait_for(500ms);

    while (status != std::future_status::ready)
    {
        std::cout << "ライセンス取得中..." << std::endl;
        status = detectAsync.wait_for(2s);
    }

    if (detectAsync.get() == false) {
        std::cout << "ライセンスを取得できませんでした" << std::endl;
        return -1;
    }

    //バックグラウンドで５秒毎に LogIn_MatrixNet 呼出してライセンス取得/更新
    //フォアグラウンドでプログラム処理

    //プログラム本体の処理開始
     app_main();

     return 0;
}
