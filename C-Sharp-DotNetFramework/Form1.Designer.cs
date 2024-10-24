namespace testcsharp
{
    partial class Form1
    {
        /// <summary>
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows フォーム デザイナーで生成されたコード

        /// <summary>
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            this.listBox1 = new System.Windows.Forms.ListBox();
            this.btn_start = new System.Windows.Forms.Button();
            this.listBox2 = new System.Windows.Forms.ListBox();
            this.listBox3 = new System.Windows.Forms.ListBox();
            this.SuspendLayout();
            // 
            // listBox1
            // 
            this.listBox1.FormattingEnabled = true;
            this.listBox1.HorizontalScrollbar = true;
            this.listBox1.ItemHeight = 21;
            this.listBox1.Location = new System.Drawing.Point(17, 71);
            this.listBox1.Margin = new System.Windows.Forms.Padding(6, 5, 6, 5);
            this.listBox1.Name = "listBox1";
            this.listBox1.Size = new System.Drawing.Size(349, 319);
            this.listBox1.TabIndex = 0;
            // 
            // btn_start
            // 
            this.btn_start.Location = new System.Drawing.Point(339, 12);
            this.btn_start.Name = "btn_start";
            this.btn_start.Size = new System.Drawing.Size(443, 33);
            this.btn_start.TabIndex = 4;
            this.btn_start.Text = "Start";
            this.btn_start.UseVisualStyleBackColor = true;
            this.btn_start.Click += new System.EventHandler(this.btn_start_Click);
            // 
            // listBox2
            // 
            this.listBox2.FormattingEnabled = true;
            this.listBox2.HorizontalScrollbar = true;
            this.listBox2.ItemHeight = 21;
            this.listBox2.Location = new System.Drawing.Point(393, 71);
            this.listBox2.Margin = new System.Windows.Forms.Padding(6, 5, 6, 5);
            this.listBox2.Name = "listBox2";
            this.listBox2.Size = new System.Drawing.Size(349, 319);
            this.listBox2.TabIndex = 5;
            // 
            // listBox3
            // 
            this.listBox3.FormattingEnabled = true;
            this.listBox3.HorizontalScrollbar = true;
            this.listBox3.ItemHeight = 21;
            this.listBox3.Location = new System.Drawing.Point(769, 71);
            this.listBox3.Margin = new System.Windows.Forms.Padding(6, 5, 6, 5);
            this.listBox3.Name = "listBox3";
            this.listBox3.Size = new System.Drawing.Size(349, 319);
            this.listBox3.TabIndex = 6;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(11F, 21F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1139, 420);
            this.Controls.Add(this.listBox3);
            this.Controls.Add(this.listBox2);
            this.Controls.Add(this.btn_start);
            this.Controls.Add(this.listBox1);
            this.Margin = new System.Windows.Forms.Padding(6, 5, 6, 5);
            this.Name = "Form1";
            this.Text = "クライアントサンプル";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListBox listBox1;
        private System.Windows.Forms.Button btn_start;
        private System.Windows.Forms.ListBox listBox2;
        private System.Windows.Forms.ListBox listBox3;
    }
}

