using System;


namespace HonorSerialportGateConsole
{
    static class Program
    {

        static int Main()
        {
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
