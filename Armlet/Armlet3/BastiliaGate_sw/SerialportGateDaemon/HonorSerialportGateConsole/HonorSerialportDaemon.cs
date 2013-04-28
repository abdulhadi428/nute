using System;
using System.Collections;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Timers;
using HonorSerialportGateConsole.Interfaces;
using HonorSerialportGateConsole.Properties;
using HonorSerialportGateConsole.ServerWCFService;
using HonorSerialportGateDaemon;


namespace HonorSerialportGateConsole
{
    public class HonorSerialportDaemon
    {
        public  StronglyTypedSerialPortConnector TypedSerialPort;
        private Thread sendThread;

        private  Queue queue1 = new Queue();
        public  Queue inputMessageQueue;
        private  Queue queue2 = new Queue();
        public  Queue outputMessageQueue;

        
        private  bool _sending = true;

        private System.Timers.Timer UpdateLogTimer;

        public HonorSerialportDaemon()
        {
            InitiateLog();
            sendThread = new Thread(SendToServer);
           // WCFClient.Client = new GateWCFServiceClient();


        }

   #region InitLog
        private void InitiateLog()
        {
            LogClass.SetVerbosity(Settings.Default.LogIsVerbose);
            UpdateLogTimer = new System.Timers.Timer(1000) { AutoReset = true };
            UpdateLogTimer.Elapsed += new ElapsedEventHandler(UpdateLog_Elapsed);
            UpdateLogTimer.Start();
        }
        static void UpdateLog_Elapsed(object sender, ElapsedEventArgs e)
        {

            lock (LogClass.Items)
            {
                if (LogClass.Items.Count != 0)
                {
                    Console.WriteLine("\n" + string.Join("\n", LogClass.Items.ToArray()));
                    LogClass.Items.Clear();
                }
            }
        }

#endregion

       
        public int RunMailCycle()
        {
            inputMessageQueue = Queue.Synchronized(queue1);
            outputMessageQueue = Queue.Synchronized(queue2);

            TypedSerialPort = new StronglyTypedSerialPortConnector(
                inputMessageQueue, outputMessageQueue,
                new SerialPortSettings()
                {
                    BaudRate = 115200,
                    DataBits = 8,
                    Parity = Parity.None,
                    Handshake = Handshake.None,
                    StopBits = StopBits.One
                });
            TypedSerialPort.SearchForPortAndConnect();

            sendThread.Start();
            return 0;
        }

        public  void SendToServer()
        {
            while (true)
            {
                while (_sending)
                {
                    lock (outputMessageQueue.SyncRoot)
                    {
                        while (outputMessageQueue.Count > 0)
                        {
                            var outputCommandString = (string) outputMessageQueue.Dequeue();
                            var outputBytes =
                                Command.HexStringToByteArray(Command.SanitiseStringFromComas(outputCommandString));
                            if (Enum.IsDefined(typeof (GateToServerCommands), outputBytes[0]))
                            {
                                LogClass.Write("Should send from gate to server this: " + outputCommandString);
                            }
                            if (Enum.IsDefined(typeof (ArmletToServerCommands), outputBytes[0]))
                            {
                                LogClass.Write("Should send from armlet to server this: " + outputCommandString);
                            }
                        }
                    }
                    Thread.Sleep(300); //Отправляем каждые 300 секунду.
                }
                Thread.Sleep(1000); //Ждем секунду и надеемся, что в следующий раз уже начнем слать (_sending станет true)
            }
        
        }


    }
}
