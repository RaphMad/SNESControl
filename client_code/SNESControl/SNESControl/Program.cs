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

            // initialize file
            string fileName = FindArg(argList, "-file");
            if (fileName == null)
            {
                Console.WriteLine("Please supply a file name via '-file <fileName'>!");
                Console.ReadKey();
                return;
            }

            try
            {
                // set up the serial connection
                _serialConnector = new SerialConnector(argList[1]);
                _serialConnector.Start();
            }
            catch (Exception e)
            {
                Console.WriteLine("Cannot access port " + port + ". Error: " + e.Message);
                Console.ReadKey();
                return;
            }

            // set up file reader and writer
            _replayFileWriter = new ReplayFileWriter(fileName);
            _serialConnector.UseReplayFileWriter(_replayFileWriter);

            _replayFileReader = new ReplayFileReader(fileName);
            _serialConnector.UseReplayFileReader(_replayFileReader);

            Console.WriteLine("Commands:");
            Console.WriteLine();
            Console.WriteLine("'s': save to file");
            Console.WriteLine("'l': load from file");
            Console.WriteLine("'x': stop saving / loading");
            Console.WriteLine("'i': request information");
            Console.WriteLine("'p': send a PING request containing 32 bytes");
            Console.WriteLine("'ESC': quit");
            Console.WriteLine();

            ProcessKeys();

            _serialConnector.Stop();
            _replayFileWriter.Close();

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
                                     { ConsoleKey.S, Save },
                                     { ConsoleKey.L, Load },
                                     { ConsoleKey.X, StopSaveLoad }
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
            byte[] bytes = Enumerable.Range(0, 32).Select(x => (byte)x).ToArray();
            _serialConnector.StartPingTimer();
            _serialConnector.SendData(MessageType.Ping, bytes);
        }

        static void Save()
        {
            Console.WriteLine("Saving to file");
            _replayFileWriter.Open();
            _serialConnector.SendData(MessageType.EnableSave, new byte[] { });
        }

        static void Load()
        {
            Console.WriteLine("Loading from file");
            _replayFileReader.Read();
            _serialConnector.SendData(MessageType.EnableLoad, new byte[] { });
        }

        static void StopSaveLoad()
        {
            Console.WriteLine("Stopping saving/loading.");
            _replayFileWriter.Close();
            _serialConnector.SendData(MessageType.DisableSave, new byte[] { });
            _serialConnector.SendData(MessageType.DisableLoad, new byte[] { });
        }
    }
}