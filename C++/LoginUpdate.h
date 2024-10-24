#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <future>



class LoginUpdate {
    std::thread     m_waitThread;
    bool            m_stop;
    bool            m_runUpdate;
    bool            m_errorExit;
    int             m_interval;
    short           m_appSlot;
    int             m_userCode;
    
    std::mutex                   mtx_wait;
    std::mutex                   mtx_loginUpdate;
    std::condition_variable      cv;
    std::condition_variable      cv2;

    std::thread     m_loginUpdateThread;

public:
    LoginUpdate(int userCode, short appSlot, int interval);
    ~LoginUpdate();
    void stop();
    void join();

    static void loginUpdate(LoginUpdate* obj);
    static void WaitProc(LoginUpdate* obj);


};

