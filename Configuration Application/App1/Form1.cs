using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using NAudio.Wave;
using WaveFormRendererLib;


namespace App1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        Bitmap img= new Bitmap(480, 272, PixelFormat.Format16bppRgb565);
        Bitmap[] images = new Bitmap[35];
        Bitmap[] images_up = new Bitmap[35];
        Bitmap[] images_updown = new Bitmap[35];
        Bitmap[] images_down = new Bitmap[35];
        Byte[] Loaded_images = new Byte[35];


        AudioFileReader AudioFile = null;
        WaveOutEvent outputDevice = new WaveOutEvent();
        Image[] Audio_images = new Bitmap[35];        
        string[] Audios = new string[35];
        Byte[] Loaded_Audios = new Byte[35];
        int[] Audios_Index = new int[35];
        Byte[] Pre_Audios = new Byte[35];

        
        bool comboBox2_Flag=false;
        bool checkbox_Flag = false;
        int arrow = 0;



        public void ComboBox_create()
        {
            comboBox2.Items.Add("دینگ دانگ 1");                             //00
            comboBox2.Items.Add("دینگ دانگ 2");                             //01
            comboBox2.Items.Add("خوش آمدید");                               //02
            comboBox2.Items.Add("وضع اضطراری");                             //03
            comboBox2.Items.Add("خون سرد باشید برق قطع شده");              //04
            comboBox2.Items.Add("لطفا مانع بسته شدن درب نشوید");           //05
            comboBox2.Items.Add("افراد سوار شده بیش از حد مجاز");          //06
            comboBox2.Items.Add("اضافه بار");                               //07
            comboBox2.Items.Add("رستوران");                                 //08
            comboBox2.Items.Add("لابی");                                      //09
            comboBox2.Items.Add("همکف");                                     //10
            comboBox2.Items.Add("پارکینگ");                                  //11
            comboBox2.Items.Add("پارکینگ اول");                              //12
            comboBox2.Items.Add("پارکینگ دوم");                              //13
            comboBox2.Items.Add("پارکینگ سوم");                              //14
            comboBox2.Items.Add("پارکینگ چهارم");                            //15
            comboBox2.Items.Add("زیر زمین");                                 //16
            comboBox2.Items.Add("زیر زمین اول");                             //17
            comboBox2.Items.Add("زیر زمین دوم");                             //18
            comboBox2.Items.Add("زیر زمین سوم");                             //19
            comboBox2.Items.Add("زیر زمین چهارم");                           //20                             
            comboBox2.Items.Add("طبقه منفی چهار");                           //21
            comboBox2.Items.Add("طبقه منفی سه");                             //22
            comboBox2.Items.Add("طبقه منفی دو");                             //23
            comboBox2.Items.Add("طبقه منفی یک");                             //24
            comboBox2.Items.Add("طبقه اول");                                 //25
            comboBox2.Items.Add("طبقه دوم");                                 //26
            comboBox2.Items.Add("طبقه سوم");                                 //27
            comboBox2.Items.Add("طبقه چهارم");                               //28
            comboBox2.Items.Add("طبقه پنجم");                                //29
            comboBox2.Items.Add("طبقه ششم");                                 //30
            comboBox2.Items.Add("طبقه هفتم");                                //31
            comboBox2.Items.Add("طبقه هشتم");                                //32                             
            comboBox2.Items.Add("طبقه نهم");                                 //33
            comboBox2.Items.Add("طبقه دهم");                                 //34
            comboBox2.Items.Add("طبقه یازدهم");                              //35
            comboBox2.Items.Add("طبقه دوازدهم");                             //36
            comboBox2.Items.Add("طبقه سیزدهم");                              //37
            comboBox2.Items.Add("طبقه چهاردهم");                             //38
            comboBox2.Items.Add("طبقه پانزدهم");                             //39
            comboBox2.Items.Add("طبقه شانزدهم");                             //40
            comboBox2.Items.Add("طبقه هفدهم");                               //41
            comboBox2.Items.Add("طبقه هجدهم");                               //42
            comboBox2.Items.Add("طبقه نوزدهم");                              //43
        }

        public WaveFileReader Default_Sound(int n)
        {
            WaveFileReader o=null;
            switch(n)
            {
                case 0:
                    o = new WaveFileReader(App1.Properties.Resources.SDINGDONG);
                    break;

                case 1:
                    o = new WaveFileReader(App1.Properties.Resources.SDINGDONG2);
                    break;

                case 2:
                    o = new WaveFileReader(App1.Properties.Resources.SWELCOME);
                    break;

                case 3:
                    o = new WaveFileReader(App1.Properties.Resources.SEMG);
                    break;

                case 4:
                    o = new WaveFileReader(App1.Properties.Resources.SEMG2);
                    break;

                case 5:
                    o = new WaveFileReader(App1.Properties.Resources.SDO);
                    break;

                case 6:
                    o = new WaveFileReader(App1.Properties.Resources.SOVL);
                    break;

                case 7:
                    o = new WaveFileReader(App1.Properties.Resources.SOVL2);
                    break;

                case 8:
                    o = new WaveFileReader(App1.Properties.Resources.SR);
                    break;

                case 9:
                    o = new WaveFileReader(App1.Properties.Resources.SL);
                    break;

                case 10:
                    o = new WaveFileReader(App1.Properties.Resources.SGF);
                    break;

                case 11:
                    o = new WaveFileReader(App1.Properties.Resources.SP);
                    break;

                case 12:
                    o = new WaveFileReader(App1.Properties.Resources.SP1);
                    break;

                case 13:
                    o = new WaveFileReader(App1.Properties.Resources.SP2);
                    break;

                case 14:
                    o = new WaveFileReader(App1.Properties.Resources.SP3);
                    break;

                case 15:
                    o = new WaveFileReader(App1.Properties.Resources.SP4);
                    break;

                case 16:
                    o = new WaveFileReader(App1.Properties.Resources.SUG);
                    break;

                case 17:
                    o = new WaveFileReader(App1.Properties.Resources.SUG1);
                    break;

                case 18:
                    o = new WaveFileReader(App1.Properties.Resources.SUG2);
                    break;

                case 19:
                    o = new WaveFileReader(App1.Properties.Resources.SUG3);
                    break;

                case 20:
                    o = new WaveFileReader(App1.Properties.Resources.SUG4);
                    break;

                case 21:
                    o = new WaveFileReader(App1.Properties.Resources.S_4);
                    break;

                case 22:
                    o = new WaveFileReader(App1.Properties.Resources.S_3);
                    break;

                case 23:
                    o = new WaveFileReader(App1.Properties.Resources.S_2);
                    break;

                case 24:
                    o = new WaveFileReader(App1.Properties.Resources.S_1);
                    break;

                case 25:
                    o = new WaveFileReader(App1.Properties.Resources.S01);
                    break;

                case 26:
                    o = new WaveFileReader(App1.Properties.Resources.S02);
                    break;

                case 27:
                    o = new WaveFileReader(App1.Properties.Resources.S03);
                    break;

                case 28:
                    o = new WaveFileReader(App1.Properties.Resources.S04);
                    break;

                case 29:
                    o = new WaveFileReader(App1.Properties.Resources.S05);
                    break;

                case 30:
                    o = new WaveFileReader(App1.Properties.Resources.S06);
                    break;

                case 31:
                    o = new WaveFileReader(App1.Properties.Resources.S07);
                    break;

                case 32:
                    o = new WaveFileReader(App1.Properties.Resources.S08);
                    break;

                case 33:
                    o = new WaveFileReader(App1.Properties.Resources.S09);
                    break;

                case 34:
                    o = new WaveFileReader(App1.Properties.Resources.S10);
                    break;

                case 35:
                    o = new WaveFileReader(App1.Properties.Resources.S11);
                    break;

                case 36:
                    o = new WaveFileReader(App1.Properties.Resources.S12);
                    break;

                case 37:
                    o = new WaveFileReader(App1.Properties.Resources.S13);
                    break;

                case 38:
                    o = new WaveFileReader(App1.Properties.Resources.S14);
                    break;

                case 39:
                    o = new WaveFileReader(App1.Properties.Resources.S15);
                    break;

                case 40:
                    o = new WaveFileReader(App1.Properties.Resources.S16);
                    break;

                case 41:
                    o = new WaveFileReader(App1.Properties.Resources.S17);
                    break;

                case 42:
                    o = new WaveFileReader(App1.Properties.Resources.S18);
                    break;

                case 43:
                    o = new WaveFileReader(App1.Properties.Resources.S19);
                    break;


                default:
                    o = null;
                    break;
            }
            return o;
        }


        public Image Resize(Image source, int width, int height)

        {

            if (source.Width == width && source.Height == height) return source;

            var result = new Bitmap(width, height, PixelFormat.Format16bppRgb565);

            result.SetResolution(source.HorizontalResolution, source.VerticalResolution);

            using (var g = Graphics.FromImage(result))

                g.DrawImage(source, new Rectangle(0, 0, width, height), new Rectangle(0, 0, source.Width, source.Height), GraphicsUnit.Pixel);

            return result;

        }


        public Image draw_up(Image source, int width, int height)
        {
            var o = new Bitmap(480, 272, PixelFormat.Format24bppRgb);
            using (var g = Graphics.FromImage(o))
            {
                g.DrawImage(source, new Rectangle(0, 0, 480, 272), new Rectangle(0, 0, 480, 272), GraphicsUnit.Pixel);
                g.DrawImage(App1.Properties.Resources.PUP, width, height);
            }
                
            return o;      
        }


        public Image draw_down(Image source, int width, int height)
        {
            var o = new Bitmap(480, 272, PixelFormat.Format24bppRgb);
            using (var g = Graphics.FromImage(o))
            {
                g.DrawImage(source, new Rectangle(0, 0, 480, 272), new Rectangle(0, 0, 480, 272), GraphicsUnit.Pixel);
                g.DrawImage(App1.Properties.Resources.PDOWN, width, height);
            }

            return o;
        }

        public Image segments(int r)
        {     
            Image o = new Bitmap(175, 260, PixelFormat.Format24bppRgb);
            switch (r)
            {

                case 2:
                    o = App1.Properties.Resources.PDO;
                    break;

                case 3:
                    o = App1.Properties.Resources.POVL;
                    break;

                case 4:
                    o = App1.Properties.Resources.PC;
                    break;

                case 5:
                    o = App1.Properties.Resources.PP;
                    break;

                case 6:
                    o = App1.Properties.Resources.PR;
                    break;

                case 7:
                    o = App1.Properties.Resources.PL;
                    break;

                case 8:
                    o = App1.Properties.Resources.PB;
                    break;

                case 9:
                    o = App1.Properties.Resources.PGF;
                    break;

                case 10:
                    o = App1.Properties.Resources.PH;
                    break;

                case 11:
                    o = App1.Properties.Resources.P_4;
                    break;

                case 12:
                    o = App1.Properties.Resources.P_3;
                    break;

                case 13:
                    o = App1.Properties.Resources.P_2;
                    break;

                case 14:
                    o = App1.Properties.Resources.P_1;
                    break;

                case 15:
                    o = App1.Properties.Resources.P00;
                    break;

                case 16:
                    o = App1.Properties.Resources.P01;
                    break;

                case 17:
                    o = App1.Properties.Resources.P02;
                    break;

                case 18:
                    o = App1.Properties.Resources.P03;
                    break;

                case 19:
                    o = App1.Properties.Resources.P04;
                    break;

                case 20:
                    o = App1.Properties.Resources.P05;
                    break;

                case 21:
                    o = App1.Properties.Resources.P06;
                    break;

                case 22:
                    o = App1.Properties.Resources.P07;
                    break;

                case 23:
                    o = App1.Properties.Resources.P08;
                    break;

                case 24:
                    o = App1.Properties.Resources.P09;
                    break;

                case 25:
                    o = App1.Properties.Resources.P10;
                    break;

                case 26:
                    o = App1.Properties.Resources.P11;
                    break;

                case 27:
                    o = App1.Properties.Resources.P12;
                    break;

                case 28:
                    o = App1.Properties.Resources.P13;
                    break;

                case 29:
                    o = App1.Properties.Resources.P14;
                    break;

                case 30:
                    o = App1.Properties.Resources.P15;
                    break;

                case 31:
                    o = App1.Properties.Resources.P16;
                    break;

                case 32:
                    o = App1.Properties.Resources.P17;
                    break;

                case 33:
                    o = App1.Properties.Resources.P18;
                    break;

                case 34:
                    o = App1.Properties.Resources.P19;
                    break;                


                default:
                    o = null;
                    break;
            }

                    return o;
        }
        public Image WaveRender(WaveStream Stream)
        {
            Image o=new  Bitmap(430, 100, PixelFormat.Format24bppRgb);
            var renderer = new WaveFormRenderer();
            IPeakProvider myPeakProvider = new AveragePeakProvider(4);
            var soundCloudDarkBlocks = new SoundCloudBlockWaveFormSettings(Color.FromArgb(52, 52, 52), Color.FromArgb(55, 55, 55), Color.FromArgb(154, 154, 154),
                Color.FromArgb(204, 204, 204))
            { Name = "SoundCloud Darker Blocks" };

            var settings = (WaveFormRendererSettings) soundCloudDarkBlocks;
            settings.TopHeight = 50;
            settings.BottomHeight = 50;
            settings.Width = 430;
            settings.DecibelScale =false;

            o=renderer.Rend(Stream, myPeakProvider, settings);
 
            return o;
        }


        

        public bool Disk_Check()
        {
            
            string str = comboBox1.Text;
            bool o;
            if (str.Length == 0)
            {
                MessageBox.Show("کارت حافظه یافت نشد!\r لطفا ابتدا درایو کارت حافظه را انتخاب کنید.", "", MessageBoxButtons.OK, MessageBoxIcon.None,
                    MessageBoxDefaultButton.Button1, MessageBoxOptions.RightAlign | MessageBoxOptions.RtlReading);
                o = false;
            }
            else
            {
                DriveInfo di = new DriveInfo(str.Substring(0, 1));
                if (di.IsReady != true)
                {
                    MessageBox.Show("کارت حافظه در دسترس نیست!\r لطفا مجددا درایو کارت حافظه را انتخاب کنید.", "", MessageBoxButtons.OK, MessageBoxIcon.None,
                        MessageBoxDefaultButton.Button1, MessageBoxOptions.RightAlign | MessageBoxOptions.RtlReading);
                    o = false;
                }
                else if (di.DriveFormat != "FAT32")
                {

                    MessageBox.Show("فرمت کارت حافظه معتبر نیست!\r فرمت کارت حافظه باید FAT32 باشد.", "", MessageBoxButtons.OK, MessageBoxIcon.None,
                        MessageBoxDefaultButton.Button1, MessageBoxOptions.RightAlign | MessageBoxOptions.RtlReading);
                    o = false;
                }
                else o=true;
            }

            return o;

        }

        public void MusicList_Refresh()
        {
            if (Disk_Check() == true)
            {
                string str = comboBox1.Text;

                string pathString = str + "Music";


                if (!(System.IO.Directory.Exists(pathString)))
                {
                    System.IO.Directory.CreateDirectory(pathString);
                    
                }
                else
                {
                    dataGridView1.Rows.Clear();
                    for (int i = 0; i < 1000; i++)
                    {
                        string path = (pathString + @"\" + i.ToString() + ".wav");
                        if (System.IO.File.Exists(path))
                        {
                            AudioFileReader AudioFile = new AudioFileReader(path);
                            
                            
                            int rowid = dataGridView1.Rows.Add();
                            DataGridViewRow r1 = (DataGridViewRow)dataGridView1.Rows[rowid];
                            r1.Cells[4].Value = dataGridView1.RowCount.ToString();
                            r1.Cells[3].Value = i.ToString() + ".wav";
                            string ss = AudioFile.TotalTime.ToString();
                            ss=ss.Remove(0, 3);
                            ss=ss.Remove(ss.Length - 4, 4);
                            r1.Cells[2].Value = ss;
                            r1.Cells[1].Value = "حذف";
                            r1.Cells[0].Value = "پخش";
                            AudioFile.Dispose();
                        }
                    }
                }
            }
            else dataGridView1.Rows.Clear();
        }


        private void OnPlaybackStopped2(object sender, StoppedEventArgs args)
        {

            for (int i = 0; i < dataGridView1.RowCount; i++)
            {
                if(dataGridView1.Rows[i].Cells[0].Value.ToString()=="توقف")
                {
                    dataGridView1.Rows[i].Cells[1].Style.BackColor = Color.White;
                    dataGridView1.Rows[i].Cells[2].Style.BackColor = Color.White;
                    dataGridView1.Rows[i].Cells[3].Style.BackColor = Color.White;
                    dataGridView1.Rows[i].Cells[4].Style.BackColor = Color.White;
                    dataGridView1.Rows[i].Cells[0].Value = "پخش";
                }
                
            }
        }

        private void OnPlaybackStopped1(object sender, StoppedEventArgs args)
        {
            //outputDevice.Dispose();

        }



        private void button1_Click(object sender, EventArgs e)
        {
            openFileDialog1.Title = "افزودن تصویر";
            openFileDialog1.Filter = "Picture |*.bmp;*.PNG;*.jpg;*.jpeg";
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                img = (Bitmap)Resize(new Bitmap(openFileDialog1.FileName), 480, 272);
                
                listView1.SelectedItems[0].SubItems[0].BackColor = Color.SpringGreen;
                button1.Text = "تغییر تصویر";
                button2.Visible = true;
                int r = Convert.ToByte(listView1.SelectedItems[0].Index);
                Loaded_images[r]=1;                
                images[r] = img;




                images_up[r] = (Bitmap)draw_up(img, arrow, 0);
                images_down[r] = (Bitmap)draw_down(img, arrow, 192);
                images_updown[r]= (Bitmap)draw_down(images_up[r], arrow, 192);

                if (r > 4) pictureBox1.Image = images_updown[r];
                else pictureBox1.Image = images[r];
            }

        }

        private void button2_Click(object sender, EventArgs e)
        {
            int r = Convert.ToByte(listView1.SelectedItems[0].Index);

            if (Loaded_images[r] == 1)
            {
                images[r]= new Bitmap(480, 272);
                Loaded_images[r] = 0;
                pictureBox1.Image = new Bitmap(480, 272);
                listView1.SelectedItems[0].SubItems[0].BackColor = Color.White;
                button1.Text = "افزودن تصویر";
                button2.Visible = false;
            }
        

        }



        private void Form1_Load(object sender, EventArgs e)
        {
            listView1.Items[0].Selected = true;
            listView2.Items[0].Selected = true;
            comboBox1.SelectedIndex = -1;
            listView1.Select();

            ComboBox_create();

            for (int r=0;r<35;r++) Loaded_images[r] = 0;
            for (int r = 0; r < 35; r++) Loaded_Audios[r] = 0;
            for (int r = 0; r < 35; r++) Audios_Index[r] = -1;

            button2.Visible = false;
            button9.Visible = false;

            checkBox1.Visible = false;
            checkBox2.Visible = false;

            Demo_On.Checked = false;
            Demo_off.Checked = true;
            foreach (DriveInfo di in DriveInfo.GetDrives())
            {
                if(di.DriveType==DriveType.Removable)
                {
                    comboBox1.Items.Add(di.Name);
                    comboBox1.SelectedIndex = 0;
                }
                    
            }

        }

        
        

        private void listView1_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            int r = 0;
            for (r = 0; r < 34; r++)
            {
                if (listView1.Items[r].Selected == true) break;
            }
            pictureBox4.Image = segments(r);
            label2.Text = listView1.Items[r].Text;
            if (Loaded_images[r] == 1)
            {
                if(r>4) pictureBox1.Image = images_updown[r];
                else pictureBox1.Image = images[r];
                button1.Text = "تغییر تصویر";
                button2.Visible = true;
            }
            else
            {
                pictureBox1.Image = new Bitmap(480, 272);
                button1.Text = "افزودن تصویر";
                button2.Visible = false;
            }
                
     
        }

        private void button3_Click(object sender, EventArgs e)
        {
            
            
            if(Disk_Check()==true)
            {
                string str = comboBox1.Text;
                label1.Text = "در حال ذخیره سازی";
                label1.Update();
                this.Cursor = Cursors.WaitCursor;
                this.UseWaitCursor = true;
                //string str = folderBrowserDialog1.SelectedPath.Remove(2);

                string pathString = System.IO.Path.Combine(str, "theme.thm");
                using (System.IO.FileStream fs = System.IO.File.Create(pathString))
                {
                    int i = 0;
                    Bitmap tmp = new Bitmap(480, 272, PixelFormat.Format24bppRgb);
                    Color clr;
                    Byte R, G, B;
                    Byte Dl, Dh;
                    char[] ss = new char[10];
                    ss = "M.H.Najafi".ToCharArray();
                    //Write Name
                    for (i = 0; i < 10; i++) fs.WriteByte(Convert.ToByte(ss[i]));
                    //Write loaded images number
                    Dl = Convert.ToByte(arrow & 0xFF);
                    Dh = Convert.ToByte((arrow >> 8) & 0xFF);
                    fs.WriteByte(Dh);
                    fs.WriteByte(Dl);
                    for (i = 0; i < 35; i++) fs.WriteByte(Loaded_images[i]);

                    

                    tmp = (Bitmap)Resize(new Bitmap(App1.Properties.Resources.PINTRO), 480, 272);
                    pictureBox2.Image = tmp;

                    for (int m = 0; m < 272; m++)
                    {
                        for (int n = 0; n < 480; n++)
                        {
                            clr = tmp.GetPixel(n, m);
                            R = clr.R;
                            G = clr.G;
                            B = clr.B;

                            Dh = Convert.ToByte((B & 0xF8) | ((G >> 5) & 0x07));
                            Dl = Convert.ToByte(((G << 3) & 0xE0) | ((R >> 3) & 0x1F));
                            fs.WriteByte(Dh);
                            fs.WriteByte(Dl);

                        }

                    }

                    progressBar1.Value = 3;
                    //write images data
                    for (i = 0; i < 35; i++)
                    {
                        if (Loaded_images[i] == 1)
                        {
                            tmp = images[i];
                            for (int m = 0; m < 272; m++)
                            {
                                for (int n = 0; n < 480; n++)
                                {
                                    clr = tmp.GetPixel(n, m);
                                    R = clr.R;
                                    G = clr.G;
                                    B = clr.B;

                                    Dh = Convert.ToByte((B & 0xF8) | ((G >> 5) & 0x07));
                                    Dl = Convert.ToByte(((G << 3) & 0xE0) | ((R >> 3) & 0x1F));
                                    fs.WriteByte(Dh);
                                    fs.WriteByte(Dl);

                                }
                            }                      
                        }
                        progressBar1.Value = (i );
                    }

                    for (i = 5; i < 35; i++)
                    {
                        if (Loaded_images[i] == 1)
                        {
                            tmp = images_up[i];
                            for (int m = 0; m < 272; m++)
                            {
                                for (int n = 0; n < 480; n++)
                                {
                                    clr = tmp.GetPixel(n, m);
                                    R = clr.R;
                                    G = clr.G;
                                    B = clr.B;

                                    Dh = Convert.ToByte((B & 0xF8) | ((G >> 5) & 0x07));
                                    Dl = Convert.ToByte(((G << 3) & 0xE0) | ((R >> 3) & 0x1F));
                                    fs.WriteByte(Dh);
                                    fs.WriteByte(Dl);

                                }
                            }
                        }
                        progressBar1.Value = 33 + i;
                    }

                    for (i = 5; i < 35; i++)
                    {
                        if (Loaded_images[i] == 1)
                        {
                            tmp = images_down[i];
                            for (int m = 0; m < 272; m++)
                            {
                                for (int n = 0; n < 480; n++)
                                {
                                    clr = tmp.GetPixel(n, m);
                                    R = clr.R;
                                    G = clr.G;
                                    B = clr.B;

                                    Dh = Convert.ToByte((B & 0xF8) | ((G >> 5) & 0x07));
                                    Dl = Convert.ToByte(((G << 3) & 0xE0) | ((R >> 3) & 0x1F));
                                    fs.WriteByte(Dh);
                                    fs.WriteByte(Dl);

                                }
                            }
                        }
                        progressBar1.Value = 66 + i;
                    }
                    

                }
                progressBar1.Value = 100;
                this.UseWaitCursor = false;
                this.Cursor = Cursors.Default;
                MessageBox.Show("ذخیره سازی تکمیل شد!", "", MessageBoxButtons.OK, MessageBoxIcon.None,
                   MessageBoxDefaultButton.Button1, MessageBoxOptions.RightAlign | MessageBoxOptions.RtlReading);
                progressBar1.Value = 0;
                label1.Text = "    ";
            }

        }

            

        private void listView1_DoubleClick(object sender, EventArgs e)
        {
            openFileDialog1.Title = "افزودن تصویر";
            openFileDialog1.Filter = "Picture |*.bmp;*.PNG;*.jpg;*.jpeg";
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                img = (Bitmap)Resize(new Bitmap(openFileDialog1.FileName), 480, 272);

                listView1.SelectedItems[0].SubItems[0].BackColor = Color.SpringGreen;
                button1.Text = "تغییر تصویر";
                button2.Visible = true;
                int r = Convert.ToByte(listView1.SelectedItems[0].Index);
                Loaded_images[r] = 1;
                images[r] = img;




                images_up[r] = (Bitmap)draw_up(img, arrow, 0);
                images_down[r] = (Bitmap)draw_down(img, arrow, 192);
                images_updown[r] = (Bitmap)draw_down(images_up[r], arrow, 192);
                if (r > 4) pictureBox1.Image = images_updown[r];
                else pictureBox1.Image = img;
                
            }
        }

        private void pictureBox2_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("http://www.rgnadin.ir");
        }

        



        private void comboBox1_DropDown(object sender, EventArgs e)
        {
            int ind = comboBox1.SelectedIndex;
            comboBox1.Items.Clear();
            foreach (DriveInfo d in DriveInfo.GetDrives())
            {

                if (d.DriveType == DriveType.Removable)
                {
                    comboBox1.Items.Add(d.Name);
                    comboBox1.SelectedIndex = ind;
                }

            }
        }

        private void button6_Click(object sender, EventArgs e)
        {
            if (Disk_Check() == true)
            {
                string str = comboBox1.Text;
                
                //string str = folderBrowserDialog1.SelectedPath.Remove(2);

                string pathString = System.IO.Path.Combine(str, "theme.thm");
                if(System.IO.File.Exists(pathString)==false)
                {
                    MessageBox.Show("هیج تم ذخیره شده ای روی کارت حافظه موجود نیست!", "", MessageBoxButtons.OK, MessageBoxIcon.None,
                        MessageBoxDefaultButton.Button1, MessageBoxOptions.RightAlign | MessageBoxOptions.RtlReading);
                    
                }
                else
                {
                    label1.Text = "در حال فراخوانی";
                    label1.Update();
                    this.Cursor = Cursors.WaitCursor;
                    this.UseWaitCursor = true;
                   
                    using (System.IO.FileStream fs = System.IO.File.Open(pathString, FileMode.Open))
                    {
                        int i = 0;
                        Byte Dl, Dh;
                        fs.Seek(10, SeekOrigin.Begin);
                        Dh = Convert.ToByte(fs.ReadByte());
                        Dl = Convert.ToByte(fs.ReadByte());
                        arrow = Convert.ToInt16((Dh << 8) | Dl);
                        if (arrow > 360) arrow = 360;

                        for (i = 0; i < 35; i++)
                        {
                            Loaded_images[i] = Convert.ToByte(fs.ReadByte());
                            if(Loaded_images[i] == 1)
                                listView1.Items[i].BackColor= Color.SpringGreen;
                            else
                                listView1.Items[i].BackColor = Color.White;
                        }

                        fs.Seek( (272 * 480 * 2), SeekOrigin.Current);

                        
                        progressBar1.Value = 3;
                        //read images data
                        for (i = 0; i < 35; i++)
                        {
                            if (Loaded_images[i] == 1)
                            {
                                Bitmap tmp = new Bitmap(480, 272, PixelFormat.Format24bppRgb);
                                Color clr;
                                Byte R, G, B;
                                
                                Int16 c;

                                for (int m = 0; m < 272; m++)
                                {
                                    for (int n = 0; n < 480; n++)
                                    {
                                        Dh = Convert.ToByte( fs.ReadByte() );
                                        Dl = Convert.ToByte( fs.ReadByte() );
                                        byte[] data = { Dl, Dh };
                                        c = BitConverter.ToInt16(data, 0);

                                        R = Convert.ToByte( (c & 0x1F) << 3 );
                                        
                                        G = Convert.ToByte( (c >> 3 ) & 0xFF);
                                        
                                        B = Convert.ToByte( ( (c & 0xF800) >> 8 ) & 0xFF);

                                        B += 1;


                                        clr = Color.FromArgb(R, G, B);
                                        tmp.SetPixel(n, m, clr);
                                    }
                                }

                                images[i]=tmp;
                            }
                            progressBar1.Value =Convert.ToInt16( i  * 2.8);
                        }

                    }
                    progressBar1.Value = 100;
                    this.UseWaitCursor = false;
                    this.Cursor = Cursors.Default;


                    for (int i = 0; i < 35; i++)
                    {
                        if (Loaded_images[i] == 1)
                        {

                            images_up[i] = (Bitmap)draw_up(images[i], arrow, 0);
                            images_down[i] = (Bitmap)draw_down(images[i], arrow, 192);
                            images_updown[i] = (Bitmap)draw_down(images_up[i], arrow, 192);
                        }
                    }

                                       
                    int r = 0;
                    for ( r = 0; r < 34; r++)
                    {
                        if (listView1.Items[r].Selected == true) break;
                    }
                    label2.Text = listView1.Items[r].Text;
                    if (Loaded_images[r] == 1)
                    {
                        if (r > 4) pictureBox1.Image = images_updown[r];
                        else pictureBox1.Image = images[r];
                        button1.Text = "تغییر تصویر";
                        button2.Visible = true;
                    }
                    else
                    {
                        pictureBox1.Image = new Bitmap(480, 272);
                        button1.Text = "افزودن تصویر";
                        button2.Visible = false;
                    }

                    MessageBox.Show("فراخوانی تکمیل شد!", "", MessageBoxButtons.OK, MessageBoxIcon.None,
                        MessageBoxDefaultButton.Button1, MessageBoxOptions.RightAlign | MessageBoxOptions.RtlReading);

                    progressBar1.Value = 0;
                    label1.Text = "    ";
                }

            }



        }

        private void button9_Click(object sender, EventArgs e)
        {
            comboBox2_Flag = true;
            comboBox2.SelectedIndex = -1;
            
            int r = Convert.ToByte(listView2.SelectedItems[0].Index);

            if (Loaded_Audios[r] == 1)
            {
                //Audios[r] = new MediaFoundationResampler(new IWaveProvider, 32000);
                Loaded_Audios[r] = 0;
                Audios[r] = null;
                Audios_Index[r] = -1;
                listView2.SelectedItems[0].SubItems[0].BackColor = Color.White;
                button10.Text = "افزودن صدا";
                button9.Visible = false;
                button11.Visible = false;
                pictureBox3.Image = null;
                Audio_images[r] = null;
            }
            comboBox2_Flag = false;
            
        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {

            //pictureBox1.Image = img;
            if (comboBox2_Flag==false)
            {
                if(comboBox2.SelectedIndex!=-1)
                {
                    listView2.SelectedItems[0].SubItems[0].BackColor = Color.SpringGreen;
                    int r = Convert.ToByte(listView2.SelectedItems[0].Index);
                    Loaded_Audios[r] = 1;
                    WaveFormat Fmt = new WaveFormat(32000, 16, 1);
                    Audios[r] = null;
                    Audios_Index[r] = comboBox2.SelectedIndex;                    
                    button9.Visible = true;
                    button11.Visible = true;
                    button10.Text = "تغییر صدا";
                    Audio_images[r] = WaveRender(Default_Sound(comboBox2.SelectedIndex));
                    pictureBox3.Image = Audio_images[r];
                }                

            }
            comboBox2_Flag = false;

        }

        private void listView2_SelectedIndexChanged(object sender, EventArgs e)
        {
            int r = 0;
            for (r = 0; r < 34; r++)
            {
                if (listView2.Items[r].Selected == true) break;

            }

            checkbox_Flag = true;
            if (r>4)
            {
                checkBox2.Visible = true;
                checkBox1.Visible = true;
            }
            else
            {
                checkBox1.Visible = false;
                checkBox2.Visible = false;
            }
            
            switch(Pre_Audios[r])
            {
                case 0:
                    checkBox2.Checked = false;
                    checkBox1.Checked = false;
                    break;
                case 1:
                    checkBox2.Checked = false;
                    checkBox1.Checked = true;
                    break;

                case 2:
                    checkBox2.Checked = true;
                    checkBox1.Checked = false;
                    break;

                default:
                    checkBox2.Checked = true;
                    checkBox1.Checked = true;
                    break;
            }
            

            label6.Text = listView2.Items[r].Text;
            if (r != 4) pictureBox5.Image = segments(r);
            else pictureBox5.Image = null;
            comboBox2_Flag = true;
            comboBox2.SelectedIndex = Audios_Index[r];

            if (Loaded_Audios[r] == 1)
            {
                button9.Visible = true;
                button11.Visible =true;
                button10.Text = "تغییر صدا";
                pictureBox3.Image = Audio_images[r];

            }
            else
            {
                button10.Text = "افزودن صدا";
                button9.Visible = false;
                button11.Visible = false;
                pictureBox3.Image = null;
            }
            comboBox2_Flag = false;
            checkbox_Flag = false;
        }

        private void button10_Click(object sender, EventArgs e)
        {
            openFileDialog1.Title = "افزودن صدا";
            openFileDialog1.Filter = "Audio Files |*.mp3; *.wav";
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {

                using (MediaFoundationReader reader= new MediaFoundationReader(openFileDialog1.FileName))
                {
                    
                    WaveFormat Fmt = new WaveFormat(32000, 16, 1);
                    if (reader.TotalTime.TotalSeconds < 5)
                    {
                        int r = Convert.ToByte(listView2.SelectedItems[0].Index);
                        Audios[r] = openFileDialog1.FileName;
                        Audio_images[r] = WaveRender(reader);
                        pictureBox3.Image = Audio_images[r];
                        comboBox2_Flag = true;
                        comboBox2.SelectedIndex = -1;
                        listView2.SelectedItems[0].SubItems[0].BackColor = Color.SpringGreen;
                        Loaded_Audios[r] = 1;
                        Audios_Index[r] = -1;
                        button9.Visible = true;
                        button11.Visible = true;
                        button10.Text = "تغییر صدا";
                        comboBox2_Flag = false;
                    }

                    else
                        MessageBox.Show("طول فایل صوتی نباید بیشتر از 5 ثانیه باشد!", "", MessageBoxButtons.OK, MessageBoxIcon.None,
                            MessageBoxDefaultButton.Button1, MessageBoxOptions.RightAlign | MessageBoxOptions.RtlReading);

                }

            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            outputDevice.Stop();
            int add= 0;
            if(Disk_Check()==true)
            {
                openFileDialog1.Title = "افزودن موزیک";
                openFileDialog1.Filter = "Audio Files |*.mp3; *.wav";
                if (openFileDialog1.ShowDialog() == DialogResult.OK)
                {
                    label1.Text = "در حال افزودن موزیک";
                    label1.Update();

                    this.Cursor = Cursors.WaitCursor;
                    this.UseWaitCursor = true;
                    
                    try
                    {
                        using (Mp3FileReader reader = new Mp3FileReader(openFileDialog1.FileName))
                        {
                            WaveFormat Fmt = new WaveFormat(32000, 16, 1);
                            progressBar1.Value = 10;
                            int i = 0;
                            for (i = 1; i < 1000; i++)
                            {
                                string path = comboBox1.Text + "Music" + @"\" + i.ToString() + ".wav";
                                if (!(System.IO.File.Exists(path)))
                                {
                                    using (var resampler = new MediaFoundationResampler(reader, Fmt))
                                    {
                                        WaveFileWriter.CreateWaveFile(path, resampler);
                                        progressBar1.Value = 100;
                                    }
                                    add = i;
                                    break;
                                }
                            }
                        }
                    }
                    catch
                    {
                        using (WaveFileReader reader = new WaveFileReader(openFileDialog1.FileName))
                        {
                            WaveFormat Fmt = new WaveFormat(32000, 16, 1);
                            progressBar1.Value = 10;
                            int i = 0;
                            for (i = 1; i < 1000; i++)
                            {
                                string path = comboBox1.Text + "Music" + @"\" + i.ToString() + ".wav";
                                if (!(System.IO.File.Exists(path)))
                                {
                                    using (var resampler = new MediaFoundationResampler(reader, Fmt))
                                    {
                                        WaveFileWriter.CreateWaveFile(path, resampler);
                                        progressBar1.Value = 100;
                                    }
                                    add = i;
                                    break;
                                }
                            }
                        }

                    }
                    
                    MusicList_Refresh();

                    dataGridView1.Rows[add-1].Selected = true;
                    dataGridView1.FirstDisplayedScrollingRowIndex = add - 1;
                    this.UseWaitCursor = false;
                    this.Cursor = Cursors.Default;
                    label1.Text = "    ";
                    progressBar1.Value = 0;
                }
            }      
            
        }

        private void button5_Click(object sender, EventArgs e)
        {
            MusicList_Refresh();
            outputDevice.Stop();         
        }

        private void tabControl1_SelectedIndexChanged(object sender, EventArgs e)
        {
            outputDevice.Stop();
            outputDevice.Dispose();
            if (tabControl1.SelectedIndex == 2)
            {
                MusicList_Refresh();
            }
            else if(tabControl1.SelectedIndex==3)
            {
                if(Demo_On.Checked==true)
                {
                    groupBox13.Enabled = false;
                    groupBox9.Enabled = false;
                    groupBox1.Enabled = false;
                }
                else
                {
                    groupBox13.Enabled = true;
                    groupBox9.Enabled = true;
                    groupBox1.Enabled = true;
                }
            }
        }

        private void dataGridView1_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            if(e.RowIndex>-1)
            {
                if (e.ColumnIndex == 1)
                {
                    outputDevice.Stop();
                    outputDevice.Dispose();
                    if (AudioFile != null) AudioFile.Dispose();
                    int i = e.RowIndex + 1;
                    string path = comboBox1.Text + "Music" + @"\" + dataGridView1.Rows[e.RowIndex].Cells[3].Value.ToString();
                    this.UseWaitCursor = true;
                    System.IO.File.Delete(path);
                    MusicList_Refresh();
                    this.UseWaitCursor = false;

                }
                else if (e.ColumnIndex == 0)
                {
                    if (dataGridView1.Rows[e.RowIndex].Cells[0].Value.ToString() == "توقف")
                    {
                        outputDevice.Stop();
                        outputDevice.Dispose();

                    }
                    else
                    {
                        outputDevice.Stop();
                        outputDevice.Dispose();
                        string path = comboBox1.Text + "Music" + @"\" + dataGridView1.Rows[e.RowIndex].Cells[3].Value.ToString();
                        AudioFile = new AudioFileReader(path);
                        outputDevice.PlaybackStopped += OnPlaybackStopped2;
                        outputDevice.Init(AudioFile);
                        outputDevice.Play();
                        dataGridView1.Rows[e.RowIndex].Cells[1].Style.BackColor = Color.Salmon;
                        dataGridView1.Rows[e.RowIndex].Cells[2].Style.BackColor = Color.Salmon;
                        dataGridView1.Rows[e.RowIndex].Cells[3].Style.BackColor = Color.Salmon;
                        dataGridView1.Rows[e.RowIndex].Cells[4].Style.BackColor = Color.Salmon;
                        dataGridView1.Rows[e.RowIndex].Cells[0].Value = "توقف";

                    }

                }
            }
           
       

        }

        private void button11_Click(object sender, EventArgs e)
        {

            outputDevice.Stop();
            outputDevice.Dispose();
            if (comboBox2.SelectedIndex > -1)
            {
                var d = new MediaFoundationResampler(Default_Sound(comboBox2.SelectedIndex), new WaveFormat(32000, 16, 1));
                    
                outputDevice.Init(d);
                outputDevice.Play();
                outputDevice.PlaybackStopped += OnPlaybackStopped1;
            }
            else
            {
                int r = Convert.ToByte(listView2.SelectedItems[0].Index);
                var reader = new AudioFileReader(Audios[r]);
                outputDevice.Init(reader);
                outputDevice.Play();
                outputDevice.PlaybackStopped += OnPlaybackStopped1;

            }
            
        }

        private void button8_Click(object sender, EventArgs e)
        {
            if (Disk_Check() == true)
            {
                
                label1.Text = "در حال ذخیره سازی";
                label1.Update();
                this.Cursor = Cursors.WaitCursor;
                this.UseWaitCursor = true;
                string pathString = comboBox1.Text + "Sound";
                outputDevice.Stop();
                outputDevice.Dispose();
               

                if (!(System.IO.Directory.Exists(pathString)))
                {
                    System.IO.Directory.CreateDirectory(pathString);

                }

                pathString = pathString + @"\" + "config.cnf";
                using (System.IO.FileStream fs = System.IO.File.Create(pathString))
                {
                    int i = 0;
                    char[] ss = new char[10];
                    ss = "M.H.Najafi".ToCharArray();
                    //Write Name
                    for (i = 0; i < 10; i++) fs.WriteByte(Convert.ToByte(ss[i]));
                    //Write loaded images number
                    for (i = 0; i < 35; i++) fs.WriteByte(Pre_Audios[i]);
                }

                    for (int i=0;i<35;i++)
                {
                    string path = comboBox1.Text + "Sound" + @"\" + i.ToString() + ".wav";
                    if (Audios_Index[i]>-1)
                    {
                        
                        var d = new MediaFoundationResampler(Default_Sound(Audios_Index[i]), new WaveFormat(32000, 16, 1));
                        WaveFileWriter.CreateWaveFile(path, d);
                    }                    
                    else if (Audios[i] != null)
                    {
                        if(path!=Audios[i])
                        {
                            var reader = new MediaFoundationReader(Audios[i]);
                            var d = new MediaFoundationResampler(reader, new WaveFormat(32000, 16, 1));
                            WaveFileWriter.CreateWaveFile(path, d);
                        }                        
                    }
                    else
                    {
                        if (System.IO.File.Exists(path) == true)
                            System.IO.File.Delete(path);
                    }

                    progressBar1.Value = Convert.ToInt16(i * 2.8);
                }

                progressBar1.Value = 100;
                this.UseWaitCursor = false;
                this.Cursor = Cursors.Default;
                MessageBox.Show("ذخیره سازی تکمیل شد!", "", MessageBoxButtons.OK, MessageBoxIcon.None,
                   MessageBoxDefaultButton.Button1, MessageBoxOptions.RightAlign | MessageBoxOptions.RtlReading);
                progressBar1.Value = 0;
                label1.Text = "    ";
            }

        }
        private void button7_Click(object sender, EventArgs e)
        {
            if (Disk_Check() == true)
            {
                string pathString = comboBox1.Text + "Sound";

                if (!(System.IO.Directory.Exists(pathString)))
                {
                    System.IO.Directory.CreateDirectory(pathString);

                }

                pathString=pathString + @"\" + "config.cnf";

                if (System.IO.File.Exists(pathString) == false)
                {
                    MessageBox.Show("هیج فایل ذخیره شده ای روی کارت حافظه موجود نیست!", "", MessageBoxButtons.OK, MessageBoxIcon.None,
                        MessageBoxDefaultButton.Button1, MessageBoxOptions.RightAlign | MessageBoxOptions.RtlReading);

                }
                else
                {
                    label1.Text = "در حال فراخوانی";
                    label1.Update();
                    this.Cursor = Cursors.WaitCursor;
                    this.UseWaitCursor = true;

                    using (System.IO.FileStream fs = System.IO.File.Open(pathString, FileMode.Open))
                    {
                        int i = 0;
                        fs.Seek(10, SeekOrigin.Begin);
                        for (i = 0; i < 35; i++)
                        {
                            Pre_Audios[i] = Convert.ToByte(fs.ReadByte());
                        }
                    }

                    for (int i = 0; i < 35; i++)
                    {
                        string path = comboBox1.Text + "Sound" + @"\" + i.ToString() + ".wav";
                        if (System.IO.File.Exists(path) == true)
                        {

                            using (var reader = new MediaFoundationReader(path))
                            {
                                Audios[i] = path;
                                Audio_images[i] = WaveRender(reader);
                                listView2.Items[i].BackColor = Color.SpringGreen;
                                Loaded_Audios[i] = 1;
                                Audios_Index[i] = -1;
                            }

                        }
                        else
                        {
                            Audios[i] = null;
                            Audio_images[i] = null;
                            listView2.Items[i].BackColor = Color.White;
                            Loaded_Audios[i] = 0;
                            Audios_Index[i] = -1;
                        }

                        progressBar1.Value =Convert.ToInt16 (i * 2.8);
                    }



                    progressBar1.Value = 100;
                    this.UseWaitCursor = false;
                    this.Cursor = Cursors.Default;

                    MessageBox.Show("فراخوانی تکمیل شد!", "", MessageBoxButtons.OK, MessageBoxIcon.None,
                        MessageBoxDefaultButton.Button1, MessageBoxOptions.RightAlign | MessageBoxOptions.RtlReading);

                    int r = Convert.ToByte(listView2.SelectedItems[0].Index);
                    if (Loaded_Audios[r] == 1)
                    {
                        comboBox2_Flag = true;
                        comboBox2.SelectedIndex = -1;
                        button9.Visible = true;
                        button11.Visible = true;
                        button10.Text = "تغییر صدا";
                        pictureBox3.Image = Audio_images[r];
                        comboBox2_Flag = false;
                    }
                    else
                    {
                        button10.Text = "افزودن صدا";
                        button9.Visible = false;
                        button11.Visible = false;
                        pictureBox3.Image = null;
                    }

                    checkbox_Flag = true;
                    switch (Pre_Audios[r])
                    {
                        case 0:
                            checkBox2.Checked = false;
                            checkBox1.Checked = false;
                            break;
                        case 1:
                            checkBox2.Checked = false;
                            checkBox1.Checked = true;
                            break;

                        case 2:
                            checkBox2.Checked = true;
                            checkBox1.Checked = false;
                            break;

                        default:
                            checkBox2.Checked = true;
                            checkBox1.Checked = true;
                            break;
                    }
                    checkbox_Flag = false;

                    progressBar1.Value = 0;
                    label1.Text = "    ";

                }
            }
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if(checkbox_Flag==false)
            {
                int r = 0;
                for (r = 0; r < 34; r++)
                {
                    if (listView2.Items[r].Selected == true) break;
                }

                if (checkBox1.Checked == true && checkBox2.Checked == true) Pre_Audios[r] = 3;
                else if (checkBox1.Checked == false && checkBox2.Checked == true) Pre_Audios[r] = 2;
                else if (checkBox1.Checked == true && checkBox2.Checked == false) Pre_Audios[r] = 1;
                else Pre_Audios[r] = 0;
            }
            checkbox_Flag = false;
        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            if (checkbox_Flag == false)
            {

                int r = 0;
                for (r = 0; r < 34; r++)
                {
                    if (listView2.Items[r].Selected == true) break;
                }

                if (checkBox1.Checked == true && checkBox2.Checked == true) Pre_Audios[r] = 3;
                else if (checkBox1.Checked == false && checkBox2.Checked == true) Pre_Audios[r] = 2;
                else if (checkBox1.Checked == true && checkBox2.Checked == false) Pre_Audios[r] = 1;
                else Pre_Audios[r] = 0;
            }
            checkbox_Flag = false;
        }

        private void button13_Click(object sender, EventArgs e)
        {
            if (Disk_Check() == true)
            {

                label1.Text = "در حال ذخیره سازی";
                label1.Update();
                this.Cursor = Cursors.WaitCursor;
                this.UseWaitCursor = true;
                string pathString = comboBox1.Text + @"\"+"Setting.cnf";
                
                using (System.IO.FileStream fs = System.IO.File.Create(pathString))
                {
                    int i = 0;
                    char[] ss = new char[10];
                    ss = "M.H.Najafi".ToCharArray();
                    //Write Name
                    for (i = 0; i < 10; i++) fs.WriteByte(Convert.ToByte(ss[i]));
                    //Write loaded images number

                    fs.WriteByte(Convert.ToByte(numericUpDown1.Value));

                    if (slider_on.Checked == true) fs.WriteByte(1);
                    else fs.WriteByte(0);

                    if (menu_on.Checked == true) fs.WriteByte(1);
                    else fs.WriteByte(0);

                    if (Play_1.Checked == true) fs.WriteByte(1);
                    else if (Play_2.Checked == true) fs.WriteByte(2);
                    else fs.WriteByte(3);

                    if (Shuffle_On.Checked == true) fs.WriteByte(1);
                    else fs.WriteByte(0);

                    if (DO_On.Checked == true) fs.WriteByte(1);
                    else fs.WriteByte(0);

                    fs.WriteByte(Convert.ToByte(numericUpDown2.Value));

                    if (DO_NC.Checked == true) fs.WriteByte(1);
                    else fs.WriteByte(0);

                    if (OVL_NC.Checked == true) fs.WriteByte(1);
                    else fs.WriteByte(0);

                    if (PH_NC.Checked == true) fs.WriteByte(1);
                    else fs.WriteByte(0);

                    if (TR_NC.Checked == true) fs.WriteByte(1);
                    else fs.WriteByte(0);

                    if (Demo_On.Checked == true) fs.WriteByte(1);
                    else fs.WriteByte(0);
                }

                progressBar1.Value = 100;
                this.UseWaitCursor = false;
                this.Cursor = Cursors.Default;
                MessageBox.Show("ذخیره سازی تکمیل شد!", "", MessageBoxButtons.OK, MessageBoxIcon.None,
                   MessageBoxDefaultButton.Button1, MessageBoxOptions.RightAlign | MessageBoxOptions.RtlReading);
                progressBar1.Value = 0;
                label1.Text = "    ";

            }
        }

        private void button12_Click(object sender, EventArgs e)
        {
            if (Disk_Check() == true)
            {
                string pathString = comboBox1.Text + @"\" + "Setting.cnf";

                if (System.IO.File.Exists(pathString) == false)
                {
                    MessageBox.Show("هیج فایل ذخیره شده ای روی کارت حافظه موجود نیست!", "", MessageBoxButtons.OK, MessageBoxIcon.None,
                        MessageBoxDefaultButton.Button1, MessageBoxOptions.RightAlign | MessageBoxOptions.RtlReading);

                }
                else
                {
                    label1.Text = "در حال فراخوانی";
                    label1.Update();
                    this.Cursor = Cursors.WaitCursor;
                    this.UseWaitCursor = true;

                    using (System.IO.FileStream fs = System.IO.File.Open(pathString, FileMode.Open))
                    {
                        int i = 0;
                        fs.Seek(10, SeekOrigin.Begin);

                        i = fs.ReadByte();
                        numericUpDown1.Value = i;

                        if (fs.ReadByte() == 0) slider_off.Checked = true;
                        else slider_on.Checked = true;

                        if (fs.ReadByte() == 0) menu_off.Checked = true;
                        else menu_on.Checked = true;

                        i = fs.ReadByte();
                        if (i == 1) Play_1.Checked = true;
                        else if (i == 2) Play_2.Checked = true;
                        else Play_3.Checked = true;

                        if (fs.ReadByte() == 0) Shuffle_Off.Checked = true;
                        else Shuffle_On.Checked = true;

                        if (fs.ReadByte() == 0) DO_Off.Checked = true;
                        else DO_On.Checked = true;


                        i = fs.ReadByte();
                        numericUpDown2.Value = i;

                        if (fs.ReadByte() == 0) DO_NO.Checked = true;
                        else DO_NC.Checked = true;

                        if (fs.ReadByte() == 0) OVL_NO.Checked = true;
                        else OVL_NC.Checked = true;

                        if (fs.ReadByte() == 0) PH_NO.Checked = true;
                        else PH_NC.Checked = true;

                        if (fs.ReadByte() == 0) TR_NO.Checked = true;
                        else TR_NC.Checked = true;

                        if (fs.ReadByte() == 0) Demo_off.Checked = true;
                        else Demo_On.Checked = true;

                    }
                }

                progressBar1.Value = 100;
                this.UseWaitCursor = false;
                this.Cursor = Cursors.Default;               

                MessageBox.Show("فراخوانی تکمیل شد!", "", MessageBoxButtons.OK, MessageBoxIcon.None,
                    MessageBoxDefaultButton.Button1, MessageBoxOptions.RightAlign | MessageBoxOptions.RtlReading);

                progressBar1.Value = 0;
                label1.Text = "    ";
            }
        }

        private void Demo_On_CheckedChanged(object sender, EventArgs e)
        {
            if (Demo_On.Checked == true)
            {
                groupBox13.Enabled = false;
                groupBox9.Enabled = false;
                groupBox1.Enabled = false;
            }
            else
            {
                groupBox13.Enabled = true;
                groupBox9.Enabled = true;
                groupBox1.Enabled = true;
            }
        }

        private void Bright_Click(object sender, EventArgs e)
        {
            if (arrow < 360) arrow+=3;

            for (int i = 0; i < 35; i++)
            {
                if (Loaded_images[i] == 1)
                {

                    images_up[i] = (Bitmap)draw_up(images[i], arrow, 0);
                    images_down[i] = (Bitmap)draw_down(images[i], arrow, 192);
                    images_updown[i] = (Bitmap)draw_down(images_up[i], arrow, 192);
                }
            }

            int r = 0;
            for (r = 0; r < 34; r++)
            {
                if (listView1.Items[r].Selected == true) break;
            }

            if (Loaded_images[r] == 1)
            {
                if (r > 4) pictureBox1.Image = images_updown[r];
                else pictureBox1.Image = images[r];
                button1.Text = "تغییر تصویر";
                button2.Visible = true;
            }


        }

        private void button14_Click(object sender, EventArgs e)
        {
            if (arrow > 2) arrow -= 3;

            for (int i = 0; i < 35; i++)
            {
                if (Loaded_images[i] == 1)
                {

                    images_up[i] = (Bitmap)draw_up(images[i], arrow, 0);
                    images_down[i] = (Bitmap)draw_down(images[i], arrow, 192);
                    images_updown[i] = (Bitmap)draw_down(images_up[i], arrow, 192);
                }
            }

            int r = 0;
            for (r = 0; r < 34; r++)
            {
                if (listView1.Items[r].Selected == true) break;
            }

            if (Loaded_images[r] == 1)
            {
                if (r > 4) pictureBox1.Image = images_updown[r];
                else pictureBox1.Image = images[r];
                button1.Text = "تغییر تصویر";
                button2.Visible = true;
            }
        }
    }
}
