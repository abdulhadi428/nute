using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace HackerTool {
    public enum LockState_t { Disconnected, Connected, AskingCode, WaitingCommand, AskingNewCode };

    public partial class MainForm : Form {
        // ============================= Variables =================================
        public HTool_t HTool;
        public Lock_t Lock;
        private List<string> LockMenu;

        #region ============================= Init / deinit ============================
        public MainForm() {
            InitializeComponent();
            Console.Clear();
            HTool = new HTool_t();
            Lock = new Lock_t();
            Lock.Tool = HTool;
            HTool.ComPort = serialPort1;
            HTool.IConsole = Console;
            FillMenu();
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e) {
            try {
                HTool.ComPort.Close();
                HTool.ComPort = null;
                serialPort1.Dispose();
            }
            catch (System.Exception ex) {
                HTool.LastError = ex.Message;
            }
            HTool.IsConnected = false;
        }

        #endregion

        #region ========================= Buttons =========================
        private void btnRescan_Click(object sender, EventArgs e) {
            Console.AppendText("Scanning system for available ports..." + Environment.NewLine);
            timerU.Enabled = false;     // Disable U polling
            HTool.IsConnected = false;
            Lock.State = LockState_t.Disconnected;
            Console.ReadOnly = true;
            cbPorts.Items.Clear();
            serialPort1.Close();
            string[] SPorts = System.IO.Ports.SerialPort.GetPortNames();
            int NPorts = SPorts.Length;
            if (NPorts == 0) {
                Console.AppendText("Ports not found, mate. No ports - no business.");
                return;
            }
            else {
                cbPorts.Items.AddRange(SPorts);
                cbPorts.SelectedIndex = 0;
                for(int i=0; i<NPorts; i++) Console.AppendText(SPorts[i] + Environment.NewLine);
                Console.AppendText("Ports found: " + NPorts.ToString() + Environment.NewLine);
            }
        }

        private void btnConnect_Click(object sender, EventArgs e) {
            timerU.Enabled = false; // Disable U polling
            Lock.State = LockState_t.Disconnected;
            Console.ReadOnly = true;

            if ((cbPorts.Items.Count == 0) || (cbPorts.SelectedItem.ToString() == "")) {
                Console.AppendText("Port not selected, afraid to connect to nowhere" + Environment.NewLine);
                return;
            }
            // Try to connect
            Console.AppendText("Trying to connect to " + cbPorts.SelectedItem.ToString() + Environment.NewLine);
            serialPort1.Close();
            serialPort1.PortName = cbPorts.SelectedItem.ToString();
            try {
                serialPort1.Open();
                // Try to ping device
                if (HTool.Ping()) { // Device found
                    Console.AppendText("Tool is found and connected." + Environment.NewLine);
                    //pictureBoxStendState.Image = imageList1.Images[0];
                    HTool.IsConnected = true;
                    timerU.Enabled = true;
                    Console.ReadOnly = false;
                    return; // All right, nothing to do here
                }
                else {
                    HTool.IsConnected = false;
                    serialPort1.Close();
                    Console.AppendText("Silence answers our cries. Tool not found." + Environment.NewLine);
                }
            }   //try
            catch (System.Exception ex) {
                Console.AppendText(ex.Message + Environment.NewLine);
                serialPort1.Close();
            }
        }
        #endregion

        #region ============================== Serial RX handler =========================
        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e) {
            // Read RX buffer
            int N = serialPort1.BytesToRead;
            if (N > 128) N = 128;
            char[] buf = new char[128];
            serialPort1.Read(buf, 0, N);
            serialPort1.DiscardInBuffer();  // clear what remained 
            string S = new string(buf);
            char[] charsToTrim = {'\0'};
            HTool.RXString += S.TrimEnd(charsToTrim);
        }
        #endregion

        private void timerU_Tick(object sender, EventArgs e) {
            if (HTool.GetU()) {
                progressBar1.Value = HTool.U;
            }
            else {
                HTool.IsConnected = false;
                timerU.Enabled = false;
            }
            
        }

        #region ========================= Console =========================
        private void FillMenu() {
            LockMenu = new List<string>();
            LockMenu.Add("Open door");
            LockMenu.Add("Close door");
            LockMenu.Add("Display code");
            LockMenu.Add("Change code");
            LockMenu.Add("Change service code");
        }
        private void PrintMenu() {
            Console.AppendText("> Enter command:" + Environment.NewLine);
            foreach (string SItem in LockMenu) {
                Console.AppendText("> * " + SItem + Environment.NewLine);
            }
        }

        private void ParseCmd(string SCmd) {
            // Tool is just connected. Need to connect lock.
            if ((Lock.State == LockState_t.Disconnected) && SCmd.Equals("connect", StringComparison.OrdinalIgnoreCase)) {
                if (Lock.GetState()) {
                    Console.AppendText("Lock connected" + Environment.NewLine + "> Enter service code:" + Environment.NewLine);
                    Lock.State = LockState_t.AskingCode;
                }
                else Console.AppendText("No reply" + Environment.NewLine);
            } // if connect

            // Entering service code
            else if (Lock.State == LockState_t.AskingCode) {
                // Wait a little
                System.Threading.Thread.Sleep(999);
                if (SCmd.Equals(Lock.ServiceCode)) {    // Check if code equal
                    Console.AppendText("> Access is allowed." + Environment.NewLine);
                    PrintMenu();
                    Lock.State = LockState_t.WaitingCommand;
                }
                else Console.AppendText("> Incorrect code, access denied." + Environment.NewLine + "> Enter service code:" + Environment.NewLine);
            } // if code

            // Handling menu
            else if (Lock.State == LockState_t.WaitingCommand) {
                int indx = LockMenu.FindIndex(S => S.Equals(SCmd, StringComparison.OrdinalIgnoreCase)); // Get indx of entered command
                switch (indx) {
                    case 0: // Open door
                        if (!Lock.Open()) Console.AppendText("> Command failed." + Environment.NewLine);
                        else              Console.AppendText("> Door opened." + Environment.NewLine);
                        break;
                    case 1: // Close door
                        if (!Lock.Close()) Console.AppendText("> Command failed." + Environment.NewLine);
                        else               Console.AppendText("> Door closed." + Environment.NewLine);
                        break;
                    case 2: // Display code
                        Console.AppendText("> Code: " + Lock.Code + Environment.NewLine);
                        break;
                    case 3: // Change code
                        if (!Lock.ChangeCode()) Console.AppendText("> Command failed." + Environment.NewLine);
                        else {
                            Console.AppendText("> Enter new code:" + Environment.NewLine);
                            Lock.State = LockState_t.AskingNewCode;
                        }
                        break;
                    case 4: // Change service code
                        if (!Lock.ChangeServiceCode()) Console.AppendText("> Command failed." + Environment.NewLine);
                        else                           Console.AppendText("> Service code changed." + Environment.NewLine);
                        break;
                    default:
                        Console.AppendText("> Command not recognized." + Environment.NewLine);
                        break;
                } // switch 
                PrintMenu();
            } // if WaitingCommand
            
            // Entering new code 
            else if (Lock.State == LockState_t.AskingNewCode) {
                // Check if code ok
                int dummy;
                if ((SCmd.Length > 4) || !Int32.TryParse(SCmd, out dummy)) {
                    Console.AppendText("> Bad code." + Environment.NewLine + "> Enter new code:" + Environment.NewLine);
                }
                else {

            }

        } // ParseCmd Console.AppendText("> Code changed." + Environment.NewLine);

        private void Console_KeyUp(object sender, KeyEventArgs e) {
            if (e.KeyValue == 13) {    // Enter was pressed
                // Get entered string
                string S = Console.Lines[Console.Lines.Count()-2];
                ParseCmd(S);                
            }

        }
        #endregion
    }


    #region ========================= Classes =========================
    public class Lock_t {
        public string ServiceCode, Code;
        public int Battery;
        public LockState_t State;
        public HTool_t Tool;

        #region ==== Lock commands ====
        public bool GetState() {
            if (!Tool.SendCmd('S')) return false;
            if (!Tool.WaitAnswer()) return false;
            // Check what received
            if (!Tool.RXString.StartsWith("S:")) return false;
            // Get service code, lock code and battery voltage from state reply
            string[] sl = Tool.RXString.Substring(2).Split(',');    // Split reply to chunks
            if (sl.Count() != 3) return false;                      // Must be 3 chunks
            ServiceCode = sl[0];
            Code = sl[1];
            Int32.TryParse(sl[2], out Battery);
            return true;
        }

        public bool Open() {
            return true;
        }
        public bool Close() {
            return true;
        }
        public bool ChangeCode() {
            return true;
        }
        public bool ChangeServiceCode() {
            return true;
        }

        #endregion
    }

    public class HTool_t {
        public string RXString;
        public bool IsConnected;
        public int U;
        public System.IO.Ports.SerialPort ComPort;
        public System.Windows.Forms.TextBox IConsole;
        public string LastError;

        // Constructor
        public HTool_t() {
            IsConnected = false;
        }

        #region ======== Tool commands ========
        public bool SendCmd(char ACmd) {
            RXString = "";
            byte[] Data = new byte[4];
            Data[0] = (byte)ACmd;
            try {
                ComPort.Write(Data, 0, 1);
                return true;
            }
            catch (System.Exception ex) {
                ComPort.Close();
                IsConnected = false;
                LastError = ex.Message;
                return false;
            }
        }
        public bool WaitAnswer() {
            for (int i = 0; i < 10; i++) {
                System.Threading.Thread.Sleep(10);
                if (RXString.EndsWith(";")) {
                    char[] charsToTrim = { ';' };
                    RXString = RXString.TrimEnd(charsToTrim);
                    return true;
                }
            }
            return false;
        }

        public bool Ping() {
            if (!SendCmd('P')) return false;
            if (!WaitAnswer()) return false;
            // Check what received
            IConsole.AppendText(RXString);
            IConsole.AppendText(Environment.NewLine);
            return RXString.StartsWith("HackerTool");
        }

        public bool GetU() {
            if (!SendCmd('U')) return false;
            if (!WaitAnswer()) return false;
            // Check what received
            if (!RXString.StartsWith("U:")) return false;   // something wrong came
            string SU = RXString.Substring(2);
            //IConsole.AppendText(u); IConsole.AppendText(Environment.NewLine);
            if (!Int32.TryParse(SU, out U)) return false;   // failed to convert string to int
            else return true;
        }
    
        #endregion
        
    } // HTool_t
    #endregion
}
