namespace SNESControl
{
    using System;
    using System.Collections.Generic;
    using System.IO.Ports;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;
    using SaveToFile;

    public class SerialConnector
    {
        private readonly SerialPort _port;

        private volatile bool _isProcessingData = true;

        public SerialConnector(string portName)
        {
            _port = new SerialPort(portName, 115200) { NewLine = "\r\n" };
        }

        public void Start()
        {
            _port.Open();

            Task.Factory.StartNew(ProcessSerialData);
        }

        public void Stop()
        {
            _isProcessingData = false;
        }

        public void SendData(MessageType type, byte[] data)
        {
            byte typeByte = (byte)type;
            var bytesToEncode = new List<byte> { typeByte };
            bytesToEncode.AddRange(data);

            byte[] encodedBytes = MessageCoder.Encode(bytesToEncode.ToArray());
            _port.Write(encodedBytes, 0, encodedBytes.Length);
        }

        private void ProcessSerialData()
        {
            var receivedBytes = new List<int>();
            bool receiveInProgress = false;

            while (_isProcessingData)
            {
                int receivedByte = _port.ReadByte();

                if (receivedByte == MessageCoder.StartMarker)
                {
                    if (receivedBytes.Count > 0)
                    {
                        Console.ForegroundColor = ConsoleColor.Red;
                        Console.WriteLine("Received incomplete message: <" + BitConverter.ToString(receivedBytes.Select(x => (byte)x).ToArray()) + ">");
                        Console.ForegroundColor = ConsoleColor.White;
                    }

                    receivedBytes.Clear();
                    receiveInProgress = true;
                }

                if (receiveInProgress)
                {
                    receivedBytes.Add(receivedByte);
                }

                if (receiveInProgress && receivedByte == MessageCoder.EndMarker)
                {
                    receiveInProgress = false;

                    DecodeReceivedMessage(receivedBytes);
                    receivedBytes.Clear();
                }
            }

            _port.Close();
        }

        private void DecodeReceivedMessage(List<int> receivedBytes)
        {
            // decode content
            byte[] decodedBytes = MessageCoder.Decode(receivedBytes.Select(x => (byte)x).ToArray());

            // call message handler
            var handlers = new Dictionary<MessageType, Action<byte[]>>
                           {
                               { MessageType.Ping, ProcessPing },
                               { MessageType.Pong, ProcessPong },
                               { MessageType.Print, ProcessPrint },
                               { MessageType.Save, SaveButtonData }
                           };

            MessageType type = (MessageType)decodedBytes[0];

            if (handlers.ContainsKey(type))
            {
                handlers[type](decodedBytes.Skip(1).ToArray());
            }
        }

        private void ProcessPing(byte[] data)
        {
            // simply answer with received data
            SendData(MessageType.Pong, data);
        }

        private void ProcessPong(byte[] data)
        {
            Console.WriteLine("PONG <" + BitConverter.ToString(data) + ">");
        }

        private void ProcessPrint(byte[] data)
        {
            Console.WriteLine("PRINT <" + Encoding.ASCII.GetString(data) + ">");
        }

        private ReplayFileWriter _replayFileWriter;

        public void UseReplayFileWriter(ReplayFileWriter replayFileWriter)
        {
            _replayFileWriter = replayFileWriter;
        }

        void SaveButtonData(byte[] data)
        {
            _replayFileWriter.AppendBytes(data);
        }
    }
}