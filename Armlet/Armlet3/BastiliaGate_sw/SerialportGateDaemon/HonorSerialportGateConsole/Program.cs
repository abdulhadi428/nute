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
            for (int i = 0; i < 100; i++)
            {

                var updatecom = new byte[] {164, (byte) i, 2, (byte) rand.Next(), (byte) rand.Next()};
                var updateString = Command.ByteArrayToHexString(updatecom);

                daemon.outputMessageQueue.Enqueue(updateString);
            }
            Console.ReadLine();
            for (int i = 0; i < 100; i++)
            {

                var updatecom = new byte[] { 164, (byte)i, 2, (byte)rand.Next(), (byte)rand.Next() };
                var updateString = Command.ByteArrayToHexString(updatecom);

                daemon.outputMessageQueue.Enqueue(updateString);
            }
            return 0;
        }
    }
}
