#include "stdafx.h"
#include "LoginUpdate.h"
#include "mxnet2api.h"

using namespace std::chrono_literals;

void exitApp();

namespace mxnet2sample {

    LoginUpdate::LoginUpdate(int userCode, short appSlot, int interval) :
        m_interval(interval), m_stop(false), m_userCode(userCode), m_appSlot(appSlot), m_runUpdate(false),
        m_errorExit(false)
    {
        m_waitThread = std::thread(WaitProc, this);
        m_loginUpdateThread = std::thread(loginUpdate, this);
    }


    LoginUpdate::~LoginUpdate()
    {
        if (m_waitThread.joinable())
            m_waitThread.join();
    }


    // rLogIn_MatrixNet呼び出し用スレッド
    // 定期的に WaitProcスレッドによって起こされる
    void LoginUpdate::loginUpdate(LoginUpdate* obj)
    {
        for (;;)
        {
            std::unique_lock<std::mutex> lock(obj->mtx_loginUpdate);

            obj->cv2.wait(lock, [obj] { return obj->m_runUpdate; });
            {
                if (obj->m_stop == true) break;

                short ret = rLogIn_MatrixNet(obj->m_userCode, obj->m_appSlot, 1);
                std::cout << ">>login=" << ret << std::endl;

                if (obj->m_stop == true) break;

                //接続不可、サーバにセッションがなければ再接続
                if (ret == -100 || ret == -105)
                {
                    for (int retry = 0; retry < 3; retry++)
                    {
                        ret = rInit_MatrixAPI();
                        if (ret > 0 || obj->m_stop == true) break;
                    }
                }


                //エラ-　/ m_stop で終了
                if (ret <= 0 || obj->m_stop == true) {
                    obj->stop();
                    obj->m_errorExit = true;
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
                // loginUpdate スレッドを終了させる
                std::lock_guard<std::mutex> lock(obj->mtx_loginUpdate);
                obj->m_runUpdate = true;
                obj->cv2.notify_one();

                //このスレッド終了
                break;
            }
            //m_intervalタイムアウト
            else
            {

                //別スレッドで LogIn_MatrixNetを呼び出す
                //(このスレッドでは LogIn_MatrixNet呼出でブロックしないように )          

                std::unique_lock<std::mutex> lock(obj->mtx_loginUpdate, std::try_to_lock);

                // loginUpdate スレッド処理中. 何もしない
                if (!lock)
                {
                    std::cout << "loginUpdateスレッド処理中..." << std::endl;
                    continue;
                }
                // loginUpdate スレッドを起こす
                else
                {
                    obj->m_runUpdate = true;
                    obj->cv2.notify_one();
                }
            }
        }


        //loginUpdateスレッド終了待ち
        //loginUpdateスレッドが rLoginIn_MatrixNetでブロックしていたら
        //直ぐに終了しない
        if (obj->m_loginUpdateThread.joinable())
            obj->m_loginUpdateThread.join();

        //セッション終了
        rRelease_MatrixAPI();

        //メインスレッドはキー押下げ待ちをしている
        //キー押下をしないで終了させる
        if (obj->m_errorExit == true)
            exitApp();
    }

    void LoginUpdate::stop()
    {
        std::lock_guard<std::mutex> lock(mtx_wait);
        m_stop = true;
        cv.notify_one();
    }

    void LoginUpdate::join()
    {
        if (m_waitThread.joinable())
            m_waitThread.join();
    }

}
