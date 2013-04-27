namespace HonorSerialportGateConsole.Interfaces
{
    public enum ServerToArmletCommands: byte
    {
        SendDataToArmlet = 0x11
    }

    public class ServerToArmletCommand : Command
    {
        public ServerToArmletCommand(ServerToArmletCommands commandId, byte[] data)
        {
            CommandCodeByte = (byte)commandId;
            Data = data;
        }
        
    }
}
