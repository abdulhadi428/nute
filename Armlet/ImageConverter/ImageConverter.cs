using System;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace ImageConverter
{
    public partial class ImageConverter : Form
    {
        public ImageConverter()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            openFileDialog1.InitialDirectory = Path.GetDirectoryName(Application.ExecutablePath);
            if (openFileDialog1.ShowDialog() != DialogResult.OK) return;
            try
            {
                var bmp = new Bitmap(openFileDialog1.FileName);
                pictureBox.Image = bmp;
                txtContent.Text = string.Empty;
                txtContent.Text += String.Format("const uword_t {0}Height = {1};{3}const uword_t {0}Width = {2};",
                                                 Path.GetFileNameWithoutExtension(openFileDialog1.FileName),
                                                 pictureBox.Image.Height, pictureBox.Image.Width, Environment.NewLine
                    );

                var sb = new StringBuilder(pictureBox.Image.Width*pictureBox.Image.Height*4*5);
                for (int i = 0; i < pictureBox.Image.Height; i++)
                {
                    sb.Append(Environment.NewLine);
                    for (int j = 0; j < pictureBox.Image.Height; j++)
                    {
                        sb.AppendFormat("{0}, {1}, {2}, {3},", bmp.GetPixel(i, j).A, bmp.GetPixel(i, j).R,
                                        bmp.GetPixel(i, j).G, bmp.GetPixel(i, j).B);
                    }
                        
                }
                //last comma
                sb.Remove(sb.Length - 2, 1);
                txtContent.Text += String.Format("sbyte_t {0}Bitmap[]={{{1}}};",
                                                 Path.GetFileNameWithoutExtension(openFileDialog1.FileName),
                                                 sb);

            }
            catch (Exception ex)
            {
                    
                MessageBox.Show(@"Can't load file: " + ex.Message);
            }
        }
    }
}
