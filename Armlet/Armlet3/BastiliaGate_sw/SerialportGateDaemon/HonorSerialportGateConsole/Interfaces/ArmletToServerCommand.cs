namespace HonorSerialportGateConsole.Interfaces
{
    public enum ArmletToServerCommands: byte
    {
        
        TXCompleted = 0xA1,
        RXCompleted = 0xA4

    }

    public class ArmletToServerCommand
    {
        public ArmletToServerCommand(ArmletToServerCommands commandId, byte[] data)
        {
            CommandInit = byte.Parse("#");
            CommandCode = commandId;
            Data = data;
            CommandTerminator = new byte[] {byte.Parse("\r"), byte.Parse("\n")};
        }
        public byte CommandInit { get; private set; }                       //Should be '#'
        public ArmletToServerCommands CommandCode { get; private set; }     //One of ServerToGateCommands enum
        public byte[] Data { get; private set; }                            //Data payload (0-252 bytes)
        public byte[] CommandTerminator { get; private set; }               //Should be '\r\n'
    }
}
