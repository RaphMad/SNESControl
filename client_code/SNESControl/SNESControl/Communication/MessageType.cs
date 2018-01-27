namespace SNESControl
{
    public enum MessageType
    {
        // start with a value of 3 for more efficient encoding (the encoding will use 2 bytes for 0,1,2)
        EnableSave = 3,
        Save = 4,
        EnableLoad = 5,
        Load = 6,
        Ping = 7,
        Pong = 8,
        Print = 9,
        RequestStatus = 10
    }
}