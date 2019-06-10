namespace TiltMe.Models
{
    public class ApplicationConfiguration : IApplicationConfiguration
    {
        public string TableStorageConnectionString { get; set; }
    }
}