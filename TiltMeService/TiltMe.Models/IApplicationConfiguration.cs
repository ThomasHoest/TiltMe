namespace TiltMe.Models
{
    public interface IApplicationConfiguration
    {
        string TableStorageConnectionString { get; set; }
    }
}