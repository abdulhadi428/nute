using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HonorSerialportGateDaemon
{
    class LogClass
    {
        public static List<string> Items = new List<string>();
        private static bool isVerbose = false;

        public static void SetVerbosity(bool verbosityLevel)
        {
            isVerbose = verbosityLevel;
        }
        public static void  Write(string v)
        {
            lock (Items)
            {
                Items.Add(v);
            }
        }
        public static void WritePacket(string h, string packet)
        {
            if (isVerbose)
            {
                Write(h + packet);
            }
        }
        

        

    }
}
