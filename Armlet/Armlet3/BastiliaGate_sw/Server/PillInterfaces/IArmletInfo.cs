namespace PillInterfaces
{
    public interface IArmletInfo
    {
        void SetName(string text);
        void SendMessage(string text);
        void SetRegeneration(byte selectedValue);

        int Id { get; }
        int Room { get; }
        int BloodLevel { get; }
        string Name { get; }
    }
}