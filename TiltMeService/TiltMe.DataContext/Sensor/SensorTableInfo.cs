using System;
using Microsoft.WindowsAzure.Storage.Table;

namespace TiltMe.DataContext.Sensor
{
    public class SensorTableInfo : TableEntity
    {
        public SensorTableInfo()
        {
            Created = DateTime.UtcNow;
        }

        private string SensorId { get; set; }
        private DateTime Created { get; set; }
    }
}