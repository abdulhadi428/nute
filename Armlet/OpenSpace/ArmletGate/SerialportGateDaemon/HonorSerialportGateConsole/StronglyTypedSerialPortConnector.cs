using System;
using System.Collections;
using System.Collections.Concurrent;
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
    public class StronglyTypedSerialPortConnector  : IDisposable
    {
        private static SerialPort port;
        private static ConcurrentQueue<Command> inputQueue;
        private static ConcurrentQueue<string> outputQueue; 
        private static object _syncRoot = new object();

        private Thread readThread = new Thread(SerialPortRead);
        private Thread writeThread = new Thread(WriteToSerialPort);


        public StronglyTypedSerialPortConnector(ConcurrentQueue<Command> _inputQuere, ConcurrentQueue<string> _outputQueue)
        {
            inputQueue = _inputQuere;
            outputQueue = _outputQueue;
            port = new SerialPort
                {
                    BaudRate = 115200,
                    DataBits = 8,
                    Handshake = Handshake.None,
                    Parity = Parity.None,
                    StopBits = StopBits.One,
                    WriteTimeout = 10000,
                    ReadTimeout = SerialPort.InfiniteTimeout,
                    ReadBufferSize = 1024*1024
                };
            port.ErrorReceived += PortErrorReceived;
        }

       
        public void SearchForPortAndConnect()
        {
            var portNames = SerialPort.GetPortNames().First();
            TryOpenPort(portNames);
            if (!port.IsOpen)
            {
                throw new Exception("Couldn't open port");
            }     //TODO MAKE ERROR Enum
            readThread.Start();
            writeThread.Start();
        }

        private static bool TryOpenPort(string portName)
        {
            try
            {
                port.PortName = portName;
                port.Open();
                port.WriteLine("#01");
                //port.ReadTimeout = 1000;
                //var answer = port.ReadLine().Trim().Replace(",", "");
                //if (answer != "#9000")
                //{
                //    port.Close();
                //    return false;
                //}
                port.ReadTimeout = SerialPort.InfiniteTimeout;
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
            try
            {
                while (true)
                {

                    Command result;
                    while (inputQueue.TryDequeue(out result))
                    {
                        string commandString = result.ConvertToString();
                        LogClass.WritePacket("Pipe -> Com: ", commandString);
                        lock (_syncRoot)
                        {
                            port.WriteLine(commandString);
                        }
                    }
                    Thread.Sleep(1000);

                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                Environment.Exit(1);
            }
        }

        private static void SerialPortRead()
        {
            try
            {
                while (true)
                {
                    string message;
                    lock (_syncRoot)
                    {
                        message = port.ReadLine();
                    }
                    LogClass.WritePacket("COM -> Pipe: ", message);
                    outputQueue.Enqueue(message);

                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                Environment.Exit(1);
            }
        }


        static void PortErrorReceived(object sender, SerialErrorReceivedEventArgs e)
        {
            LogClass.Write(string.Format("Port error: {0}", e));
        }

        public void Dispose()
        {
            if (port != null && port.IsOpen)
            {
                port.Close();
            }
        }
    }
}
