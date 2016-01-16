namespace Ex_6
{
    public interface ILogger
    {
        void Log(LoggingType entry, string s);
    }
    public enum LoggingType { Error , Warning , Info };


}