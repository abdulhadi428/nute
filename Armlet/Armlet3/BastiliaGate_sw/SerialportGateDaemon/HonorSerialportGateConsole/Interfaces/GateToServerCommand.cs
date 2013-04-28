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

    public class GateToServerCommand
    {
        public GateToServerCommand(GateToServerCommands commandId, byte[] data)
        {
            CommandInit = byte.Parse("#");
            CommandCode = commandId;
            Data = data;
            CommandTerminator = new byte[] {byte.Parse("\r"), byte.Parse("\n")};
        }
        public byte CommandInit { get; private set; }                       //Should be '#'
        public GateToServerCommands CommandCode { get; private set; }     //One of ServerToGateCommands enum
        public byte[] Data { get; private set; }                            //Data payload (0-252 bytes)
        public byte[] CommandTerminator { get; private set; }               //Should be '\r\n'
    }
}
