using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
//using System.Linq;
using System.Text;
//using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;
using System.Runtime.InteropServices;
using System.Threading.Tasks;

namespace testcsharp
{
    public partial class Form1 : Form
    {
        const int   MYUSERCODE = 1234;
        const short APPSLOT = 5;

        bool bStarted = false;
        bool bAbort = false;
        int nCountOver = 0;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
        }

        void onStart()
        {
                Parallel.For(0, 3,  async id =>
                {
                    MxNet2DotNet.API MXAPI = new MxNet2DotNet.API();
                    ListBox listBox;

                    if (id == 0)
                        listBox = listBox1;
                    else if (id == 1)
                        listBox = listBox2;
                    else if (id == 2)
                        listBox = listBox3;
                    else
                        return;

                    nCountOver = 0;

                    for (int loop = 0; loop < 100; loop++)
                    {
                        Application.DoEvents();

                        // MXAPI.Ini() はタイムアウトするまでブロック
                        // これでウィンドウが固まらない
                        short r = await Task.Run(() => MXAPI.Init());

                        if (bAbort == true) break;

                        listBox.Items.Add(loop + ")rInit_MatrixAPI()=" + r);
                        Application.DoEvents();

                        if (r < 0)
                            break;

                        if (bAbort == true) break;

                        int SerNr = MXAPI.ReadSerNr(MYUSERCODE, 1);
                        listBox.Items.Add(loop + ")rDongle_ReadSerNr=" + SerNr);
                        listBox.SelectedIndex = listBox.Items.Count - 1;
                        Application.DoEvents();

                        if (bAbort == true)
                        {
                            MXAPI.Release();
                            break;
                        }

                        int[] data;
                        data = new int[16];

                        r = MXAPI.ReadData((int)MYUSERCODE, data, (short)16, (short)1);
                        listBox.Items.Add(loop + ")rDongle_ReadData=" + r);
                        listBox.SelectedIndex = listBox.Items.Count - 1;
                        Application.DoEvents();

                        if (bAbort == true)
                        {
                            MXAPI.Release();
                            break;
                        }

                        string s = "";
                        for (int i = 0; i < 16; i++)
                            s += data[i] + ":";

                        listBox.Items.Add(s);

                        int[] t = new int[2];
                        int rr = MXAPI.GetDetectTime((int)MYUSERCODE, t, (short)1);
                        if (rr > 0)
                        {
                            UInt64 ii = (UInt64)(((UInt64)t[1] << 32 & (UInt64)0xffffffff00000000) | (UInt64)t[0] & 0xffffffff);

                            listBox.Items.Add(loop + ")rDongle_DetectTime=" + ii);
                            listBox.SelectedIndex = listBox.Items.Count - 1;
                            Application.DoEvents();
                        }

                        if (bAbort == true)
                        {
                            MXAPI.Release();
                            break;
                        }

                        r = MXAPI.LogIn(MYUSERCODE, APPSLOT, 1);
                        listBox.Items.Add(loop + ")rLogin_MatrixNet()=" + r);
                        listBox.SelectedIndex = listBox.Items.Count - 1;
                        Application.DoEvents();

                        if (bAbort == true)
                        {
                            MXAPI.Release();
                            break;
                        }

                        
                        string ClientNames = "";
                        r = (short)MXAPI.GetLoginCountEx(MYUSERCODE, (short)APPSLOT, (short)1, out ClientNames);

                        if (ClientNames != null && ClientNames.Length > 0)
                        {
                            string[] aClientNames = ClientNames.Split('\t');

                            foreach (string n in aClientNames)
                            {
                                if (n.Length > 0)
                                {
                                    listBox.Items.Add(loop + ")Client Name=" + n);
                                    listBox.SelectedIndex = listBox.Items.Count - 1;
                                    Application.DoEvents();
                                }
                            }
                        }

                        if (bAbort == true)
                        {
                            MXAPI.Release();
                            break;
                        }


                        r = MXAPI.LogOut(MYUSERCODE, APPSLOT, 1);
                        listBox.Items.Add(loop + ")rLogOut_MatrixNet()=" + r);
                        listBox.SelectedIndex = listBox.Items.Count - 1;
                        Application.DoEvents();

                        if (bAbort == true)
                        {
                            MXAPI.Release();
                            break;
                        }

                        r = MXAPI.Release();
                        listBox.Items.Add(loop + ")rRelease_MatrixAPI()=" + r);
                        listBox.SelectedIndex = listBox.Items.Count - 1;
                        Application.DoEvents();

                        if (bAbort == true)
                            break;

                    }

                    listBox.Items.Add("Over");
                    listBox.SelectedIndex = listBox.Items.Count - 1;

                    nCountOver++;

                    if( nCountOver == 3)
                    {
                        btn_start.Text = "Start";
                        bStarted = false;
                    }
                });
        }


        private void btn_start_Click(object sender, EventArgs e)
        {

            if (bStarted == false)
            {
                listBox1.Items.Clear();
                listBox2.Items.Clear();
                listBox3.Items.Clear();

                bAbort = false;

                onStart();

                bStarted = true;
                btn_start.Text = "Stop";
            }
            else
            {
                bAbort= true;
            }
        }
    }
}
