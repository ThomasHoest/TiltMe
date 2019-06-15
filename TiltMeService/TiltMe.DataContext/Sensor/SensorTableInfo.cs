using System;
using Microsoft.WindowsAzure.Storage.Table;

namespace TiltMe.DataContext.Sensor
{
    public class SensorTableInfo : TableEntity
    {
        public SensorTableInfo()
        {
            
        }

        public SensorTableInfo(string sensorId, string ownerEmail)
        {
            RowKey = SensorId = sensorId;
            PartitionKey = OwnerEmail = ownerEmail;
            Created = DateTime.UtcNow;
        }

        public string OwnerEmail { get; set; }

        public string SensorId { get; set; }

        public DateTime Created { get; set; }
    }
}