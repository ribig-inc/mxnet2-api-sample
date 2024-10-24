import java.io.IOException;
import java.util.Timer;
import java.util.TimerTask;
import java.util.Scanner;

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class Demo 
{
    private boolean initOk=false;
    private boolean bExit=false;
    private Thread  loginUpdate=null;

    private final int USERCODE = 1234;
    private final short APPSLOT = 7;
    private final int INTERVAL=5000;   //ms

    private final short DNG_Port=85;
    private final short DNG_Nr=1;

    class AcquireLicense implements Callable<Integer> {
	    public Integer call() throws Exception {
            int bOk=0;

            int[] DataIn = new int[32];
            
            short ret = MxNet2.rInit_MatrixAPI();
            System.out.println( "ret=" + ret);
            if( ret < 0) return bOk;
            
            ret = MxNet2.rDongle_Count(DNG_Port);
            System.out.println( "Count=" + ret);
            //ret==0（ドングルカウント 0 )はエラー
            if( ret <= 0) {
                MxNet2.rRelease_MatrixAPI();
                return bOk;
            }

            int SerNr = MxNet2.rDongle_ReadSerNr(USERCODE, DNG_Nr, DNG_Port);
            System.out.println( "SerNr=" + SerNr);
            if( SerNr <= 0) {
                MxNet2.rRelease_MatrixAPI();
                return bOk;
            }

            ret = MxNet2.rDongle_ReadData(USERCODE, DataIn, (short)16, DNG_Nr, DNG_Port);
            System.out.println( "Read Count=" + ret );
            if( ret != 16){
                MxNet2.rRelease_MatrixAPI();
                return bOk;
            }

            for( int i=0 ; i < 16 ; i++)
                System.out.println("   Field" + i + ":" + DataIn[i]);

            // LogIn_MatrixNet
            ret = MxNet2.rLogIn_MatrixNet(USERCODE, APPSLOT, DNG_Nr);
            System.out.println( "Login=" + ret );
            //ret==0（ライセンス残 0 )は成功
            if( ret < 0) {
                MxNet2.rRelease_MatrixAPI();
                return bOk;
            }

            bOk=1;
            return bOk;
	}
}

    public Demo()
    {
       if( getLicense() == false)
            System.exit(-1);

         // Wait till enter is pressed
        System.out.println( "\nEnterキーで終了");
        Scanner keyboard = new Scanner(System.in);
        try{
            String input = keyboard.nextLine();
        }catch(Exception e)
        {

        }

        System.out.println( "\nKey Pressed");

        releaseLicense();
    }

    private boolean getLicense()
    {
        //プログラムでIP/Portを指定すれば mxnetapi.ini が見つからなくても
        //サーバに接続可能
        //MxNet2.MxNet_SetIP("192.168.1.1");
        //MxNet2.MxNet_SetPort(12300);

        //メインスレッドとは別スレッド（ スレッドや Executor Service等）で
        //MxNet2サーバとやり取りする
/*
        Thread th = new Thread( () -> {

            int[] DataIn = new int[32];
            
            short ret = MxNet2.rInit_MatrixAPI();
            System.out.println( "ret=" + ret);
            if( ret < 0) return;
            
            ret = MxNet2.rDongle_Count(DNG_Port);
            System.out.println( "Count=" + ret);
            //ret==0（ドングルカウント 0 )はエラー
            if( ret <= 0) {
                MxNet2.rRelease_MatrixAPI();
                return;
            }

            int SerNr = MxNet2.rDongle_ReadSerNr(USERCODE, DNG_Nr, DNG_Port);
            System.out.println( "SerNr=" + SerNr);
            if( SerNr <= 0) {
                MxNet2.rRelease_MatrixAPI();
                return;
            }

            ret = MxNet2.rDongle_ReadData(USERCODE, DataIn, (short)16, DNG_Nr, DNG_Port);
            System.out.println( "Read Count=" + ret );
            if( ret != 16){
                MxNet2.rRelease_MatrixAPI();
                return;
            }

            for( int i=0 ; i < 16 ; i++)
                System.out.println("   Field" + i + ":" + DataIn[i]);

            // LogIn_MatrixNet
            ret = MxNet2.rLogIn_MatrixNet(USERCODE, APPSLOT, DNG_Nr);
            System.out.println( "Login=" + ret );
            //ret==0（ライセンス残 0 )は成功
            if( ret < 0) {
                MxNet2.rRelease_MatrixAPI();
                return;
            }

            initOk=true;
        } );

        th.start();
        
        try{
            th.join();
        }catch(InterruptedException e)
        {
            e.printStackTrace();
        }
*/

        int ok=0;

		ExecutorService execService = null;
		try
         {
			execService = Executors.newSingleThreadExecutor();
			Future<Integer> result = execService.submit(new AcquireLicense());
			try {
				ok = result.get();
			} catch (InterruptedException | ExecutionException e) {
				e.printStackTrace();
			}
		}
		finally
        {
			execService.shutdown();
		}

        if( ok==0) return false;

        //Start periodic Login Update
        loginUpdate = new Thread( () -> {
           while(true)
            {
                synchronized(this)
                {
                    try{
                        wait(INTERVAL);
                    }
                    catch(InterruptedException e)
                    {

                    }
                }
                    
               if( bExit==true) break;

                short ret = MxNet2.rLogIn_MatrixNet(USERCODE, APPSLOT, DNG_Nr); 
                System.out.println( "Login=" + ret );

                if( ret < 0) 
                {
                    MxNet2.rRelease_MatrixAPI();
                    System.exit(-1);
                }
            }

       } );
       loginUpdate.start();

        return true;
    }

    private void releaseLicense()
    {
        bExit=true;

        synchronized(this)
        {
            notify();
        }

        try{
            loginUpdate.join();
        }catch(InterruptedException e)
        {
            e.printStackTrace();
        }

        MxNet2.rRelease_MatrixAPI();

    }

    public static void main(String args[])
    {
        new Demo();
    }

 
}

