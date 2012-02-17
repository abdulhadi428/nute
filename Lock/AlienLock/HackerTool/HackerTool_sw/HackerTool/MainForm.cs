using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;

namespace HackerTool {
    public enum LockState_t { Disconnected, EnteringServiceCode, WaitingCommand, AskingNewCodeA, AskingNewCodeB, AskingNewServiceCode };

    public partial class MainForm : Form {
        // ============================= Variables =================================
        public HTool_t HTool;
        public Lock_t Lock;
        private List<string> LockMenu;
        // Iteration
        int CurrentX, CurrentY;
        Random RndValue = new Random();
        int[,] InstantTable, AccumTable;
        int IterationCount = 0;

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
            // Iteration components
            // Setup Grid
            dgvTable.RowCount = 10;
            for (int i = 0; i <= 9; i++) dgvTable.Rows[i].Cells[0].Value = i.ToString();
            // Setup variables
            InstantTable = new int[10, 10];
            AccumTable = new int[10, 10];
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

        private void tbStartValue_TextChanged(object sender, EventArgs e) {
            UInt32 dummy;
            if (!UInt32.TryParse(tbStartValue.Text, out dummy)) tbStartValue.BackColor = Color.Red;
            else if (tbStartValue.Text.Length != 6) tbStartValue.BackColor = Color.Yellow;
            else tbStartValue.BackColor = Color.FromKnownColor(KnownColor.Window);
        }

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
                    //timerU.Enabled = true;
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

        private void btnStart_Click(object sender, EventArgs e) {
            if (BruteForcer.IsBusy) return;      // do not start second time
            //if (Lock.State != LockState_t.EnteringServiceCode) return;
            UInt32 dummy;
            if (!UInt32.TryParse(tbStartValue.Text, out dummy)) return;
            // Display start info
            Console.AppendText("Password search started" + Environment.NewLine);
            BruteForcer.RunWorkerAsync();
        }
        private void btnStop_Click(object sender, EventArgs e) {
            BruteForcer.CancelAsync();
        }

        private void btnIterate_Click(object sender, EventArgs e) {
            if (timerIteration.Enabled) return;     // do not start second time
            if (Lock.State == LockState_t.Disconnected) {
                Console.AppendText("Lock is not connected" + Environment.NewLine);
                return;
            }
            // Get current pair
            string S;
            if (rbtnP1.Checked) S = Lock.ServiceCode.Substring(0, 2);
            else if (rbtnP2.Checked) S = Lock.ServiceCode.Substring(2, 2);
            else S = Lock.ServiceCode.Substring(4, 2);
            int.TryParse(S.Substring(0, 1), out CurrentY);
            int.TryParse(S.Substring(1, 1), out CurrentX);
            // Start iteration
            Console.AppendText("Iteration started" + Environment.NewLine);
            timerIteration.Enabled = true;
        }
        private void btnStopIteration_Click(object sender, EventArgs e) {
            timerIteration.Enabled = false;
            Console.AppendText("Iteration stopped." + Environment.NewLine);
        }
        private void btnReset_Click(object sender, EventArgs e) {
            IterationCount = 0;
            for (int x = 0; x <= 9; x++) {
                for (int y = 0; y <= 9; y++) {
                    AccumTable[x, y] = 0;
                    dgvTable.Rows[y].Cells[x + 1].Style.BackColor = Color.White;
                }
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
            LockMenu.Add("Change code A");
            LockMenu.Add("Change code B");
            LockMenu.Add("Change service code");
        }
        private void PrintMenu() {
            Console.AppendText("> Enter command:" + Environment.NewLine);
            foreach (string SItem in LockMenu) {
                Console.AppendText("> * " + SItem + Environment.NewLine);
            }
        }

        private bool CheckEnteredCode(string AInCode, out string AOutCode) {
            Lock.Nop(); // blink led
            // Check if none
            if ((AInCode.Length == 0) || AInCode.Equals("None", StringComparison.OrdinalIgnoreCase)) {
                Console.AppendText("> Empty code entered (not recommended)." + Environment.NewLine);
                AOutCode = "None";
                return true;
            }
            else {  // Check if ok
                UInt32 dummy;
                AOutCode = AInCode;
                if ((AInCode.Length <= 4) && UInt32.TryParse(AInCode, out dummy)) {
                    return true;
                }
                else {
                    Console.AppendText("> Bad code." + Environment.NewLine + "> Enter new code:" + Environment.NewLine);
                    return false;
                }
            }
        } // CheckEnteredCode

        private void ParseCmd(string SCmd) {
            // Tool is just connected. Need to connect lock.
            if ((Lock.State == LockState_t.Disconnected) && SCmd.Equals("connect", StringComparison.OrdinalIgnoreCase)) {
                if (Lock.GetState()) {
                    Console.AppendText("Lock connected" + Environment.NewLine + "> Enter service code:" + Environment.NewLine);
                    Lock.State = LockState_t.EnteringServiceCode;
                }
                else Console.AppendText("No reply" + Environment.NewLine);
            } // if connect

            // Entering service code
            else if (Lock.State == LockState_t.EnteringServiceCode) {
                Lock.Nop(); // blink led
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
                        PrintMenu();
                        break;
                    case 1: // Close door
                        if (!Lock.Close()) Console.AppendText("> Command failed." + Environment.NewLine);
                        else               Console.AppendText("> Door closed." + Environment.NewLine);
                        PrintMenu();
                        break;
                    case 2: // Display code
                        Lock.Nop(); // blink led
                        Console.AppendText("> Code A: " + Lock.CodeA + Environment.NewLine + "> Code B: " + Lock.CodeB + Environment.NewLine);
                        PrintMenu();
                        break;
                    case 3: // Change code A
                        Lock.Nop(); // blink led
                        Console.AppendText("> Enter new code A:" + Environment.NewLine);
                        Lock.State = LockState_t.AskingNewCodeA;
                        break;
                    case 4: // Change code B
                        Lock.Nop(); // blink led
                        Console.AppendText("> Enter new code B:" + Environment.NewLine);
                        Lock.State = LockState_t.AskingNewCodeB;
                        break;
                    case 5: // Change service code
                        Lock.Nop(); // blink led
                        Console.AppendText("> Enter new service code:" + Environment.NewLine);
                        Lock.State = LockState_t.AskingNewServiceCode;
                        break;
                    default:
                        Lock.Nop(); // blink led
                        Console.AppendText("> Command not recognized." + Environment.NewLine);
                        PrintMenu();
                        break;
                } // switch 
            } // if WaitingCommand
            
            // Entering new code 
            else if (Lock.State == LockState_t.AskingNewCodeA) {
                if (!CheckEnteredCode(SCmd, out SCmd)) return;  // Bad code entered
                else {                                          // SCmd contains correct code
                    if (Lock.ChangeCodeA(SCmd)) Console.AppendText("> Code A changed." + Environment.NewLine);
                    else Console.AppendText("> Command failed." + Environment.NewLine);
                    PrintMenu();
                    Lock.State = LockState_t.WaitingCommand;
                }
            } // AskingNewCodeA
            else if (Lock.State == LockState_t.AskingNewCodeB) {
                if (!CheckEnteredCode(SCmd, out SCmd)) return;  // Bad code entered
                else {                                          // SCmd contains correct code
                    if (Lock.ChangeCodeB(SCmd)) Console.AppendText("> Code B changed." + Environment.NewLine);
                    else Console.AppendText("> Command failed." + Environment.NewLine);
                    PrintMenu();
                    Lock.State = LockState_t.WaitingCommand;
                }
            } // AskingNewCodeB

            // Entering new service code 
            else if (Lock.State == LockState_t.AskingNewServiceCode) {
                int dummy;
                if ((SCmd.Length == 6) && Int32.TryParse(SCmd, out dummy)) {    // Check if code ok
                    if (Lock.ChangeServiceCode(SCmd)) Console.AppendText("> Service code changed." + Environment.NewLine);
                    else Console.AppendText("> Command failed." + Environment.NewLine);
                    PrintMenu();
                    Lock.State = LockState_t.WaitingCommand;
                }
                else {
                    Lock.Nop(); // blink led
                    Console.AppendText("> Bad code." + Environment.NewLine + "> Enter new service code:" + Environment.NewLine);
                }
            } // AskingNewServiceCode
        } // ParseCmd 

        private void Console_KeyUp(object sender, KeyEventArgs e) {
            if (e.KeyValue == 13) {    // Enter was pressed
                // Get entered string
                string S = Console.Lines[Console.Lines.Count()-2];
                ParseCmd(S);                
            }
        }
        #endregion

        #region ============= Brute force ==============
        int MaxRang = 5;
        string ICode;
        
        string SumCodes(byte[] ACodeArr, byte[] AShiftArr) {
            string S = "";
            for (int i = 0; i < 6; i++) S += ((ACodeArr[i] + AShiftArr[i]) % 10).ToString();
            return S;
        }

        int ShiftRang(byte[] AShiftArr) {


        byte[] GetNextShift(byte[] AShiftArr) {
            byte[] NewShiftArr;
            int IRang = ShiftRang(AShiftArr);
            if (IRang == 0) IRang = 1;
            NewShiftArr = GetNextShiftSimple(AShiftArr, IRang);
            int ReturnedRang = ShiftRang(NewShiftArr);
            if (ReturnedRang < IRang) {
                if (ReturnedRang == 0) {
                    if (IRang == MaxRang) return null;
                }
            }

            return NewShiftArr;
        }


        private void BruteForcer_DoWork(object sender, DoWorkEventArgs e) {
            Lock.ServiceCode = "123456";    // DEBUG
            string StartCodeStr = tbStartValue.Text;
            byte[] StartCodeArr = (from x in StartCodeStr select Byte.Parse(x.ToString())).ToArray();
            byte[] ShiftArr = new byte[6];
        
            while (true) {
                // Get next code
                ICode = SumCodes(StartCodeArr, ShiftArr);

                // Check if code already equals
                if (ICode.Equals(Lock.ServiceCode)) return;
                if (BruteForcer.CancellationPending) {
                    e.Cancel = true;
                    return;
                }
                // Blink LED
//                if (Lock.Nop()) {
                    BruteForcer.ReportProgress(0);
                    Thread.Sleep(50);
  /*              }
                else {  // error occured
                    throw new Exception("Lock connection failure");
                }
                */
                ShiftArr = GetNextShift(ShiftArr, MaxRang);
            } // while 1
        }



        private void BruteForcer_ProgressChanged(object sender, ProgressChangedEventArgs e) {
            // Display code
            tbStartValue.Text = ICode;
            Console.AppendText(ICode + Environment.NewLine);
        }

        private void BruteForcer_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e) {
            if (e.Error != null) Console.AppendText("Error: " + e.Error.Message + Environment.NewLine);
            else if (e.Cancelled) Console.AppendText("Pass search eagerly cancelled." + Environment.NewLine);
            else {
                // Check if password is correct
                if (ICode.Equals(Lock.ServiceCode)) {
                    Console.AppendText("Password found: " + ICode + Environment.NewLine);
                    Console.AppendText("> Access is allowed." + Environment.NewLine);
                    PrintMenu();
                    Lock.State = LockState_t.WaitingCommand;
                }
                else Console.AppendText("> Incorrect code, access denied." + Environment.NewLine + "> Enter service code:" + Environment.NewLine);
            }
        }

        #endregion

        #region ==== Iteration ====
        double dif(int a1, int a2) {
            int NoiseMagn = trackBar1.Value;
            if (NoiseMagn > 0) {
                a1 += RndValue.Next(-NoiseMagn, NoiseMagn);
                if (a1 > 9) a1 -= 10;
                if (a1 < 0) a1 += 10;
            }
            return Math.Min((a1 - a2 + 10) % 10, (a2 - a1 + 10) % 10);
        }

        double p(double dist) {
            //int NoiseMagn = trackBarNoise.Value;
            //            return (1-dist)*.1+NoiseMagn*.01;
            //return (1 - dist) / (1 + NoiseMagn);
            return (1 - dist);
        }        // Distance function: returns 0...255, 255 means equality
        int Distance(int x1, int x2, int y1, int y2) {
            double difx = dif(x1, x2);
            double dify = dif(y1, y2);
            double Distn = Math.Sqrt(difx * difx + dify * dify);
            double c2 = RndValue.NextDouble() < p(Distn / (5 * Math.Sqrt(2))) ? 1 : 0;    // Norm to 1
            double c3 = c2 * 255;
            return (int)Math.Round(c2);
        }

        // Fill instant table
        void FillInstantTable() {
            for (int y = 0; y <= 9; y++) {
                for (int x = 0; x <= 9; x++) {
                    InstantTable[x, y] = Distance(x, CurrentX, y, CurrentY);
                }
            }
        }

        private void timerIteration_Tick(object sender, EventArgs e) {
            IterationCount++;
            lblIterationCounter.Text = "Iteration: " + IterationCount.ToString();
            FillInstantTable();
            // Accumulate values
            double MaxValue = 0;
            for (int y = 0; y <= 9; y++) {
                for (int x = 0; x <= 9; x++) {
                    AccumTable[x, y] += InstantTable[x, y];
                    if (AccumTable[x, y] > MaxValue) MaxValue = AccumTable[x, y];
                }
            }
            // Fill grid with normalized colors
            double NormValue;
            for (int y = 0; y <= 9; y++) {
                for (int x = 0; x <= 9; x++) {
                    NormValue = Math.Round(255 * (AccumTable[x, y] / MaxValue));
                    dgvTable.Rows[y].Cells[x + 1].Style.BackColor = Color.FromArgb(0, (int)NormValue, 0);
                }
            }
        }

        #endregion
    }


    #region ========================= Classes =========================
    public class Lock_t {
        public string ServiceCode = "Empty", CodeA = "Empty", CodeB = "Empty";  // will never equal
        public int Battery;
        public LockState_t State;
        public HTool_t Tool;

        #region ==== Lock commands ====
        public bool GetState() {
            if (!Tool.SendCmd("S")) return false;
            if (!Tool.WaitAnswer()) return false;
            // Check what received
            if (!Tool.RXString.StartsWith("S:")) return false;
            // Get service code, lock code and battery voltage from state reply
            string[] sl = Tool.RXString.Substring(2).Split(',');    // Split reply to chunks
            if (sl.Count() != 4) return false;                      // Must be 4 chunks
            ServiceCode = sl[0];
            CodeA = sl[1];
            CodeB = sl[2];
            if (!Int32.TryParse(sl[3], out Battery)) return false;
            return true;
        }

        public bool Nop() {
            if (!Tool.SendCmd("N")) return false;
            if (!Tool.WaitAnswer()) return false;
            // Check what received
            return Tool.RXString.StartsWith("N");
        }
        public bool Open() {
            if (!Tool.SendCmd("O")) return false;
            if (!Tool.WaitAnswer()) return false;
            // Check what received
            return Tool.RXString.StartsWith("O");
        }
        public bool Close() {
            if (!Tool.SendCmd("C")) return false;
            if (!Tool.WaitAnswer()) return false;
            // Check what received
            return Tool.RXString.StartsWith("C");
        }
        
        public bool ChangeCodeA(string ANewCode) {
            if (!Tool.SendCmd("A:" + ANewCode)) return false;
            if (!Tool.WaitAnswer()) return false;
            // Check what received
            if (Tool.RXString.StartsWith("A")) {
                CodeA = ANewCode;
                return true;
            }
            else return false;
        }
        public bool ChangeCodeB(string ANewCode) {
            if (!Tool.SendCmd("B:" + ANewCode)) return false;
            if (!Tool.WaitAnswer()) return false;
            // Check what received
            if (Tool.RXString.StartsWith("B")) {
                CodeB = ANewCode;
                return true;
            }
            else return false;
        }
        public bool ChangeServiceCode(string ANewCode) {
            if (!Tool.SendCmd("V:" + ANewCode)) return false;
            if (!Tool.WaitAnswer()) return false;
            // Check what received
            if (Tool.RXString.StartsWith("V")) {
                ServiceCode = ANewCode;
                return true;
            }
            else return false;
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
        public bool SendCmd(string ACmd) {
            RXString = "";
            ACmd += ';';
            byte[] buf = System.Text.Encoding.ASCII.GetBytes(ACmd);
            try {
                ComPort.Write(buf, 0, ACmd.Length);
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
                    RXString = RXString.TrimEnd(';');
                    return true;
                }
            }
            return false;
        }

        public bool Ping() {
            if (!SendCmd("P")) return false;
            if (!WaitAnswer()) return false;
            // Check what received
            IConsole.AppendText(RXString);
            IConsole.AppendText(Environment.NewLine);
            return RXString.StartsWith("HackerTool");
        }

        public bool GetU() {
            if (!SendCmd("U")) return false;
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
