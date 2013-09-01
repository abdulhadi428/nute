using System;
using System.Collections.Concurrent;
using System.IO.Ports;
using System.Linq;
using System.Threading;
using HonorSerialportGateConsole.Interfaces;
using HonorSerialportGateConsole.Properties;

namespace HonorSerialportGateConsole
{
    public class StronglyTypedSerialPortConnector  : IDisposable
    {
        private readonly SerialPort _port;
        private readonly ConcurrentQueue<Command> _inputQueue;
        private readonly ConcurrentQueue<string> _outputQueue; 
        private readonly object _syncRoot = new object();

        private readonly Thread _readThread;
        private readonly Thread _writeThread;


        public StronglyTypedSerialPortConnector(ConcurrentQueue<Command> inputQuere, ConcurrentQueue<string> outputQueue)
        {
            _writeThread = new Thread(WriteToSerialPort);
            _readThread = new Thread(SerialPortRead);
            _inputQueue = inputQuere;
            _outputQueue = outputQueue;
            _port = new SerialPort
                {
                    BaudRate = 115200,
                    DataBits = 8,
                    Handshake = Handshake.None,
                    Parity = Parity.None,
                    StopBits = StopBits.One,
                    WriteTimeout = 10000,
                    ReadTimeout =1000,
                    ReadBufferSize = 1024*1024
                };
            _port.ErrorReceived += PortErrorReceived;
        }

       
        public void SearchForPortAndConnect()
        {
            var defaultPortName = Settings.Default.DefaultComPortName;
            LogClass.Write("Using default port name: " + defaultPortName);
            if (!TryOpenPort(defaultPortName))
            {
               
                var portNames = SerialPort.GetPortNames();
                var firstPortName = portNames.FirstOrDefault();
                if (firstPortName != null)
                {
                    LogClass.Write("Default port failed. Using first encountered port " + firstPortName);
                    TryOpenPort(firstPortName);
                }
            }
            if (!_port.IsOpen)
            {
                LogClass.Write("COM port opening failed entirely. Aborting operations");
                throw new Exception("Couldn't open port");
            }
            LogClass.Write("Com port opened! Starting reading and writing threads");
            _readThread.Start();
            _writeThread.Start();
        }

        private  bool TryOpenPort(string portName)
        {
            try
            {
                _port.PortName = portName;
                _port.Open();
                _port.WriteLine("#01");
                //port.ReadTimeout = 1000;
                //var answer = port.ReadLine().Trim().Replace(",", "");
                //if (answer != "#9000")
                //{
                //    port.Close();
                //    return false;
                //}
                _port.ReadTimeout = 1000;
                return true;
            }
            catch (Exception)
            {
                LogClass.Write("Error at opening port named " + portName);
                if (_port.IsOpen)
                {
                    _port.Close();
                }
                return false;
            }
        }

        private void WriteToSerialPort()
        {
            try
            {
                while (true)
                {

                    Command result;
                    while (_inputQueue.TryDequeue(out result))
                    {
                        string commandString = result.ConvertToString();
                        LogClass.Write("Pipe -> Com pending");
                        lock (_syncRoot)
                        {
                            _port.WriteLine(commandString);
                        }
                        LogClass.WritePacket("Pipe -> Com: ", commandString);
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

        private void SerialPortRead()
        {
            try
            {
                while (true)
                {
                    while (!_inputQueue.IsEmpty)
                    {
                        Thread.Sleep(0);
                    }
                    string message;
                    try
                    {
                        lock (_syncRoot)
                        {
                            message = _port.ReadLine();
                        }
                    }
                    catch (TimeoutException)
                    {
                        continue;
                    }
                    
                    LogClass.WritePacket("COM -> Pipe: ", message);
                    _outputQueue.Enqueue(message);

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
            if (_port != null && _port.IsOpen)
            {
                _port.Close();
            }
        }
    }
}
