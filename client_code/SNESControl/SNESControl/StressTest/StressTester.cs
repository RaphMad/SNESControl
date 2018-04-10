namespace SNESControl.StressTest
{
    using System;
    using System.Linq;

    public class StressTester
    {
        private readonly SerialConnector _serialConnector;

        private readonly Random _random = new Random();

        private bool _isRunning;

        private byte[] _lastSentBytes = new byte[0];

        private ulong sentBytes;

        private ulong byteErrors;

        public StressTester(SerialConnector serialConnector)
        {
            _serialConnector = serialConnector;
            _serialConnector.PongReceived += HandlePong;
        }

        public void Start()
        {
            _isRunning = true;
            sentBytes = 0;
            byteErrors = 0;
            Console.WriteLine();

            SendRandomData();
        }

        private void SendRandomData()
        {
            _lastSentBytes = Enumerable.Range(0, 32).Select(x => (byte)_random.Next(33)).ToArray();
            sentBytes += 32;

            _serialConnector.StartPingTimer();
            _serialConnector.SendData(MessageType.Ping, _lastSentBytes);
        }

        private void HandlePong(object sender, byte[] e)
        {
            if (_isRunning)
            {
                for (int i = 0; i < _lastSentBytes.Length; i++)
                {
                    if (_lastSentBytes[i] != e[i])
                    {
                        byteErrors++;
                    }
                }

                PrintStatistics();

                if (_isRunning)
                {
                    SendRandomData();
                }
            }
        }

        private void PrintStatistics()
        {
            Console.WriteLine($"Bytes sent / Byte errors: {sentBytes} / {byteErrors}");
        }

        public void Stop()
        {
            Console.WriteLine();
            _isRunning = false;
        }
    }
}