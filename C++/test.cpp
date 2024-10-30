
#include "stdafx.h"
#include "mxtypes.h"

#include <iostream>
#include <string>
#include <future>
#include <locale>

#include "LoginUpdate.h"

#ifdef  _MSC_VER
    #if defined _WIN64
        #pragma comment( lib, "lib\\x64\\mxnet2st_mt2022.lib")
    #else
        #pragma comment( lib, "lib\\x86\\mxnet2st_mt2022.lib")
    #endif
#endif


using namespace std::chrono_literals;


namespace sampleApp{

    constexpr _mxINT32   USERCODE = 1234;
    constexpr _mxINT16   APPSLOT = 7;
    constexpr int        INTERVAL = 5;  // second

    std::mutex mtx;
    std::condition_variable cv;
    bool g_keyIn = false;

    void read_key()
    {
        char ch;
        std::cin.get(ch);

        std::unique_lock<std::mutex> lck(mtx);
        g_keyIn = true;
        cv.notify_one();
    }

    void waitForKeyPress()
    {
        std::cout << "*** Enterで終了 *** " << std::endl;

        //Enterキーが押されるまで止める
        std::thread readKey(read_key);
        readKey.detach();

        std::unique_lock<std::mutex> lck(mtx);
        cv.wait(lck, [] { return g_keyIn; });
    }

    static int app_main()
    {
        mxnet2license::LoginUpdate updater(USERCODE, APPSLOT, INTERVAL);

        //LoginUpdate バックグラウンドで５秒毎に LogIn_MatrixNet 呼出してライセンス取得/更新
        try {
            updater.start();
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << std::endl;
            return -1;
        }

        //フォアグラウンドでプログラム処理
        //Enterキーが押されるまで待つ（またはプログラム開始コードに置き換える）
        waitForKeyPress();
        std::cout << "終了します" << std::endl;


        // LoginUpdateのデストラクタが呼び出されない場合、
        // 明示的に止める
        //updater.stop();
        //exit(0);

        return 0;

        //スコープを抜ける
        //loginUpdate デストラクタ呼び出し
    }

    void exitApp()
    {
        std::unique_lock<std::mutex> lck(mtx);
        g_keyIn = true;
        cv.notify_one();
    }

}


int main()
{
     std::setlocale(LC_ALL, "");

    // MxNet2 API はサーバに接続できないとブロックするため
    // 別スレッドで呼出
    std::future<bool> detectAsync = std::async(&mxnet2license::getLicense, sampleApp::USERCODE, sampleApp::APPSLOT);

    //しばらく待つ　サーバに問題なく接続できれば、待っている間に処理は完了するはずなので
    //メッセージ表示されずに sampleApp::app_main()行へ
    std::future_status status = detectAsync.wait_for(500ms);

    while (status != std::future_status::ready)
    {
        std::cout << "ライセンス取得中..." << std::endl;
        status = detectAsync.wait_for(2s);
    }

    if (detectAsync.get() == false) {
        std::cout << "ライセンスを取得できませんでした" << std::endl;
        return -1;
    }

    //プログラム本体の処理開始
    return sampleApp::app_main();
}
