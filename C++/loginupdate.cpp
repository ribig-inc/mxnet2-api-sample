#include "stdafx.h"
#include "LoginUpdate.h"
#include "mxnet2api.h"

using namespace std::chrono_literals;

namespace sampleApp{
    void exitApp();
}

namespace mxnet2license {

    bool getLicense(_mxINT32 usercode, _mxINT16 appSlot)
    {
        bool        bInitOk = false;
        short       ret;
        _mxINT32    serNr;

        ret = mxnet2::rInit_MatrixAPI();

        //Init_MatrixAPI失敗
        if (ret < 0)
            goto EXIT1;

        ret = mxnet2::rDongle_Count(85);
        std::cout << "カウント=" << ret << std::endl;
        if (ret <= 0)
            goto EXIT;

        serNr = mxnet2::rDongle_ReadSerNr(usercode, 1, 85);
        std::cout << "シリアル番号=" << serNr << std::endl;
        if (serNr <= 0)
            goto EXIT;

        ret = mxnet2::rLogIn_MatrixNet(usercode, appSlot, 1);
        std::cout << "Login=" << ret << std::endl;
        if (ret < 0)
            goto EXIT;

        bInitOk = true;
        goto EXIT1;

    EXIT:
        mxnet2::rRelease_MatrixAPI();

    EXIT1:
        return bInitOk;
    }


    LoginUpdate::LoginUpdate(int userCode, short appSlot, int interval) :
        m_interval(interval), m_stop(false), m_userCode(userCode), m_appSlot(appSlot), m_runUpdate(false),
        m_errorExit(false) {}

    LoginUpdate::~LoginUpdate()
    {
        //std::cout << "LoginUpdate Destructor " << std::endl;
        stop();
    }

    void LoginUpdate::start()
    {
        m_loginThread = std::thread(login, this);
        m_waitThread = std::thread(WaitProc, this);
    }

   void LoginUpdate::stopWaitThread()
   {
        //エラー発生で stopWaitThread()を呼び出して waitスレッドを止める
        // waitスレッドは止まる前に loginスレッドを止ようとする
        //loginスレッドが止まる前に waitスレッドを止めようとする。
        //m_stop==true　既に止められているので何もしない（ループ防止)
        if (m_stop == false)
        {
            std::lock_guard<std::mutex> lock(mtx_wait);
            m_stop = true;
            cv.notify_one();
        }
    }

    void LoginUpdate::stopLoginThread()
    {
         std::lock_guard<std::mutex> lock(mtx_loginUpdate);
         m_runUpdate = true;
         cv2.notify_one();
    }

    // rLogIn_MatrixNet呼び出し用スレッド
    // 定期的に WaitProcスレッドによって起こされる
    void LoginUpdate::login(LoginUpdate* obj)
    {
        for (;;)
        {
            std::unique_lock<std::mutex> lock(obj->mtx_loginUpdate);

            obj->cv2.wait(lock, [obj] { return obj->m_runUpdate; });
            {
                //m_stopが立っている状態で呼び出されたら即座に終了
                if (obj->m_stop == true) break;

                short ret = mxnet2::rLogIn_MatrixNet(obj->m_userCode, obj->m_appSlot, 1);
                std::cout << ">>login=" << ret << std::endl;

                if (obj->m_stop == true) break;

                bool bOk = true;

                if (ret < 0)
                {
                    //再試行
                    //-100　接続可能であってもクライアント接続が多すぎたり/回線不安定だと接続不能になることがあるため
                    //-105  セッションタイムアウトでセッション解放 / サーバが落ちてセッション開放
                    if (ret == -100 || ret == -105)
                    {
                        for (int retry = 0; retry < 3; retry++)
                        {
                            std::this_thread::sleep_for(250ms);

                            if (ret == -105)
                            {
                                ret = mxnet2::rInit_MatrixAPI();
                                if (ret > 0) break;
                            }
                            else
                            {
                                ret = mxnet2::rLogIn_MatrixNet(obj->m_userCode, obj->m_appSlot, 1);
                                if (ret >= 0) break;
                            }
                            if (obj->m_stop == true) break;
                        }
                        bOk = false;
                    }
                    else
                        bOk = false;
                }

                //エラ-　/ m_stop でプログラムはエラー終了
                if (bOk==false || obj->m_stop == true) {
                    //エラー発生フラッグ
                    obj->m_errorExit = true;

                    //waitスレッドを止める
                    obj->stopWaitThread();

                    //このスレッドは終了
                    break;
                }

                //処理が長引いたら 1。。。
                //std::this_thread::sleep_for(6s);

                //処理が長引いたら 2。。。
                //for (int i = 0; i < 6 && !obj->m_stop; i++)
                //    std::this_thread::sleep_for(1s);

                //処理終了
                obj->m_runUpdate = false;
            }
        }
    }

    void LoginUpdate::WaitProc(LoginUpdate* obj)
    {
        for (;;)
        {
            std::unique_lock<std::mutex> lock(obj->mtx_wait);

            //m_intervalでタイムアウト、または m_stopがtrue(キー押し下げ）を待つ

            //キーが押された
            if (obj->cv.wait_for(lock, std::chrono::seconds(obj->m_interval), [obj] { return obj->m_stop; }))
            {
                // m_stopが立っている
                // login スレッドを終了させる
                obj->stopLoginThread();

                //このスレッド終了
                break;
            }
            //m_intervalタイムアウト
            else
            {
                //別スレッドで LogIn_MatrixNetを呼び出す
                //(このスレッドでは LogIn_MatrixNet呼出でブロックしないように )          
                std::unique_lock<std::mutex> lock(obj->mtx_loginUpdate, std::try_to_lock);

                // login スレッド処理中. 何もしない
                if (!lock)
                {
                    std::cout << "loginUpdateスレッド処理中..." << std::endl;
                    continue;
                }
                // login スレッドを起こす
                else
                {
                    obj->m_runUpdate = true;
                    obj->cv2.notify_one();
                }
            }
        }


        //loginスレッド終了待ち
        //loginスレッドが rLoginIn_MatrixNetでブロックしていたら
        //直ぐに終了しない
        if (obj->m_loginThread.joinable())
            obj->m_loginThread.join();

        //メインスレッドはキー押下げ待ちをしている
        //キー押下をしないで終了させる
        if (obj->m_errorExit == true)
            sampleApp::exitApp();
    }

    void LoginUpdate::stop()
    {
        // waitスレッドを終了させる
        stopWaitThread();
        
        // m_waitThread内で m_loginThreadスレッド終了待
        // ここで、m_waitThread終了待ち
        if (m_waitThread.joinable())
            m_waitThread.join();

        //セッション終了
        mxnet2::rRelease_MatrixAPI();
    }
}
