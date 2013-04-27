using System;
using System.IO.Ports;

namespace HonorSerialportGateConsole.Interfaces
{
    public interface IStronglyTypedSerialPortConnector
    {
        int SearchForPortAndConnect();
        bool IsConnected { get; }

        int SendCommandToArmlet(ServerToArmletCommand armletCommand);
        int SendCommandToGate(ServerToGateCommand gateCommand);
        
        event EventHandler<ReceivedArmletDataEventArgs> ReceivedArmletData;
        event EventHandler<ReceivedGateDataEventArgs> ReceivedGateData;
    }

    public class SerialPortSettings
    {
        public int BaudRate;
        public Parity Parity;
        public StopBits StopBits;
        public int DataBits;
        public Handshake Handshake;
    }

    public class ReceivedArmletDataEventArgs : EventArgs
    {
        public ReceivedArmletDataEventArgs(ArmletToServerCommand data)
        {
            ArmletCommand = data;
        }
        public ArmletToServerCommand ArmletCommand { get; private set; }
    }

    public class ReceivedGateDataEventArgs : EventArgs
    {
        public ReceivedGateDataEventArgs(GateToServerCommand data)
        {
            GateCommand = data;
        }
        public GateToServerCommand GateCommand { get; private set; }
    }
}
