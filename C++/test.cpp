// test.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "mxtypes.h"

#include <iostream>
#include <string>
#include <future>
#include <locale>
#include <exception>

#include <stdexcept>

#include "mxnet2license.h"

#ifdef  _MSC_VER
    #if defined _WIN64
        #pragma comment( lib, "lib\\x64\\mxnet2st_mt2022.lib")
    #else
        #pragma comment( lib, "lib\\x86\\mxnet2st_mt2022.lib")
    #endif
#endif


using namespace std::chrono_literals;


namespace sampleApp{

    constexpr _mxINT32   USERCODE = 34219;
    constexpr _mxINT16   APPSLOT = 7;
    constexpr int        INTERVAL = 5;  // second

    std::mutex mtx;
    std::condition_variable cv;
    bool g_keyIn = false;

    void exitApp()
    {
        std::unique_lock<std::mutex> lck(mtx);
        g_keyIn = true;
        cv.notify_one();
    }

    mxnet2::License mxnet2License(USERCODE, APPSLOT, INTERVAL, exitApp);

    void read_key()
    {
        char ch;
        std::cin.get(ch);

        exitApp();
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
        int retVal = 0;


        try {

            //MxNet2License バックグラウンド更新開始（５秒毎に LogIn_MatrixNet 呼出してライセンス更新)
            mxnet2License.startUpdate();


            //フォアグラウンドでプログラム処理
            //Enterキーが押されるまで待つ（またはプログラム開始コードに置き換える）
            waitForKeyPress();
            std::cout << "終了します" << std::endl;


            // MxNet2Licenseデストラクタが呼び出されない場合、
            // 明示的に止める
            // mxnet2License.stopUpdate();
            //exit(0);
        }
        catch (std::exception& e)
        {
            std::cout << "Exception raised: " << e.what() << std::endl;
            retVal = -1;
        }

        return retVal;
    }

}


int main()
{
     std::setlocale(LC_ALL, "");

     if( false == sampleApp::mxnet2License.get(sampleApp::USERCODE, sampleApp::APPSLOT) )
     {
        std::cout << "ライセンスを取得できませんでした" << std::endl;
        return -1;
     }

    //プログラム本体の処理開始
    return sampleApp::app_main();

    //スコープを抜ける
    //MxNet2License バックグランド処理停止（デストラクタ呼び出し）

}
