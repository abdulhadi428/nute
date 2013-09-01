using System.Collections.Concurrent;

namespace HonorSerialportGateConsole
{
    static class LogClass
    {
        public static readonly ConcurrentQueue<string> Items = new ConcurrentQueue<string>();
        private static bool _isVerbose;

        public static void SetVerbosity(bool verbosityLevel)
        {
            _isVerbose = verbosityLevel;
        }
        public static void  Write(string v)
        {
            Items.Enqueue(v);
        }
        public static void WritePacket(string h, string packet)
        {
            if (_isVerbose)
            {
                Write(h + packet);
            }
        }
    }
}
