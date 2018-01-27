namespace SNESControl
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Threading;

    class Program
    {
        private static SerialConnector _serialConnector;

        static void Main(string[] args)
        {
            var argList = new List<string>(args);

            if (argList[0] != "-port")
            {
                Console.WriteLine("Please supply a port via '--port <portName'>!");
                Console.ReadKey();
                return;
            }

            _serialConnector = new SerialConnector(argList[1]);
            _serialConnector.Start();

            Console.WriteLine("Commands:");
            Console.WriteLine();
            Console.WriteLine("'i': request information from backend");
            Console.WriteLine("'p': send a PING request containing 256 bytes");
            Console.WriteLine("'ESC': quit");
            Console.WriteLine();

            ProcessKeys();

            _serialConnector.Stop();

            // wait some time to allow for cleanup 
            Thread.Sleep(1000);
        }

        static void ProcessKeys()
        {
            ConsoleKey pressedKey = ConsoleKey.OemClear;

            while (pressedKey != ConsoleKey.Escape)
            {
                pressedKey = Console.ReadKey(true).Key;

                var keyActions = new Dictionary<ConsoleKey, Action>
                                 {
                                     { ConsoleKey.I, RequestInfo },
                                     { ConsoleKey.P, SendPing }
                                 };

                if (keyActions.ContainsKey(pressedKey))
                {
                    keyActions[pressedKey]();
                }
            }
        }

        static void RequestInfo()
        {
            _serialConnector.SendData(MessageType.RequestStatus, new byte[] { });
        }

        static void SendPing()
        {
            byte[] bytes = Enumerable.Range(0, 256).Select(x => (byte)x).ToArray();
            _serialConnector.SendData(MessageType.Ping, bytes);
        }
    }
}