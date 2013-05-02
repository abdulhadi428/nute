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

        
        private  bool _sending = false;
        private InstanceContext instanceContext;
        private byte serverProvidedGateId;

        private System.Timers.Timer UpdateLogTimer;

        public HonorSerialportDaemon()
        {
            inputMessageQueue = Queue.Synchronized(queue1);
            outputMessageQueue = Queue.Synchronized(queue2);

            InitiateLog();
            sendThread = new Thread(SendToServer);
            instanceContext = new InstanceContext(new WCFCallbackHandler(this));

            serverProvidedGateId = Byte.Parse(Settings.Default.PreferedGateId);
            LogClass.Write("Trying to obtain connection with gate # " + Settings.Default.PreferedGateId);
            try
            {
                
                WCFClient.Client = new GateWCFServiceClient(instanceContext, "NetTcpBindingEndpoint",
                                                        RemoteAddress);
                serverProvidedGateId = WCFClient.Client.RegisterGate(Byte.Parse(Settings.Default.PreferedGateId));
                LogClass.Write("Connection to server successfully established. This is now Gate № " + serverProvidedGateId);
                Console.Title = "Gate # " + serverProvidedGateId + "Console";
                inputMessageQueue.Enqueue(new ServerToGateCommand(ServerToGateCommands.SetGateNum, new byte[] { serverProvidedGateId }));
                ((ICommunicationObject)WCFClient.Client).Faulted += HonorSerialportDaemon_Faulted;
                ((ICommunicationObject)WCFClient.Client).Closed += HonorSerialportDaemon_Closed;
                _sending = true;
            }
            catch (EndpointNotFoundException)
            {
                _sending = false;
                WCFClient.Client = null;
                LogClass.Write("Невозможно подсоединится к серверу: " + RemoteAddress);
                LogClass.Write("Я тут еще раз попробую подключиться, а вы там, пожалуйста, проверьте, что у сервер работает и у меня указан правильный адрес :)");
                LogClass.Write("");
                while (!RetryConnection())
                {
                    LogClass.Write("Timeout is 5 seconds...");
                    Thread.Sleep(5000);
                }
                _sending = true;
            }
        }


        private static string RemoteAddress
        {
            get
            {
                string WcfSericeIp = Settings.Default.WCFIPAddress;
                return "net.tcp://" + WcfSericeIp + ":8765/GateWcfService";
            }
        }


        void HonorSerialportDaemon_Closed(object sender, EventArgs e)
        {
            NetworkProblemHandler();
        }

        void HonorSerialportDaemon_Faulted(object sender, EventArgs e)
        {
            NetworkProblemHandler();
        }

        void NetworkProblemHandler()
        {
            LogClass.Write("Server connection error! Stopping sending events from COM port to server :(");
            _sending = false;
            WCFClient.Client = null;
            while (!RetryConnection())
            {
                LogClass.Write("Timeout is 5 seconds...");
                Thread.Sleep(5000);
            }
            _sending = true;
        }

        bool RetryConnection()
        {
            LogClass.Write("Now retring to restore connection as Gate # " + serverProvidedGateId);

            try
            {
                WCFClient.Client = new GateWCFServiceClient(instanceContext, "NetTcpBindingEndpoint",
                                                        RemoteAddress);
                serverProvidedGateId = WCFClient.Client.RegisterGate(serverProvidedGateId);
            }
            catch (EndpointNotFoundException)
            {
                LogClass.Write("Can't connect to server... Retrying indefinitly...");
                return false;
            }

            LogClass.Write("Connection to server successfully established. This is now Gate № " + serverProvidedGateId);
            Console.Title = "Gate # " + serverProvidedGateId + "Console";
            inputMessageQueue.Enqueue(new ServerToGateCommand(ServerToGateCommands.SetGateNum, new byte[] { serverProvidedGateId }));
            ((ICommunicationObject)WCFClient.Client).Faulted += HonorSerialportDaemon_Faulted;
            ((ICommunicationObject)WCFClient.Client).Closed += HonorSerialportDaemon_Closed;
            return true;
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
            //TypedSerialPort.SearchForPortAndConnect();

            sendThread.Start();
            return 0;
        }

        public  void SendToServer()
        {
            List<PlayerUpdate> updates = new List<PlayerUpdate>();
            while (true)
            {
                while (_sending)
                {
                    
                    lock (outputMessageQueue.SyncRoot)
                    {
                        while (outputMessageQueue.Count > 0)
                        {
                            try
                            {
                                var outputCommandString = (string) outputMessageQueue.Peek();
                                var outputBytes =
                                    Command.HexStringToByteArray(Command.SanitiseStringFromComas(outputCommandString));
                                if (Enum.IsDefined(typeof (GateToServerCommands), outputBytes[0]))
                                {
                                    byte commandByte = outputBytes[0];
                                    //byte[] payload = new byte[outputBytes.Length-1];
                                    //outputBytes.CopyTo(payload, 1);
                                    // var payload = outputBytes.Skip(1).ToArray();
                                    byte[] payload = new byte[outputBytes.Length - 1];
                                    Buffer.BlockCopy(outputBytes, 1, payload, 0, payload.Length);
                                    // outputBytes.CopyTo(payload, 1);

                                    switch (commandByte)
                                    {
                                        case (byte) GateToServerCommands.Ack:
                                            continue;
                                        case (byte) GateToServerCommands.GateNumberSet:
                                            continue;
                                        case (byte) GateToServerCommands.PillConnectedStatus:
                                            WCFClient.Client.PillConnectionStatus(payload);
                                            break;
                                        case (byte) GateToServerCommands.PillReadResult:
                                            WCFClient.Client.PillDataRead(payload);
                                            break;
                                        case (byte) GateToServerCommands.PillWriteResult:
                                            WCFClient.Client.PillWriteCompleted(payload);
                                            break;
                                        case (byte) GateToServerCommands.PinSignalSet:
                                            WCFClient.Client.PinSet(payload);
                                            break;
                                    }
                                    LogClass.Write("Should send from gate to server this: " + outputCommandString);
                                }
                                if (Enum.IsDefined(typeof (ArmletToServerCommands), outputBytes[0]))
                                {
                                    byte commandByte = outputBytes[0];
                                    byte[] payload = new byte[outputBytes.Length - 1];
                                    Buffer.BlockCopy(outputBytes, 1, payload, 0, payload.Length);
                                    //outputBytes.CopyTo(payload, 1);

                                    switch (commandByte)
                                    {
                                        case (byte) ArmletToServerCommands.TXCompleted:
                                            WCFClient.Client.TXCompleted(payload);
                                            break;
                                        case (byte) ArmletToServerCommands.RXCompleted:
                                            byte armlet_id = outputBytes[1];
                                            byte data_count = outputBytes[2];
                                            if (data_count >= 2)
                                            {
                                                updates.Add(
                                                    new PlayerUpdate()
                                                        {
                                                            ArmletID = armlet_id,
                                                            NewRoom = outputBytes[3],
                                                            NewBlood = outputBytes[4]
                                                        });
                                            }
                                            if (data_count > 2)
                                            {
                                                byte[] rxDataPaylod = new byte[data_count - 2];
                                                Buffer.BlockCopy(outputBytes, 5, rxDataPaylod, 0, rxDataPaylod.Length);
                                                //outputBytes.CopyTo(rxDataPaylod, 5);
                                                WCFClient.Client.ArmletSendsData(armlet_id, rxDataPaylod);
                                            }
                                            break;
                                    }
                                    LogClass.Write("Should send from armlet to server this: " + outputCommandString);
                                }
                                outputMessageQueue.Dequeue();
                            }
                            catch (Exception e)
                            {
                                NetworkProblemHandler();
                            }
                        }
                        if (updates.Count != 0)
                        {
                            try
                            {
                                WCFClient.Client.ArmlteStatusUpdate(updates.ToArray());
                                LogClass.Write("Presence data of " + updates.Count + " players is send.");
                                updates.Clear();
                            }
                            catch (Exception e)
                            {
                                NetworkProblemHandler();
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
