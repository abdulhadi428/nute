namespace HonorSerialportGateConsole.Interfaces
{
    public enum ArmletToServerCommands: byte
    {
        TXCompleted = 0xA1,
        RXCompleted = 0xA4
    }

    public class ArmletToServerCommand :Command
    {
        public ArmletToServerCommand(ArmletToServerCommands commandId, byte[] data)
        {
            CommandCodeByte = (byte) commandId;
            Data = data;
        }
    }
}
