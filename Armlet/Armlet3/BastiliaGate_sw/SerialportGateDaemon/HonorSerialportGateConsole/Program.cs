using System;
using System.Reflection;


namespace HonorSerialportGateConsole
{
    static class Program
    {

        static int Main()
        {
            Console.WriteLine("SerialportGateDaemon " + Assembly.GetEntryAssembly().GetName().Version);
            try
            {
                using (var daemon = new HonorSerialportDaemon())
                {
                    daemon.RunMailCycle();
                }
                return 0;
            }
            catch (Exception exc)
            {
                
                Console.WriteLine(exc.ToString());
                return 1;
            }
            
        }
    }
}
