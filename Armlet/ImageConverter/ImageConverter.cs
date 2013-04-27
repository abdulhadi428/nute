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
                txtContent.Text += String.Format(
                    "const uword_t {0}Height = {1};{3}const uword_t {0}Width = {2};{3}",
                        Path.GetFileNameWithoutExtension(openFileDialog1.FileName),
                        pictureBox.Image.Height, 
                        pictureBox.Image.Width, 
                        Environment.NewLine
                    );

                var sb = new StringBuilder(pictureBox.Image.Width*pictureBox.Image.Height*8);
                for (int j = 0; j < pictureBox.Image.Height; j++)
                {
                    sb.Append(Environment.NewLine);
                    for (int i = 0; i < pictureBox.Image.Width; i++)
                    {
                        int chA = (bmp.GetPixel(i, j).A >> 4) & 0x0F;
                        int chR = (bmp.GetPixel(i, j).R >> 4) & 0x0F;
                        int chG = (bmp.GetPixel(i, j).G >> 4) & 0x0F;
                        int chB = (bmp.GetPixel(i, j).B >> 4) & 0x0F;

                        int color = (chA << 12)|
                                    (chR <<  8)|
                                    (chG <<  4)|
                                    (chB <<  0);

                        sb.AppendFormat("0x{0:X4}, ",color);
                    }
                        
                }
                //last comma
                sb.Remove(sb.Length - 2, 1);
                txtContent.Text += String.Format("const uword_t {0}Bitmap[]={{{1}{2}}};",
                        Path.GetFileNameWithoutExtension(openFileDialog1.FileName),
                        sb,
                        Environment.NewLine);
            }
            catch (Exception ex)
            {
                    
                MessageBox.Show(@"Can't load file: " + ex.Message);
            }
        }
    }
}
