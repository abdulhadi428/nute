using System;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace ImageConverter
{
    
// ReSharper disable IdentifierTypo
// ReSharper disable InconsistentNaming
    public partial class frmImageConverter : Form
// ReSharper restore InconsistentNaming
// ReSharper restore IdentifierTypo
    {
        public frmImageConverter()
        {
            InitializeComponent();
        }

        void ProcessFolder(string folderPath)
        {
            var files = Directory.GetFiles(folderPath);
            int filesCount = files.Length;
            
            StringBuilder defines = new StringBuilder(filesCount*50);
            StringBuilder imageData = new StringBuilder(filesCount * 5000);
            StringBuilder arrayInit = new StringBuilder(filesCount * 5000);

            StringBuilder cppContent = new StringBuilder();
            StringBuilder hContent = new StringBuilder();

            progressBar1.Maximum = filesCount;
            progressBar1.Value = 0;

            int imageIndex = 0;
            foreach (var filePath in Directory.GetFiles(folderPath))
            {
                try
                {
                    ProcessFile(filePath, defines, imageData, arrayInit, imageIndex);
                    imageIndex++;
                    progressBar1.Value++;
                    Refresh();
                }
                catch (Exception)
                {
                    //eat it
                }
            }

            string initFuncName = "FillAppSpecificImages";
            if (chkSystemImages.Checked)
            {
                initFuncName = "FillStdImages";
            }

            hContent.AppendLine("#pragma once");
            hContent.AppendLine();
            if (chkSystemImages.Checked)
            {
                hContent.AppendFormat("#define SYS_IMAGES_COUNT {0}", imageIndex);
            }
            else
            {
                if (!string.IsNullOrEmpty(txtAppImagesName.Text))
                {
                    hContent.AppendFormat("#define {1}_COUNT {0}", imageIndex, txtAppImagesName.Text);
                }
            }
                
            hContent.AppendLine();
            hContent.AppendLine();
            hContent.AppendLine(defines.ToString());
            hContent.AppendLine();
            hContent.AppendLine();
            hContent.AppendFormat("void {0}(ImagesRepository* images);", initFuncName);

            cppContent.AppendLine("#include \"ArmletShell.h\"");
            cppContent.AppendLine("#pragma region static img defs");
            cppContent.Append(imageData);
            cppContent.AppendLine();
            cppContent.AppendLine("#pragma endregion"); 
            cppContent.AppendLine();
            if (chkSystemImages.Checked)
            {
                cppContent.AppendLine("BitmapImage _images[SYS_IMAGES_COUNT];");
            }
            else
            {
                if (!string.IsNullOrEmpty(txtAppImagesName.Text))
                {
                    string offset = string.Empty;
                    if (!string.IsNullOrEmpty(cbxOffsetName.Text))
                    {
                        offset = string.Format("{0}+", cbxOffsetName.Text);
                    }

                    string arraySize = string.Format("{0}{1}_COUNT", offset, txtAppImagesName.Text);

                    cppContent.AppendFormat("BitmapImage _images[{0}];", arraySize);
                }
                else
                {
                    cppContent.AppendFormat("BitmapImage _images[{0}];", imageIndex);
                }
                
                cppContent.AppendLine();
            }
            
            cppContent.AppendLine();
            cppContent.AppendLine();
            cppContent.AppendFormat("void {0}(ImagesRepository* images)", initFuncName);
            cppContent.AppendLine();
            cppContent.AppendLine("{");
            cppContent.AppendLine();
            cppContent.Append(arrayInit);
            cppContent.AppendLine("}");

            txtCPPContent.Text = cppContent.ToString();
            txtHContent.Text = hContent.ToString();
        }

        private void ProcessFile(string filePath, StringBuilder defines, StringBuilder imagedata, StringBuilder arrayInit, int imageIndex)
        {
            string imageName = Path.GetFileNameWithoutExtension(filePath);
            if (chkSystemImages.Checked)
            {
                defines.AppendFormat("#define {0} {1}{2}", imageName, imageIndex, Environment.NewLine);    
            }
            else
            {
                string offset = string.Empty;
                if (!string.IsNullOrEmpty(cbxOffsetName.Text))
                {
                    offset = string.Format(" + {0}", cbxOffsetName.Text);
                }

                defines.AppendFormat("#define {0} SYS_IMAGES_COUNT{3}+{1}{2}", imageName, imageIndex, Environment.NewLine, offset); 
            }
            

            try
            {
                Size sz = readImageData(filePath, imagedata, imageName);

                string offset = string.Empty;
                if (!string.IsNullOrEmpty(cbxOffsetName.Text))
                {
                    offset = string.Format("{0}+", cbxOffsetName.Text);
                }

                arrayInit.AppendFormat("    //{0}{1}", imageName, Environment.NewLine);
                arrayInit.AppendFormat("    _images[{3}{0}].ImageSize.Height = {1};{2}", imageIndex, sz.Height, Environment.NewLine, offset);
                arrayInit.AppendFormat("    _images[{3}{0}].ImageSize.Width  = {1};{2}", imageIndex, sz.Width, Environment.NewLine, offset);
                arrayInit.AppendFormat("    _images[{3}{0}].Bitmap = (Color*) {1}Bitmap;{2}", imageIndex, imageName, Environment.NewLine, offset);
                arrayInit.AppendFormat("    images->RegisterImage(&_images[{2}{0}]);{1}", imageIndex, Environment.NewLine, offset);
                arrayInit.AppendLine();
            }
            catch
            {
                //eat it
            }

        }


        private Size readImageData(string filePath, StringBuilder imagedata, string imageName)
        {

            Size sz = new Size();
            var bmp = new Bitmap(filePath);
            sz.Height = bmp.Height;
            sz.Width = bmp.Width;
            StringBuilder arrayData = new StringBuilder(sz.Height*sz.Width*15);
            Color transparentColor = cbxTrasparency.BackColor;

            for (int j = 0; j < sz.Height; j++)
            {
                arrayData.Append(Environment.NewLine);  
                for (int i = 0; i < sz.Width; i++)
                {
                    Color currentColor = bmp.GetPixel(i, j);
                    if (i == 0 && j == 0 && cbxTrasparency.SelectedIndex==1)
                    {
                        //first pixel sets transparency
                        transparentColor = currentColor;
                    }

                    bool transparent = false;
                    if (cbxTrasparency.SelectedIndex != 3)
                    {
                         transparent = currentColor.ToArgb() ==transparentColor.ToArgb();    
                    }
                    
                    int color = ConvertTo16BitColor(currentColor,transparent);

                    arrayData.AppendFormat("0x{0:X4}, ", color);
                }

            }
            //last comma
            arrayData.Remove(arrayData.Length - 2, 1);
            imagedata.AppendFormat("{2}const Color {0}Bitmap[]={{{1}{2}}};", imageName, arrayData, Environment.NewLine);
            bmp.Dispose();
            return sz;
        }

        private static int ConvertTo16BitColor(Color currentColor, bool transparent)
        {
            int chA = (currentColor.A >> 4) & 0x0F;
            int chR = (currentColor.R >> 4) & 0x0F;
            int chG = (currentColor.G >> 4) & 0x0F;
            int chB = (currentColor.B >> 4) & 0x0F;

            if (transparent)
            {
                chA = 0x0; //(0xFF >> 4) & 0x0F;
            }

            int color16 = (chA << 12) |
                        (chR << 8) |
                        (chG << 4) |
                        (chB << 0);
            return color16;
        }

        private void txt32bitColor_TextChanged(object sender, EventArgs e)
        {
            Color clr;
            if (TryGetColorFrom32BitArgbString(txt32bitColor.Text, out clr))
            {
                pnlColor.BackColor = clr;
                txt16bitColor.Text = string.Format("0x{0:X4}", ConvertTo16BitColor(clr, false));
            }
        }

        private bool TryGetColorFrom32BitArgbString(string str, out Color clr)
        {
            int color32Bit;
            bool result = false;
            clr = Color.White;
            if (int.TryParse(str, out color32Bit))
            {
                color32Bit = color32Bit | unchecked((int)0xFF000000);
                clr = Color.FromArgb(color32Bit);
                result = true;
            }
            else
            {
                try
                {
                    color32Bit = Convert.ToInt32(str, 16);
                    clr = Color.FromArgb(color32Bit);
                    result = true;
                }
                catch
                {
                    //parse Bublik's fromat "0xFF,230,230,230"
                    string[] splitted = str.Split(new char[] {','}, 4);
                    if (str.Contains(","))
                    {
                        if (splitted.Length == 4)
                        {
                            try
                            {
                                splitted[0] = splitted[0].Trim();
                                splitted[1] = splitted[1].Trim();
                                splitted[2] = splitted[2].Trim();
                                splitted[3] = splitted[3].Trim();

                                clr = Color.FromArgb(
                                                255,
                                                int.Parse(splitted[1]),
                                                int.Parse(splitted[2]),
                                                int.Parse(splitted[3])
                                               );

                                //remove transparency
                                result = true;
                            }
                            catch (Exception)
                            {

                                //eat it;
                            }
                        }
                    }
                }
            }
            return result;
        }

        private void cmdColorDialog_Click(object sender, EventArgs e)
        {
            int color32Bit;
            if (int.TryParse(txt32bitColor.Text, out color32Bit))
            {
                colorPicker.Color = Color.FromArgb(color32Bit);
            }

            if (colorPicker.ShowDialog() == DialogResult.OK)
            {
                txt32bitColor.Text = string.Format("0x{0:X4}",colorPicker.Color.ToArgb());
            }
        }

        private void cmdBrowseDir_Click(object sender, EventArgs e)
        {
            folderDialog.RootFolder = Environment.SpecialFolder.DesktopDirectory;
            if (string.IsNullOrEmpty(txtFolder.Text))
            {
                folderDialog.SelectedPath = Path.GetDirectoryName(Application.ExecutablePath);
            }
            else
            {
                folderDialog.SelectedPath = txtFolder.Text;
            }
            
            if (folderDialog.ShowDialog() == DialogResult.OK)
            {
                txtFolder.Text = folderDialog.SelectedPath;
            }
        }

        private void cmdStart_Click(object sender, EventArgs e)
        {
            if (!string.IsNullOrEmpty(txtFolder.Text))
            {
                ProcessFolder(txtFolder.Text);
            }
        }

        private void frmImageConverter_Load(object sender, EventArgs e)
        {
            if (Environment.GetCommandLineArgs().Length > 1 && Directory.Exists(Environment.GetCommandLineArgs()[1]))
            {
                txtFolder.Text = Environment.GetCommandLineArgs()[1];
            }
            else if (!Directory.Exists(txtFolder.Text))
            {
                txtFolder.Text = Application.ExecutablePath;
            }

            cbxTrasparency.SelectedIndex = 0;
            gbxCustomSettings.Enabled = !chkSystemImages.Checked;
        }

        private void cbxTrasparency_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (cbxTrasparency.SelectedIndex == 2)
            {
                dlgTransparentColorPicker.ShowDialog();
                cbxTrasparency.BackColor = dlgTransparentColorPicker.Color;
            }
            else
            {
                cbxTrasparency.BackColor = Color.White;
            }
        }

        private void chkSystemImages_CheckedChanged(object sender, EventArgs e)
        {
            gbxCustomSettings.Enabled = !chkSystemImages.Checked;
        }

    }
}
