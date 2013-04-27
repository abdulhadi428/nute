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
using HonorSerialportGateDaemon;
using Timer = System.Timers.Timer;


namespace HonorSerialportGateConsole
{
    class Program
    {
        private static Queue queue1 = new Queue();
        public static Queue inputMessageQueue;
        private static Queue queue2 = new Queue();
        public static Queue outputMessageQueue;

        private Thread sendThread = new Thread(SendToServer);
        private static bool _sending = true;

        private static void SendToServer()
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
                        if (Enum.IsDefined(typeof(GateToServerCommands), outputBytes[0]))
                        {
                            LogClass.Write("Should send from gate to server this: " + outputCommandString);
                        }
                        if (Enum.IsDefined(typeof(ArmletToServerCommands), outputBytes[0]))
                        {
                            LogClass.Write("Should send from armlet to server this: " + outputCommandString);
                        }

                        
                    }
                }

                Thread.Sleep(300);
            }
        }


        public static StronglyTypedSerialPortConnector TypedSerialPort;
        static int Main(string[] args)
        {

            InitiateLog();
            return (new Program().RunMailCycle());


        }


        private static Timer UpdateLogTimer;

        private static void InitiateLog()
        {
            LogClass.SetVerbosity(Settings.Default.LogIsVerbose);
            UpdateLogTimer = new Timer(1000) {AutoReset = true};
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

        
        int RunMailCycle()
        {
            inputMessageQueue = Queue.Synchronized(queue1);
            outputMessageQueue = Queue.Synchronized(queue2);
            
            TypedSerialPort = new StronglyTypedSerialPortConnector(
                inputMessageQueue, outputMessageQueue, new SerialPortSettings()
                                                            {
                                                                BaudRate = 115200,
                                                                DataBits = 8,
                                                                Parity = Parity.None,
                                                                Handshake = Handshake.None,
                                                                StopBits = StopBits.One
                                                            });
            TypedSerialPort.SearchForPortAndConnect();
            sendThread.Start();

            while (true)
            {
                Console.ReadLine();
                TypedSerialPort.SendCommandToGate(new ServerToGateCommand(ServerToGateCommands.Ping, new byte[0]));
                TypedSerialPort.SendCommandToGate(new ServerToGateCommand(ServerToGateCommands.GetPillStatus,
                                                                          new byte[0]));
                TypedSerialPort.SendCommandToGate(new ServerToGateCommand(ServerToGateCommands.ReadDataFromPill,
                                                                          new byte[] { 0, 5 }));
                TypedSerialPort.SendCommandToGate(new ServerToGateCommand(ServerToGateCommands.WriteDataToPill,
                                                                          new byte[] {0, 5, 1, 100}));
                TypedSerialPort.SendCommandToGate(new ServerToGateCommand(ServerToGateCommands.ReadDataFromPill,
                                                                          new byte[] {0, 5}));
                TypedSerialPort.SendCommandToArmlet(new ServerToArmletCommand(ServerToArmletCommands.SendDataToArmlet,
                                                                              new byte[] {1, 2, 3}));


            }

            return 0;
        }


        
    }
}
