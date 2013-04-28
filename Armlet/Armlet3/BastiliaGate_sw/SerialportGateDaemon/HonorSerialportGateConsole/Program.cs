using System;
using System.Collections;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Timers;

using Timer = System.Timers.Timer;


namespace HonorSerialportGateConsole
{
    class Program
    {
        static int Main(string[] args)
        {
            var daemon = new HonorSerialportDaemon();
            daemon.RunMailCycle();

            Console.ReadLine();
            return 0;
        }
    }
}
