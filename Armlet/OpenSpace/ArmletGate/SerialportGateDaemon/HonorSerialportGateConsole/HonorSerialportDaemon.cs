using System;
using System.Collections;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using System.Text;
using System.Threading;
using System.Timers;
using HonorSerialportGateConsole.Interfaces;
using HonorSerialportGateConsole.Properties;
using HonorSerialportGateConsole.ServerWCFService;
using HonorSerialportGateDaemon;


namespace HonorSerialportGateConsole
{
    public class HonorSerialportDaemon : IDisposable
    {
        public  StronglyTypedSerialPortConnector TypedSerialPort;
        private Thread sendThread;

        public  ConcurrentQueue<Command> inputMessageQueue = new ConcurrentQueue<Command>();

        public  ConcurrentQueue<string> outputMessageQueue = new ConcurrentQueue<string>();

        
        private  bool _sending = true;

        private System.Timers.Timer UpdateLogTimer;

        public HonorSerialportDaemon()
        {
            InitiateLog();
            sendThread = new Thread(SendToServer);
            var instanceContext = new InstanceContext(new WCFCallbackHandler(this));
            

            byte serverProvidedGateId;
            try
            {
                
                WCFClient.Client = new GateWCFServiceClient(instanceContext, "NetTcpBindingEndpoint");
                
                
                serverProvidedGateId = WCFClient.Client.RegisterGate(Byte.Parse(Settings.Default.PreferedGateId));
            }
            catch (EndpointNotFoundException)
            {
                throw new Exception("Не возможно подсоединится к серверу");
            }
            
            
            inputMessageQueue.Enqueue(new ServerToGateCommand(ServerToGateCommands.SetGateNum, new byte[]{serverProvidedGateId}));

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
            TypedSerialPort = new StronglyTypedSerialPortConnector(
                inputMessageQueue, outputMessageQueue);
            TypedSerialPort.SearchForPortAndConnect();

            sendThread.Start();
            Console.ReadLine();
            return 0;
        }

        private void SendToServer()
        {
            try
            {
                while (true)
                {
                    bool tryDequeue = true;


                    while (tryDequeue)
                    {
                        string outputCommandString;
                        tryDequeue = outputMessageQueue.TryDequeue(out outputCommandString);
                        if (tryDequeue)
                        {
                            SendMessage(outputCommandString);
                        }
                    }
                    Thread.Sleep(300); //Отправляем каждые 300 секунду.
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                Environment.Exit(1);
            }
        }

        private static bool SendMessage(string outputCommandString)
        {
            var outputBytes =
                Command.HexStringToByteArray(Command.SanitiseStringFromComas(outputCommandString));
            if (Enum.IsDefined(typeof (GateToServerCommands), outputBytes[0]))
            {
                byte commandByte = outputBytes[0];
                var payload = outputBytes.Skip(1).ToArray();

                switch (commandByte)
                {
                    case (byte) GateToServerCommands.Ack:
                        return true;
                    case (byte) GateToServerCommands.GateNumberSet:
                        return true;
                    case (byte) GateToServerCommands.PillConnectedStatus:
                        WCFClient.Client.PillConnectionStatusAsync(payload);
                        break;
                    case (byte) GateToServerCommands.PillReadResult:
                        WCFClient.Client.PillDataReadAsync(payload);
                        break;
                    case (byte) GateToServerCommands.PillWriteResult:
                        WCFClient.Client.PillWriteCompletedAsync(payload);
                        break;
                    case (byte) GateToServerCommands.PinSignalSet:
                        WCFClient.Client.PinSetAsync(payload);
                        break;
                }

                //LogClass.Write("Should send from gate to server this: " + outputCommandString);
            }
            if (Enum.IsDefined(typeof (ArmletToServerCommands), outputBytes[0]))
            {
                byte commandByte = outputBytes[0];
                var payload = outputBytes.Skip(1).ToArray();

                switch (commandByte)
                {
                    case (byte) ArmletToServerCommands.TXCompleted:
                        WCFClient.Client.TXCompletedAsync(payload);
                        break;
                    case (byte) ArmletToServerCommands.RXCompleted:
                        byte armlet_id = outputBytes[1];
                        byte data_count = outputBytes[2];
                        if (data_count >= 2)
                        {
                            WCFClient.Client.ArmlteStatusUpdateAsync(new PlayerUpdate[]
                                {
                                    new PlayerUpdate
                                        {
                                            ArmletID = armlet_id,
                                            NewRoom = outputBytes[3],
                                            NewBlood = outputBytes[4]
                                        }
                                });
                        }
                        if (data_count > 2)
                        {
                            byte[] rxDataPaylod = outputBytes.Skip(2).ToArray();
                            WCFClient.Client.ArmletSendsDataAsync(armlet_id, rxDataPaylod);
                        }
                        break;
                }
//                LogClass.Write("Should send from armlet to server this: " + outputCommandString);
            }
            return false;
        }


        public void Dispose()
        {
            if (TypedSerialPort != null)
            {
                TypedSerialPort.Dispose();
            }
        }
    }
}
