namespace SNESControl.SaveToFile
{
    using System;
    using System.IO;

    public class ReplayFileWriter
    {
        private readonly string _fileName;

        private FileStream _file;

        private volatile bool _isOpen;

        public ReplayFileWriter(string fileName)
        {
            _fileName = fileName;
        }

        public void Open()
        {
            try
            {
                _isOpen = false;
                _file = File.Open(_fileName, FileMode.Create, FileAccess.Write);
                _isOpen = true;
            }
            catch (Exception e)
            {
                Console.WriteLine("Could not open file for writing: " + e.Message);
            }
        }

        public void AppendBytes(byte[] bytes)
        {
            if (_isOpen)
            {
                _file?.Write(bytes, 0, bytes.Length);
                _file?.Flush(true);
            }
        }

        public void Close()
        {
            _file?.Dispose();
            _isOpen = false;
        }
    }
}