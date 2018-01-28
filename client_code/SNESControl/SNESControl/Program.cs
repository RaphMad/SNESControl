namespace SNESControl
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Threading;
    using SaveToFile;

    class Program
    {
        private static SerialConnector _serialConnector;
        private static ReplayFileWriter _replayFileWriter;
        private static ReplayFileReader _replayFileReader;

        static void Main(string[] args)
        {
            var argList = new List<string>(args);

            // parse arguments
            string port = FindArg(argList, "-port");
            if (port == null)
            {
                Console.WriteLine("Please supply a port via '-port <portName'>! (Usually 'COM3')");
                Console.ReadKey();
                return;
            }

            // set up the serial connection
            _serialConnector = new SerialConnector(argList[1]);
            _serialConnector.Start();

            Console.WriteLine("Commands:");
            Console.WriteLine();
            Console.WriteLine("'i': request information");
            Console.WriteLine("'p': send a PING request containing 64 bytes");
            Console.WriteLine("'s': stop saving to a file");
            Console.WriteLine("'l': stop loading from a file");
            Console.WriteLine("'ESC': quit");
            Console.WriteLine();

            // initialize saving to file if needed
            string saveToFile = FindArg(argList, "-saveToFile");
            if (saveToFile != null)
            {
                _replayFileWriter = new ReplayFileWriter(saveToFile);
                _serialConnector.UseReplayFileWriter(_replayFileWriter);
                _serialConnector.SendData(MessageType.EnableSave, new byte[] { });

                Console.WriteLine("Saving incoming inputs to file <" + saveToFile + ">.");
                Console.WriteLine();
            }

            // initialize saving to file if needed
            string loadFromFile = FindArg(argList, "-loadFromFile");
            if (loadFromFile != null)
            {
                _replayFileReader = new ReplayFileReader(loadFromFile);
                _serialConnector.UseReplayFileReader(_replayFileReader);
                _serialConnector.SendData(MessageType.EnableLoad, new byte[] { });

                Console.WriteLine("Loading inputs from file <" + loadFromFile + ">.");
                Console.WriteLine();
            }

            ProcessKeys();

            _serialConnector.Stop();
            _replayFileWriter?.Close();

            // wait some time to allow for cleanup 
            Thread.Sleep(1000);
        }

        static string FindArg(List<string> argList, string argName)
        {
            string value = null;
            int index = argList.IndexOf(argName);

            if (index != -1 && argList.Count > index + 1 && !string.IsNullOrEmpty(argList[index + 1]))
            {
                value = argList[index + 1];
            }

            return value;
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
                                     { ConsoleKey.P, SendPing },
                                     { ConsoleKey.S, StopSaving },
                                     { ConsoleKey.L, StopLoading }
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
            byte[] bytes = Enumerable.Range(0, 64).Select(x => (byte)x).ToArray();
            _serialConnector.SendData(MessageType.Ping, bytes);
        }

        static void StopSaving()
        {
            Console.WriteLine("Stop saving to file");
            _replayFileWriter?.Close();
            _serialConnector.SendData(MessageType.DisableSave, new byte[] { });
        }

        static void StopLoading()
        {
            Console.WriteLine("Stop loading from file");
            _replayFileWriter?.Close();
            _serialConnector.SendData(MessageType.DisableLoad, new byte[] { });
        }
    }
}