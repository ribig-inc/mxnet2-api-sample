// test.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <windows.h>
#include <stdio.h>
#include <process.h>

#include ".\mxnet2api.h"


#if defined(_WIN64)
	#pragma comment(lib,"lib\\x64\\mxnet2st_mt2022.lib")
#else
	#pragma comment(lib,"lib\\x86\\mxnet2st_mt2022.lib")
#endif

const int   USERCODE = 1234;
const short APPSLOT = 7;
const DWORD  INTERVAL = 5000;  //ms

const int EVT_TIMER = 0;
const int EVT_KEY_PRESS = 1;

HANDLE hEvent[2] = { NULL, NULL };  //0 timer, 1 key press
HANDLE g_timer_handle;


/*
	プログラム起動時のライセンス取得用
		detect_key
		getLicense
*/

static unsigned __stdcall detect_key(void* pParam)
{
	printf("Establishing a session\n");

	short ret = rInit_MatrixAPI();
	printf("\tInit_MatrixAPI=%d\n", ret);
	if( ret < 0 ) return -1;

	ret = rDongle_Count(85);
	printf("\tDongle_Count=%d\n", ret);
	if (ret <= 0)
	{
		rRelease_MatrixAPI();
		return -1;
	}

	int serNr = rDongle_ReadSerNr(USERCODE, 1, 85);
	printf("\tSerNr=%d\n", serNr);
	if (serNr <= 0)
	{
		rRelease_MatrixAPI();
		return -1;
	}

	ret = rLogIn_MatrixNet(USERCODE, APPSLOT, 1);
	printf("\tLogin=%d\n", ret);
	if (ret < 0)
	{
		rRelease_MatrixAPI();
		return -1;
	}

	printf("\n");
	return 0;
}

//detect_keyスレッドで　MxNet2 API呼び出し
//ライセンス取得に成功 - > TRUE
BOOL getLicense()
{
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, detect_key, NULL, 0, NULL);
	
	if (hThread == NULL) return FALSE;

	if (WAIT_OBJECT_0 != WaitForSingleObject(hThread, INFINITE))
		return FALSE;

	DWORD dwExitCode;
	if (GetExitCodeThread(hThread, &dwExitCode) == 0)
		return FALSE;

	CloseHandle(hThread);

	if (dwExitCode == -1) return FALSE;

	return TRUE;
}
//ここまで（ライセンス取得用）


/*
	プログラム起動後のライセンス更新用
		TimerProc
		startTimer
		stopTimer
		login_thread
*/
VOID CALLBACK TimerProc(_In_ PVOID lpParameter, _In_ BOOLEAN TimerOrWaitFired)
{
	SetEvent(hEvent[EVT_TIMER]);
}

BOOL startTimer()
{
	if (!CreateTimerQueueTimer(&g_timer_handle, NULL, TimerProc, NULL, INTERVAL, INTERVAL, WT_EXECUTELONGFUNCTION))
		return FALSE;
	else
		printf("タイマー起動\n\n");

	return TRUE;
}

void stopTimer()
{
	do {
		if (DeleteTimerQueueTimer(NULL, g_timer_handle, INVALID_HANDLE_VALUE))
			break;
	} while (GetLastError() != ERROR_IO_PENDING);
}

static unsigned __stdcall LoginUpdateThread(void* pParam)
{
	unsigned nResult = 0;

	for (;;)
	{
		DWORD dwRet = WaitForMultipleObjects(2, hEvent, FALSE, INFINITE);

		if (dwRet == (WAIT_OBJECT_0 + EVT_TIMER))
		{
			short ret = rLogIn_MatrixNet(USERCODE, APPSLOT, 1);
			printf("Login=%d\n", ret);

			if (ret < 0) {
				stopTimer();
				rRelease_MatrixAPI();
				exit(-1);
			}
		}
		else
			break;
	}

	return 0;
}
// ここまで（ライセンス更新用）


int main()
{
	int		ch;
	int		nResult = -1;
	HANDLE	hLoginUpdateThread = NULL;

	//イベント作成
	hEvent[EVT_TIMER]	  = CreateEvent(NULL, FALSE , FALSE, NULL);
	hEvent[EVT_KEY_PRESS] = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (hEvent[EVT_TIMER] == NULL || hEvent[EVT_KEY_PRESS] == NULL) 
		goto MAIN_EXIT1;

	//ライセンス取得
	if (FALSE == getLicense())
		goto MAIN_EXIT1;


	//MxNet2とのセッション開始済
	//ここから先、終了前にrRelease_MatrixAPI()を呼び出す
	// 
	//rRelease_MatrixAPI()を呼び出さずに終了すると MxNet2はこのセッションを
	//session timeoutまで保持（セッションの Loginもログインした状態のまま）


	//処理スレッド
	hLoginUpdateThread = (HANDLE)_beginthreadex(NULL, 0, LoginUpdateThread, NULL, 0, NULL);
	if( hLoginUpdateThread == NULL) goto MAIN_EXIT;

	//タイマ（イベント）
	if (FALSE == startTimer())
		goto MAIN_EXIT;

	//キー入力待ち
	ch=getchar();
	printf("Key Pressed\n");

	//タイマー削除
	stopTimer();

	//キーイベントで処理スレッド終了
	SetEvent(hEvent[EVT_KEY_PRESS]);

	//処理スレッド終了待ち
	WaitForSingleObject(hLoginUpdateThread, INFINITE);

	//成功リターン
	nResult = 0;


MAIN_EXIT:

	rRelease_MatrixAPI();

MAIN_EXIT1:

	if( hEvent[EVT_TIMER] )
		CloseHandle(hEvent[EVT_TIMER]);

	if(hEvent[EVT_KEY_PRESS] )
		CloseHandle(hEvent[EVT_KEY_PRESS]);

	if( hLoginUpdateThread )
		CloseHandle(hLoginUpdateThread);

	return nResult;
}
