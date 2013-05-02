using System;
using HonorSerialportGateConsole.Interfaces;


namespace HonorSerialportGateConsole
{
    class Program
    {

        static int Main(string[] args)
        {
            
            var daemon = new HonorSerialportDaemon();
            daemon.RunMailCycle();
            var rand = new Random();
            

            Console.ReadLine();
            
            return 0;
        }
    }
}
