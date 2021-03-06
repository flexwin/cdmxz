﻿using System;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;

namespace 自动进入钉钉直播间
{
    public partial class CustomScreenshot : Form
    {
        private bool SaveDesktop;
        private string DesktopPath, path, type, DingDingClass;

        // 截图配置文件
        private string configFileDir, screenIniPath;
        private int DingDingX, DingDingY, DingDingWidth, DingDingHeight, PictureX, PictureY, PictureWidth, PictureHeight;

        public CustomScreenshot(bool saveDesk, string deskPath, string cfFileDir, string scIniPath,string dingDingClass)
        {
            SaveDesktop = saveDesk;
            DesktopPath = deskPath;
            configFileDir = cfFileDir;
            screenIniPath = scIniPath;
            DingDingClass = dingDingClass;
            InitializeComponent();
        }



        [DllImport("user32.dll", EntryPoint = "FindWindow")]
        private extern static IntPtr FindWindow(string lpClassName, string lpWindowName);
        [DllImport("user32.dll")]
        private static extern int GetWindowRect(IntPtr hwnd, out Rect lpRect);

        [DllImport("user32.dll")]
        private static extern bool SetForegroundWindow(IntPtr hWnd);

        [StructLayout(LayoutKind.Sequential)]
        public struct Rect
        {
            public int Left;
            public int Top;
            public int Right;
            public int Bottom;
        }


        private void Form1_Load(object sender, EventArgs e)
        {
            // 设置窗口透明
            pictureBox1.BackColor = Color.Red;
            this.TransparencyKey = Color.Red;
            this.BackColor = Color.Red;
            this.TopMost = true;

            string DingDingPath;
            string err = Reg.GetDingDingPath(out DingDingPath);// 获取钉钉路径
            if (err != null)
            {
                MessageBox.Show(err, "自动进入钉钉直播间", MessageBoxButtons.OK, MessageBoxIcon.Error);
                if (DingDingPath == null)
                {
                    MessageBox.Show("打开钉钉失败，请手动打开钉钉\n原因：" + err, "自动进入钉钉直播间", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    this.Close();
                }
            }

            try
            {
                Process.Start(DingDingPath);// 打开钉钉

                for (int i = 1; i <= 20; i++)// 寻找钉钉进程
                {
                    foreach (Process pro in Process.GetProcesses())
                    {
                        if (pro.ProcessName.ToLower() == "DingTalk".ToLower())
                        {
                            i = 999;
                            break;
                        }
                    }

                    if (i > 20)
                        break;
                    else if (i == 20)
                    {
                        MessageBox.Show("未找到钉钉进程，请手动打开钉钉", "自动进入钉钉直播间", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        this.Close();
                    }
                    System.Threading.Thread.Sleep(3000);// 如未找到则等待3秒再查找
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("错误\n原因：" + ex.Message, "自动进入钉钉直播间", MessageBoxButtons.OK, MessageBoxIcon.Error);
                this.Close();
            }
        }


        // 截图按钮
        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                pictureBox1.Image = null;

                Point point = PointToScreen(pictureBox1.Location);// 将控件相对于窗体坐标转为相对于屏幕坐标
                PictureX = point.X;
                PictureY = point.Y;

                // 截取指定区域
                Bitmap bit = ScreenCapture.Screenshot(PictureX, PictureY, pictureBox1.Width, pictureBox1.Height);
                // 显示到pictureBox
                pictureBox1.Image = bit;
                // 如果打开截图保存到桌面
                if (SaveDesktop)
                {
                    path = DesktopPath + DateTime.Now.ToString("yyyy-MM-dd HH_mm_ss") + ".jpg";
                    bit.Save(path);
                }
                else
                {
                    path = Environment.GetEnvironmentVariable("TEMP") + "\\自动进入钉钉直播间截图.jpg";
                    bit.Save(path);
                }

                type = "RGB";
                // 判断图片rgb颜色是否是钉钉正在直播时的rgb
                if (ScreenCapture.GetPixel(bit) == false)
                {
                    type = "OCR";
                    // 判断关键字
                    if (OCR.Live(path) == false)
                    {
                        type = "RGB和OCR";
                        label1.Text = "(" + type + ")验证失败！";
                        label2.Text = "";
                        return;
                    }
                }

                label1.Text = "(" + type + ")验证成功！\n请退出并重新打开本软件";

                // 查找钉钉窗口句柄
                IntPtr hwnd = FindWindow(DingDingClass, null);
                if (hwnd == IntPtr.Zero)
                {
                    MessageBox.Show("获取钉钉窗口句柄失败！");
                    return;
                }
                // 激活显示钉钉窗口
                SetForegroundWindow(hwnd);
                // 获取钉钉窗口坐标
                Rect re;
                if (GetWindowRect(hwnd, out re) == 0)
                {
                    MessageBox.Show("获取钉钉窗口坐标失败！");
                    return;
                }

                DingDingX = re.Left;
                DingDingY = re.Top;
                DingDingWidth = re.Right - re.Left;
                DingDingHeight = re.Bottom - re.Top;
                PictureWidth = pictureBox1.Width;
                PictureHeight = pictureBox1.Height;

                // 调整窗体高度，以便显示下面的信息
                this.Height = 185;
                label2.Text = string.Format($"图片左上角坐标：{PictureX}x{PictureY}\n图片高x宽：{PictureHeight}x{PictureWidth}\n" +
                   $"钉钉左上角坐标：{DingDingX}x{DingDingY}\n钉钉高x宽：{DingDingHeight}x{DingDingWidth}");
                // 激活窗体
                this.Activate();

                // 写入配置文件
                if (!Directory.Exists(configFileDir))
                    Directory.CreateDirectory(configFileDir);

                string err = ConfigFile.ScreenWriteFile(DingDingX, DingDingY, DingDingWidth, DingDingHeight,
                    PictureX, PictureY, PictureWidth, PictureHeight, screenIniPath);
                if (err != null)
                    throw new Exception("写入自定义截图数据文件错误\n原因：" + err);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "自定义截图", MessageBoxButtons.OK, MessageBoxIcon.Error);
                label1.Text = "截图失败！";
                label2.Text = "";
            }
        }


        // 清除按钮
        private void button2_Click(object sender, EventArgs e)
        {
            pictureBox1.Image = null;
            label1.Text = "请将透明区移到钉钉左上角\nxx群正在直播区域并截图";
            label2.Text = "";
            this.Height = 130;

            if (File.Exists(screenIniPath))
                File.Delete(screenIniPath);
        }

    }
}