namespace HonorSerialportGateConsole.Interfaces
{
    public  enum GateToServerCommands: byte
    {
        Ack = 0x90,
        GateNumberSet = 0x0A,

        PillConnectedStatus = 0xC0,
        PillWriteResult = 0xC1,
        PillReadResult = 0xC2,

        PinSignalSet = 0xD0
    }

    public class GateToServerCommand :Command
    {
        public GateToServerCommand(GateToServerCommands commandId, byte[] data)
        {
            CommandCodeByte  = (byte) commandId;
            Data = data;
        }
       
    }
}
