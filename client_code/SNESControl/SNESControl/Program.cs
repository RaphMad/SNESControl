namespace SNESControl
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Threading;

    class Program
    {
        ////private static FileStream _saveToFile;

        ////private static List<byte> _readFromFileBytes;

        ////private static int _readFromFileIndex;

        static void Main(string[] args)
        {
            var argList = new List<string>(args);

            if (argList[0] != "-port")
            {
                Console.WriteLine("Please supply a port via '--port <portName'>!");
                Console.ReadKey();
                return;
            }

            ////int saveToFileIndex = argList.FindIndex(x => x == "-saveToFile");

            ////if (saveToFileIndex != -1)
            ////{
            ////    _saveToFile = File.Open(argList[saveToFileIndex + 1], FileMode.Append, FileAccess.Write);
            ////}

            ////int readFromFileIndex = argList.FindIndex(x => x == "-loadFromFile");

            ////if (readFromFileIndex != -1)
            ////{
            ////    string content = File.ReadAllText(args[readFromFileIndex + 1]);
            ////    string[] textualBytes = content.Split(' ', '\r', '\n');
            ////    _readFromFileBytes = textualBytes.Where(x => !string.IsNullOrEmpty(x)).Select(parseByte).ToList();
            ////}

            var serialConnector = new SerialConnector(argList[1]);
            serialConnector.Start();

            Console.WriteLine("Press any key to send some bytes.");
            Console.ReadKey();

            byte[] bytes = Enumerable.Range(0, 64).Select(x => (byte)x).ToArray();
            serialConnector.SendData(MessageType.Ping, bytes);

            Console.WriteLine("Press any key to request data.");
            Console.ReadKey();

            serialConnector.SendData(MessageType.RequestStatus, new byte[]{});

            Console.WriteLine("Press any key to quit");
            Console.ReadKey();

            serialConnector.Stop();

            // wait some time to allow the file handles to be closed
            Thread.Sleep(1000);
        }

        ////static byte parseByte(string input)
        ////{
        ////    return byte.Parse(input);
        ////}

        ////static void ProcessSerialInput()
        ////{
        ////    _port.Open();

        ////    if (_readFromFileBytes != null)
        ////    {
        ////        _port.WriteLine("ENABLE REPLAY");
        ////    }

        ////    if (_saveToFile != null)
        ////    {
        ////        _port.WriteLine("ENABLE SAVE");
        ////    }

        ////    while (_processSerialInput)
        ////    {
        ////        string nextCommand = _port.ReadLine();

        ////        if (nextCommand.StartsWith("SAVE|"))
        ////        {
        ////            if (_saveToFile != null)
        ////            {
        ////                string data = nextCommand.Substring(5) + Environment.NewLine;
        ////                byte[] bytes = Encoding.ASCII.GetBytes(data);
        ////                _saveToFile.Write(bytes, 0, bytes.Length);
        ////            }
        ////        }
        ////        else if (nextCommand.StartsWith("LOAD|"))
        ////        {
        ////            int amountOfBytes = int.Parse(nextCommand.Substring(5));
        ////            if (_readFromFileBytes != null && _readFromFileIndex < _readFromFileBytes.Count - amountOfBytes)
        ////            {
        ////                byte[] nextBytes = new byte[amountOfBytes];
        ////                _readFromFileBytes.CopyTo(0, nextBytes, _readFromFileIndex, amountOfBytes);

        ////                _port.Write(nextBytes, 0, 2);
        ////                _readFromFileIndex += amountOfBytes;
        ////            }
        ////        }
        ////        else
        ////        {
        ////            Console.WriteLine("Received incomplete line: " + nextCommand + " " + DateTime.Now.Millisecond);
        ////        }
        ////    }

        ////    _saveToFile?.Dispose();
        ////}
    }
}