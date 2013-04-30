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
    public class SerialPortSettings
    {
        public int BaudRate;
        public Parity Parity;
        public StopBits StopBits;
        public int DataBits;
        public Handshake Handshake;
    }

    public class StronglyTypedSerialPortConnector 
    {
        private static SerialPort port;
        private static bool _isReading = true;
        private static Queue inputQueue ;
        private static Queue outputQueue; 

        private Thread readThread = new Thread(SerialPortRead);
        private Thread writeThread = new Thread(WriteToSerialPort);
        private static bool _isWriting = true;


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
                    StopBits = settings.StopBits,
                    WriteTimeout = 10000
                };
            port.ErrorReceived += PortErrorReceived;
        }

       
        public void SearchForPortAndConnect()
        {
            var portNames = SerialPort.GetPortNames();
            if (!portNames.Any(TryOpenPort))
            {
                throw new Exception(string.Format("Tried {0} port(s). None answered.", portNames.Length));
            }
            
            _isReading = true;
            readThread.Start();
            _isWriting = true;
            writeThread.Start();
            
            if (!port.IsOpen)
            {
                throw new Exception("Couldn't open port");
            }     //TODO MAKE ERROR Enum
        }

        private static bool TryOpenPort(string portName)
        {
            try
            {
                port.PortName = portName;
                port.Open();
                port.WriteLine("#01");
                port.ReadTimeout = 1000;
                var answer = port.ReadLine().Trim().Replace(",", "");
                if (answer != "#9000")
                {
                    port.Close();
                    return false;
                }
                port.ReadTimeout = 0;
                return true;
            }
            catch (Exception)
            {
                if (port.IsOpen)
                {
                    port.Close();
                }
                return false;
            }
        }

        private static void WriteToSerialPort()
        {
            while (_isWriting)
            {
                if (inputQueue.Count > 0)
                {
                    lock (inputQueue.SyncRoot)
                    {
                        while (inputQueue.Count > 0)
                        {
                            var command = (Command) inputQueue.Dequeue();
                            string commandString = command.ConvertToString();
                            LogClass.WritePacket("Pipe -> Com: ", commandString);
                            port.WriteLine(commandString);
                        }
                    }
                }
            }
        }

        private  static void SerialPortRead()
        {
            while (_isReading)
            {
                try
                {
                    string message = port.ReadLine();
                    LogClass.WritePacket("COM -> Pipe: ", message);
                    outputQueue.Enqueue(message);
                }
                catch (TimeoutException) { }
            }
        }
        
        
        static void PortErrorReceived(object sender, SerialErrorReceivedEventArgs e)
        {
            LogClass.Write(string.Format("Port error: {0}", e));
        }

    }
}
