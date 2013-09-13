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
                    daemon.RunMainCycle();
                }
                return 0;
            }
            catch (Exception exc)
            {
                LogClass.Write(exc.Message);
                Console.WriteLine(exc.ToString());
                LogClass.Write("Global Try-Catch triggered. Killing myself");
                return 1;
            }
            
        }
    }
}
