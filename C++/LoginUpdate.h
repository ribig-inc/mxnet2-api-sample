#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <future>
#include "mxtypes.h"

namespace mxnet2sample {

    class LoginUpdate {
        std::thread     m_waitThread;
        std::thread     m_loginUpdateThread;

        bool            m_stop;
        bool            m_runUpdate;
        bool            m_errorExit;

        _mxINT32        m_userCode;
        int             m_interval;
        _mxINT16        m_appSlot;

        std::mutex                   mtx_wait;
        std::mutex                   mtx_loginUpdate;
        std::condition_variable      cv;
        std::condition_variable      cv2;


    public:
        LoginUpdate(int userCode, short appSlot, int interval);
        ~LoginUpdate();
        void stop();
        void join();

        static void loginUpdate(LoginUpdate* obj);
        static void WaitProc(LoginUpdate* obj);
    };
}
