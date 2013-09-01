using System;
using System.Collections.Concurrent;
using System.Configuration;
using System.Linq;
using System.Reflection;
using System.ServiceModel;
using System.ServiceModel.Configuration;
using System.Threading;
using System.Timers;
using HonorSerialportGateConsole.Interfaces;
using HonorSerialportGateConsole.Properties;
using HonorSerialportGateConsole.ServerWCFService;


namespace HonorSerialportGateConsole
{
    public class HonorSerialportDaemon : IDisposable
    {
        private StronglyTypedSerialPortConnector TypedSerialPort;
        private readonly Thread sendThread;

        public readonly ConcurrentQueue<Command> inputMessageQueue = new ConcurrentQueue<Command>();
        private readonly ConcurrentQueue<string> _outputMessageQueue = new ConcurrentQueue<string>();

        private  bool _sending = false;
        private readonly InstanceContext instanceContext;
        private byte serverProvidedGateId;
        private string RemoteAddress;

        private System.Timers.Timer UpdateLogTimer;

        public HonorSerialportDaemon()
        {
            InitiateLog();
            sendThread = new Thread(SendToServer);
            instanceContext = new InstanceContext(new WCFCallbackHandler(this));

            serverProvidedGateId = Byte.Parse(Settings.Default.PreferedGateId);

            ResolveRemoteAddress();

            try
            {
                ConnectToServer();
            }
            catch (EndpointNotFoundException)
            {
                _sending = false;
                WCFClient.Client = null;
                LogClass.Write("Connection Failed");
                LogClass.Write("");
                while (!RetryConnection())
                {
                    LogClass.Write("Retry in 5 secs..");
                    Thread.Sleep(5000);
                }
                _sending = true;
            }
        }

        private void ConnectToServer()
        {
            LogClass.Write("Connect to " + RemoteAddress);
            WCFClient.Client = new GateWCFServiceClient(instanceContext, "NetTcpBindingEndpoint");
            serverProvidedGateId = WCFClient.Client.RegisterGate(serverProvidedGateId);
            LogClass.Write("Connection to server successfully established. This is now Gate #" + serverProvidedGateId);
            Console.Title = string.Format("SerialportGateDaemon {0} Gate #{1}", Assembly.GetEntryAssembly().GetName().Version, serverProvidedGateId);
            ((ICommunicationObject) WCFClient.Client).Faulted += HonorSerialportDaemon_Faulted;
            ((ICommunicationObject) WCFClient.Client).Closed += HonorSerialportDaemon_Closed;
            inputMessageQueue.Enqueue(new ServerToGateCommand(ServerToGateCommands.SetGateNum, new[] { serverProvidedGateId }));
            _sending = true;
        }

        #region ReconnectLogic

        private void ResolveRemoteAddress()
        {
            var clientSettings =
                (ClientSection) ConfigurationManager.GetSection("system.serviceModel/client");

            var endpoint =
                clientSettings.Endpoints.Cast<ChannelEndpointElement>()
                    .FirstOrDefault(e => e.Name == "NetTcpBindingEndpoint");

            RemoteAddress = endpoint == null ? null : endpoint.Address.ToString();
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
            LogClass.Write("Now retring to restore connection as Gate #" + serverProvidedGateId);

            try
            {
               ConnectToServer();
            }
            catch (EndpointNotFoundException)
            {
                LogClass.Write("Can't connect to server...");
                return false;
            }
            return true;
        }

        #endregion

        #region InitLog
        private void InitiateLog()
        {
            LogClass.SetVerbosity(Settings.Default.LogIsVerbose);
            UpdateLogTimer = new System.Timers.Timer(1000) { AutoReset = true };
            UpdateLogTimer.Elapsed += UpdateLog_Elapsed;
            UpdateLogTimer.Start();
        }
        static void UpdateLog_Elapsed(object sender, ElapsedEventArgs e)
        {
            do
            {
                string result;
                var somethingToWrite = LogClass.Items.TryDequeue(out result);
                if (!somethingToWrite)
                {
                    return;
                }
                Console.WriteLine(result);
            } while (true);
        }

#endregion

       
        public void RunMainCycle()
        {
            LogClass.Write("Opening COM port");
            TypedSerialPort = new StronglyTypedSerialPortConnector(
                inputMessageQueue, _outputMessageQueue);
            TypedSerialPort.SearchForPortAndConnect();

            sendThread.Start();
            Console.ReadLine();
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
                        tryDequeue = _outputMessageQueue.TryDequeue(out outputCommandString);
                        if (tryDequeue)
                        {
                            while (!_sending)
                            {
                                Thread.Sleep(1000);
                            }
                            SendMessage(outputCommandString);
                        }
                    }
                    Thread.Sleep(300); //Отправляем каждые 300 секунду.
                }
            }
            catch (Exception e)
            {
                LogClass.Write("Exception in sending thread " + e.Message);
                Console.WriteLine(e);
                Environment.Exit(1);
            }
        }

        private static void SendMessage(string outputCommandString)
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
                        return;
                    case (byte) GateToServerCommands.GateNumberSet:
                        return;
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
                        byte armletID = outputBytes[1];
                        byte dataCount = outputBytes[2];
                        if (dataCount >= 2)
                        {
                            WCFClient.Client.ArmlteStatusUpdateAsync(new[]
                                {
                                    new PlayerUpdate
                                        {
                                            ArmletID = armletID,
                                            NewRoom = outputBytes[3],
                                            NewBlood = outputBytes[4],
                                            NewToxin = outputBytes [5],
                                            NewPulse = outputBytes [6],
                                            NewTemp = outputBytes [7]
                                        }
                                });
                        }
                        if (dataCount > 2)
                        {
                            byte[] rxDataPaylod = outputBytes.Skip(2).ToArray();
                            WCFClient.Client.ArmletSendsDataAsync(armletID, rxDataPaylod);
                        }
                        break;
                }
            }
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
