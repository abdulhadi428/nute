namespace HonorSerialportGateConsole.Interfaces
{
    public  enum GateToServerCommands: byte
    {
        Ack = 0x90,
        PillReadResult = 0xC2,
     

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
