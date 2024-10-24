#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <syslog.h>
#include <sys/file.h>

#include "mxnet2api.h"

const int USERCODE=34219;
const short APPSLOT=9;
const int INTERVAL=5;

bool exitFlag=false;

int instanceNo=0;

sigset_t set;
struct itimerspec itval;
pthread_t loginUpdateThread;


void handler(int signum)
{
	char szSignal[16];
	switch( signum)
	{
	case 1:   //SIGHUP
		strcpy( szSignal, "SIGHUP");
		break;
	case 2:   //SIGINT
		strcpy( szSignal, "SIGINT");
		break;
	case 9:   //SIGKILL
		strcpy( szSignal, "SIGKILL");
		break;
	case 15:  //SIGTERM
		strcpy( szSignal, "SIGTERM");
		break;
	}
	syslog( LOG_PID | LOG_USER | LOG_NOTICE, "[%d] %s raised. apitest is terminating", instanceNo, szSignal);

    exitFlag = true;
    pthread_kill(loginUpdateThread, SIGUSR1);
}

void *login_update_thread(void* pParam)
{
    int         counter=0;

    int sig;
    timer_t tid;
    struct sigevent se;

    itval.it_value.tv_sec = INTERVAL;
    itval.it_value.tv_nsec = 0;
    itval.it_interval.tv_sec = INTERVAL;
    itval.it_interval.tv_nsec = 0;

    // タイマー満了で SIGALRM
    se.sigev_notify = SIGEV_THREAD_ID;
    se.sigev_signo = SIGALRM;
    se._sigev_un._tid = syscall(SYS_gettid);

    timer_create(CLOCK_REALTIME, &se, &tid);

    //arm timer
    timer_settime(tid, 0, &itval, NULL);

    for (;;)
    {
        sigwait(&set, &sig);

        //シグナル受信
        if (exitFlag == true)
            pthread_exit(NULL);

        short ret = rLogIn_MatrixNet( USERCODE, APPSLOT, 1);
        //printf("%d.login=%d\n",++counter, ret);
	    syslog( LOG_PID | LOG_USER | LOG_NOTICE, "[%d] %d.login=%d",instanceNo, ++counter, ret);

        //ログイン失敗で終了
        //（本来であれば Login再試行 / ret の値によって再接続などの処理をすべき）
        if( ret < 0)
        {
            rRelease_MatrixAPI();
            exit(-1);
        }

        //arm timer
        timer_settime(tid, 0, &itval, NULL);
    }
}

void *getLicense_thread(void* pParam)
{
    short       ret;
    _mxINT32    data[16];
    _mxINT32    time1[2];
    long        detectTime;
    _mxINT32    serNr, ret1;
    struct tm*  local_time;
    int*        retval = (int*)malloc(sizeof(int));

    *retval = -1;

    ret = rInit_MatrixAPI();
    //printf("init ret=%d\n", ret);
    syslog( LOG_PID | LOG_USER | LOG_NOTICE, "[%d] init ret=%d", instanceNo, ret);

    if( ret < 0 ) goto EXIT1;

    ret = rDongle_Count(85);
 //   printf("Count=%d\n", ret);
    syslog( LOG_PID | LOG_USER | LOG_NOTICE, "[%d] Count=%d", instanceNo,ret);
    if( ret < 0 )
        goto EXIT;

    serNr = rDongle_ReadSerNr(USERCODE, 1, 85);
 //   printf("SerNr=%d\n", serNr);
    syslog( LOG_PID | LOG_USER | LOG_NOTICE, "[%d] SerNr=%d", instanceNo,serNr);
    if( serNr <= 0 )
        goto EXIT;

    ret = rDongle_ReadData(USERCODE, data, 16, 1, 85);
//    printf("Read Count=%d\n", ret);
    syslog( LOG_PID | LOG_USER | LOG_NOTICE, "[%d] Read Count=%d", instanceNo, ret);
    if( ret != 16 )
        goto EXIT;

/*
    for( int i=0 ; i  < 16 ; i++)
        printf("%d:%d ", i, data[i]);
    printf("\n");
*/

	ret1=rDongle_GetDetectTime(USERCODE, time1, 1, 85);
    if( !ret1 )
        goto EXIT;

    detectTime =(long)time1[0] + ((long)(time1[1]) << 0x32);

    local_time = localtime(&detectTime);
//    printf("%s:%d年%d月%d日\n", local_time->tm_zone, local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday);
    syslog( LOG_PID | LOG_USER | LOG_NOTICE, "[%d] %s:%d年%d月%d日", instanceNo, local_time->tm_zone, local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday);
    
    ret = rLogIn_MatrixNet( USERCODE, APPSLOT, 1);
//    printf("0.login=%d\n",ret);
    syslog( LOG_PID | LOG_USER | LOG_NOTICE, "[%d] 0.login=%d",instanceNo,ret);
    if( ret < 0)
        goto EXIT;

    *retval=0;
    goto EXIT1;

EXIT:
    rRelease_MatrixAPI();

EXIT1:
    pthread_exit(retval);
}

int readInstanceCount(bool reset=false)
{
    int no=-1;
    char path[] = "/tmp/apitest";
    int  fd;

    if( reset==true)
        unlink(path);

    if( access(path, F_OK) == -1 )
        fd = open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    else
        fd = open(path, O_RDWR,  S_IRUSR | S_IWUSR);

    if( fd != -1)
    {
        /* 排他ロック */
        if (flock(fd, LOCK_EX) == 0) 
        {
            char buf[32];
            ssize_t read_count=read( fd, buf, sizeof(buf) );
            buf[read_count] = 0;

            if( read_count==0)
            {
                no=0;
                strcpy(buf,"1");
                if( -1 == write(fd,buf, 1) )
                {
                    syslog( LOG_PID | LOG_USER | LOG_ERR, "[%d] /tmp/apitest write error", instanceNo);
                    no = -4;
                }
            }
            else
            {
                no = atoi(buf);
                sprintf(buf, "%d", no+1);
                lseek(fd,0,SEEK_SET);
                if( -1 == write(fd,buf, strlen(buf)) )
                {
                    syslog( LOG_PID | LOG_USER | LOG_ERR, "[%d] /tmp/apitest write error", instanceNo);
                    no = -3;
                }
            }

            flock(fd, LOCK_UN);       
        }
        else 
        {
            no = -2;
            syslog( LOG_PID | LOG_USER | LOG_ERR, "[%d] /tmp/apitest lock error", instanceNo);
        }
        close(fd);
    }
    else
    {
        no = -1;
        syslog( LOG_PID | LOG_USER | LOG_ERR, "[%d] /tmp/apitest open error", instanceNo);
    }

    return no;
}

int main(int argc, char** argv)
{
    bool reset=false;

    //引数 reset が渡されたら /tmp/apitest 削除（ instanceNo を 0にリセット )
    if( argc > 1 && strcasecmp(argv[1],"reset")==0 )
        reset=true;

    //apitest のインスタンスを区別するための番号
    if( (instanceNo = readInstanceCount(reset)) < 0  )
        return -1;

    pthread_t getLicenseThread;
    int* retVal;
    int pc1;

    //別スレッドでライセンス取得
    int pc = pthread_create(&getLicenseThread, NULL, &getLicense_thread, NULL);
    if( pc)
    {
        //printf("pthread_create(getLicenseThread) error: %d\n", pc);
        syslog( LOG_PID | LOG_USER | LOG_ERR, "pthread_create(getLicenseThread) error: %d", pc);
        exit(-1);
    }
 
    //スレッド終了待ち
    int pj = pthread_join(getLicenseThread, (void**)&retVal);
    if( pj )
    {
//        printf("pthread_join() error: %d\n", pj);
        syslog( LOG_PID | LOG_USER | LOG_ERR, "pthread_join() error: %d", pj);
        exit(-1);
    }

    //ライセンス取得できなければ終了
    if( *retVal == -1)
    {
        free(retVal);
        exit(-1);
    }

    free(retVal);

    //
    //ここからは終了前に必ず rRelease_MatrixAPIを呼び出す
    //

    //このプログラムはバックグラウンドで動かすことを前提としているので
    //シグナルで終了させる。そのためのハンドラーセットアップ
    if (signal(SIGINT, handler) == SIG_ERR)
    {
        syslog( LOG_PID | LOG_USER | LOG_CRIT, "signal(SIGINT, handler) error");
        goto EXIT;
    }
    if (signal(SIGHUP, handler) == SIG_ERR)
    {
        syslog( LOG_PID | LOG_USER | LOG_CRIT, "signal(SIGHUP, handler) error.");
        goto EXIT;
    }

    if (signal(SIGTERM, handler) == SIG_ERR)
    {
        syslog( LOG_PID | LOG_USER | LOG_CRIT, "signal(SIGTERM, handler) error");
        goto EXIT;
    }


    // SIGUSR1（プログラム終了用）、SIGALRM（タイマー用）をセット
    sigemptyset(&set);
    sigaddset(&set, SIGALRM);
    sigaddset(&set, SIGUSR1);
    pthread_sigmask(SIG_BLOCK, &set, NULL);

    //定期的なライセンス更新
    pc1 = pthread_create(&loginUpdateThread, NULL, &login_update_thread, NULL);
    if( pc1 )
    {
        printf("pthread_create(loginUpdateThread) error: %d\n", pc);
        exit(-1);
    }

    //スレッド終了待ち
    pthread_join(loginUpdateThread,NULL);

EXIT:
    //セッション開放
    rRelease_MatrixAPI();
 
    syslog( LOG_PID | LOG_USER | LOG_NOTICE, "[%d] apitest has terminated", instanceNo);

    return 0;
}

