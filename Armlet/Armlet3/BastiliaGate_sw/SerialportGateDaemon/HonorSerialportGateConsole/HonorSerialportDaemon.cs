using System;
using System.Collections;
using System.Collections.Generic;
using System.IO.Ports;
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
            inputMessageQueue = Queue.Synchronized(queue1);
            outputMessageQueue = Queue.Synchronized(queue2);

            InitiateLog();
            sendThread = new Thread(SendToServer);
            InstanceContext instanceContext = new InstanceContext(new WCFCallbackHandler(this));

            byte serverProvidedGateId;
            try
            {
                
                WCFClient.Client = new GateWCFServiceClient(instanceContext, "NetTcpBindingEndpoint",
                                                        RemoteAddress);
                serverProvidedGateId = WCFClient.Client.RegisterGate(Byte.Parse(Settings.Default.PreferedGateId));
            }
            catch (EndpointNotFoundException)
            {
                throw new Exception("Не возможно подсоединится к серверу: " + RemoteAddress);
            }
            
            
            inputMessageQueue.Enqueue(new ServerToGateCommand(ServerToGateCommands.SetGateNum, new byte[]{serverProvidedGateId}));

        }

        private static string RemoteAddress
        {
            get
            {
                string WcfSericeIp = Settings.Default.WCFIPAddress;
                return "net.tcp://" + WcfSericeIp + ":8765/GateWcfService";
            }
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
                                byte commandByte = outputBytes[0];
                                //byte[] payload = new byte[outputBytes.Length-1];
                                //outputBytes.CopyTo(payload, 1);
                                var payload = outputBytes.Skip(1).ToArray();

                                switch (commandByte)
                                {
                                    case (byte) GateToServerCommands.Ack:
                                        continue;
                                    case (byte) GateToServerCommands.GateNumberSet:
                                        continue;
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

                                LogClass.Write("Should send from gate to server this: " + outputCommandString);
                            }
                            if (Enum.IsDefined(typeof (ArmletToServerCommands), outputBytes[0]))
                            {
                                byte commandByte = outputBytes[0];
                                byte[] payload = new byte[outputBytes.Length - 1];
                                outputBytes.CopyTo(payload, 1);

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
                                                    new PlayerUpdate()
                                                        {
                                                            ArmletID = armlet_id,
                                                            NewRoom = outputBytes[3],
                                                            NewBlood = outputBytes[4]
                                                        }
                                                });
                                        }
                                        if (data_count > 2)
                                        {
                                            byte[] rxDataPaylod = new byte[data_count - 2];
                                            outputBytes.CopyTo(rxDataPaylod, 5);
                                            WCFClient.Client.ArmletSendsData(armlet_id, rxDataPaylod);
                                        }
                                        break;
                                }
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
