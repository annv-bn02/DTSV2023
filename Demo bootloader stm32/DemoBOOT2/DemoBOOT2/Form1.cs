using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;
using System.IO;
using System.Text.RegularExpressions;

namespace DemoBOOT2
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (cbxComlist.Text == "")
            {
                MessageBox.Show("Vui lòng chọn cổng com", "Thông báo", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            if (serialPort.IsOpen)
            {
                serialPort.Close();
                button1.Text = "Kết nối";
                cbxComlist.Enabled = true;
                button2.Enabled = true;

            }
            else
            {
                serialPort.PortName = cbxComlist.Text;
                try
                {
                    serialPort.Open();
                    button1.Text = "Ngắt";
                    cbxComlist.Enabled = false;
                    button2.Enabled = false;

                }
                catch
                {
                    MessageBox.Show("Không thể mở cổng " + serialPort.PortName, "Lỗi", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            cbxComlist.Items.Clear();
            Com_khoitao();

        }
        public void Com_khoitao()
        {
            string[] ComList = SerialPort.GetPortNames();
            int[] ComNumberList = new int[ComList.Length];
            for (int i = 0; i < ComList.Length; i++)
            {
                ComNumberList[i] = int.Parse(ComList[i].Substring(3));
            }
            Array.Sort(ComNumberList);
            foreach (int ComNumber in ComNumberList)
            {
                cbxComlist.Items.Add("COM" + ComNumber.ToString());
            }
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            Com_khoitao();          
        }
        int program_size = 0;
        const int max_size = 8000;
        byte[] data_send = new byte[max_size];
        String hex_data = "";
        private void button3_Click(object sender, EventArgs e)
        {
            OpenFileDialog diaglog = new OpenFileDialog();
            diaglog.Filter = "HEX files|*.hex";
            if (diaglog.ShowDialog() == DialogResult.OK)
            {             
                for (int i = 0; i < max_size; i++)
                    data_send[i] = 0xFF;  //clear bộ đệm gửi
                program_size = 0;

                hex_data = File.ReadAllText(diaglog.FileName);
                textBox1.Text = hex_data;

                int _2c_line1 = hex_data.IndexOf(":");
                int _2c_line_end = hex_data.LastIndexOf(":");
                hex_data = hex_data.Substring(_2c_line1+1, _2c_line_end-1); // bỏ dòng 1 và dòng cuối

                _2c_line1 = hex_data.IndexOf(":");
                _2c_line_end = hex_data.LastIndexOf(":");
                hex_data = hex_data.Substring(_2c_line1, _2c_line_end - _2c_line1 - 1 ); // bỏ dòng 1 và dòng cuối
                //tiến hành lọc lấy phần mã hex                             
                while (true)
                {
                    int count = 0;
                    count = hex_data.IndexOf(":"); //lấy vị tri bắt đầu
                    if (count != -1) //nếu còn dữ liệu
                    {
                        byte[] data_num = StringToByteArrayFastest(hex_data.Substring(count + 1, 2)); //kiểm tra số lượng byte trong hàng   
                        int byte_num = (int)data_num[0];                       
                        byte[] data_line = new byte[byte_num]; //số lượng byte data trong line đó                     
                        data_line = StringToByteArrayFastest(hex_data.Substring(count + 9, byte_num * 2));
                        //đưa data vào mảng
                        for(int i=0;i< byte_num;i++)
                            data_send[program_size++] = data_line[i];
                        //lấy xong data thì cắt bỏ luôn
                        hex_data =hex_data.Substring(count+1);
                    }
                    else
                    {
                        label1.Text = "0/" + program_size.ToString() + " byte";
                        return;
                    }
                }
            }
        }
        public static byte[] StringToByteArrayFastest(string hex)
        {
            if (hex.Length % 2 == 1)
                throw new Exception("Err");

            byte[] arr = new byte[hex.Length >> 1];

            for (int i = 0; i < hex.Length >> 1; ++i)
            {
                arr[i] = (byte)((GetHexVal(hex[i << 1]) << 4) + (GetHexVal(hex[(i << 1) + 1])));
            }

            return arr;
        }

        public static int GetHexVal(char hex)
        {
            int val = (int)hex;
            //For uppercase A-F letters:
            //return val - (val < 58 ? 48 : 55);
            //For lowercase a-f letters:
            //return val - (val < 58 ? 48 : 87);
            //Or the two combined, but a bit slower:
            return val - (val < 58 ? 48 : (val < 97 ? 55 : 87));
        }
        int flag_check=0;
        private async void button4_Click(object sender, EventArgs e)
        {
            // nạp xuống từng byte data một
            int dem = 0;         
            //gửi thông điệp upload code
            serialPort.Write("Upload=" + program_size.ToString() + "\n");
            flag_check = 0;
            await Task.Delay(50); //chờ phản hồi từ mạch
            if (flag_check == 1)
            {
                while (true)
                {                   
                    if(program_size/1024 != dem)serialPort.Write(data_send, dem * 1024, 1024); //tryền từng mảng 1024 byte 1 lần
                    else serialPort.Write(data_send, dem * 1024, program_size-(1024*dem)); //truyền nốt phần data cuối
                    flag_check = 0;
                    timer1.Enabled = true;
                    await Task.Delay(500); //chờ phản hồi      
                    if (flag_check == 0)
                    {
                        MessageBox.Show("Lỗi");
                        return;
                    }
                    if (flag_check == 2)
                    {
                        MessageBox.Show("Lỗi khi ghi");
                        return;
                    }
                    dem++;
                    String vl = (dem * 1024).ToString() + "/" + program_size.ToString() + " byte";
                    if (label1.InvokeRequired)
                        label1.Invoke(new Action(() => label1.Text = vl));
                    else
                    { label1.Text = vl; Application.DoEvents(); } //cập nhật quá trình nạp

                    if (dem*1024 > program_size)
                    {
                        label1.Text = "Nạp thành công !";
                        MessageBox.Show("Xong");
                        return;
                    }
                }
            }
            else
            {
                MessageBox.Show("Kiểm tra kết nối !");
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            timer1.Enabled = false;
        }

        private void serialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                string dataR = serialPort.ReadLine();
                if(dataR.IndexOf("OK MEN !") != -1)
                {
                    flag_check = 1;
                }
                else if (dataR.IndexOf("Read OK !") != -1)
                {
                    flag_check = 1;
                }
                else if (dataR.IndexOf("Write Fall !") != -1)
                {
                    flag_check = 2;
                }
            }
            catch
            {
                return;
            }
        }
    }
}
