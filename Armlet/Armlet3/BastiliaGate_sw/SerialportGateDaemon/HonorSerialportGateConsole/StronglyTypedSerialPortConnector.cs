using System;
using System.Collections;
using System.IO.Ports;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using HonorSerialportGateConsole.Interfaces;
using HonorSerialportGateDaemon;

namespace HonorSerialportGateConsole
{
    class StronglyTypedSerialPortConnector : IStronglyTypedSerialPortConnector
    {
        
        private static SerialPort port;
        private static bool _continue;
        private static Queue inputQueue ;
        private static Queue outputQueue; 

        private Thread readThread = new Thread(SerialPortRead);
        
       

        public StronglyTypedSerialPortConnector(Queue _inputQuere, Queue _outputQueue, SerialPortSettings settings)
        {
            inputQueue = _inputQuere;
            outputQueue = _outputQueue;
            port = new SerialPort()
                {
                    BaudRate = settings.BaudRate,
                    DataBits = settings.DataBits,
                    Handshake = settings.Handshake,
                    Parity = settings.Parity,
                    StopBits = settings.StopBits
                };
        }

        private bool CheckingConnectivity = false;
        public int SearchForPortAndConnect()
        {
            var ports = System.IO.Ports.SerialPort.GetPortNames();
            if (ports.Count() == 0)
            {
                return -5;                       //TODO: Make an audible enum to fire "No port found" exception
            }
            port.PortName = ports[0];            //TODO: Search all available COM ports in case of multiple Ports present in the system
            
            port.WriteTimeout = 10000;
            port.Open();
            _continue = true;
            readThread.Start();
            
            port.ErrorReceived += PortErrorReceived;
           

            if (!port.IsOpen) { return -1; }     //TODO MAKE ERROR Enum
            


            CheckingConnectivity = true;
            SendCommandToGate(new ServerToGateCommand(ServerToGateCommands.Ping, new byte[] { }));
            
            return 0;
        }

        public static void SerialPortRead()
        {
            while (_continue)
            {
                try
                {
                    string message = port.ReadLine();
                    LogClass.WritePacket("COM -> Pipe:", message);
                    outputQueue.Enqueue(message);
                }
                catch (TimeoutException) { }
            }
        }
       


        public bool IsConnected { get; private set; }

        public int SendCommandToArmlet(ServerToArmletCommand armletCommand)
        {
            string S = armletCommand.ConvertToString();
            LogClass.WritePacket("Pipe->COM:", S);
            //lock (_lockPort)
            {
                if (port.BytesToWrite > 0)
                {
                    throw new Exception("Write");
                }
                
                port.WriteLine(S);
            }
            return 0;
        }

        public int SendCommandToGate(ServerToGateCommand gateCommand)
        {
            string S = gateCommand.ConvertToString();
            LogClass.WritePacket("Pipe->COM:", S);
            //lock (_lockPort)
            {
                if (port.BytesToWrite > 0)
                {
                    throw new Exception("Write");
                }
                
                port.WriteLine(S);
            }
            return 0;
        }

        public event EventHandler<ReceivedArmletDataEventArgs> ReceivedArmletData;
        public event EventHandler<ReceivedGateDataEventArgs> ReceivedGateData;

        static void PortErrorReceived(object sender, SerialErrorReceivedEventArgs e)
        {
            LogClass.Write(string.Format("Port error: {0}", e));
        }

    }
}
