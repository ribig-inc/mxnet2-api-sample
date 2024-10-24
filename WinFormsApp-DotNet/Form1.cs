using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace WinFormsApp_NetCore
{
    public partial class Form1 : Form
    {
        const int MYUSERCODE = 1234;
        const short APPSLOT = 7;
        const int INTERVAL = 5000;  //ms

        private static readonly object _lock = new object();
        private static bool bExit = false;
        private static bool bTaskOver = false;

        MxNet2DotNet.API MXAPI = new MxNet2DotNet.API();

        public Form1()
        {
            InitializeComponent();
        }

        //MxNet2からライセンス取得
        private async void ConnectToMxNet2Server()
        {
            //MxNet2サーバに接続できなければ終了
            short ret = await Task.Run(() => MXAPI.Init());
            listView1.Items.Add(new ListViewItem("Init=" + ret));
            if (ret < 0) Application.Exit();

            //USBキーが接続されていない/エラーで終了
            ret = await Task.Run(() => MXAPI.Count());
            listView1.Items.Add(new ListViewItem("Count=" + ret));
            if (ret <= 0)
            {
                MXAPI.Release();
                Application.Exit();
            }

            //必要であればシリアル取得
            int SerNr = MXAPI.ReadSerNr(MYUSERCODE, 1);
            listView1.Items.Add(new ListViewItem("SerNr=" + SerNr));
            if (SerNr < 0)
            {
                MXAPI.Release();
                Application.Exit();
            }

            //ライセンス取得できない/エラーで終了
            ret = MXAPI.LogIn(MYUSERCODE, APPSLOT, 1);
            listView1.Items.Add(new ListViewItem("Login=" + ret));
            if (ret < 0)
            {
                MXAPI.Release();
                Application.Exit();
            }

            listView1.Items[listView1.Items.Count - 1].Selected = true;
            listView1.Select();
            listView1.EnsureVisible(listView1.Items.Count - 1);

        }

        //MxNet2からライセンス取得
        private async void DisonnectFromMxNet2Server()
        {
            // releaseでログイン状態は自動ですべてログアウト           
            short ret = await Task.Run(() => MXAPI.Release());
            listView1.Items.Add(new ListViewItem("Release=" + ret));

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            ConnectToMxNet2Server();

            Task.Run(() =>
            {
                lock (_lock)
                {
                    while (true)
                    {
                        Monitor.Wait(_lock, INTERVAL);

                        if (bExit == true) break;

                        short ret = MXAPI.LogIn(MYUSERCODE, APPSLOT, 1);
                        listView1.Items.Add(new ListViewItem("Login=" + ret));

                        listView1.Items[listView1.Items.Count - 1].Selected = true;
                        listView1.Select();
                        listView1.EnsureVisible(listView1.Items.Count - 1);

                        if (ret < 0) Application.Exit();
                    }
                }

                lock (_lock)
                {
                    Monitor.Pulse(_lock);
                }

            });

        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            lock (_lock)
            {
                bExit = true;
                Monitor.Pulse(_lock);
            }

            lock (_lock)
            {
                Monitor.Wait(_lock);
            }

            DisonnectFromMxNet2Server();
        }

    }
}
