namespace HonorSerialportGateConsole.Interfaces
{
    public enum ServerToGateCommands : byte
    {
        Ping = 0x01,
        SetGateNum = 0x10,
        
        GetPillStatus = 0x30,
        WriteDataToPill = 0x31,
        ReadDataFromPill = 0x32,
        InitiatePinSignal = 0x40,
    }

    public class ServerToGateCommand : Command
    {
        public ServerToGateCommand(ServerToGateCommands commandId, byte[] data)
        {
            CommandCodeByte = (byte) commandId;
            Data = data;
        }
        
    }
}
