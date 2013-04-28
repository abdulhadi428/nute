namespace PillInterfaces
{
    public interface IArmletInfo
    {
        void SetName(string text);
        void SendMessage(string text);
        void SetRegeneration(byte selectedValue);

        byte Id { get; }
        byte Room { get; }
        byte BloodLevel { get; }
        string Name { get; }
        void SetLockList(byte[] lockList);
    }
}