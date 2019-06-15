using System;
using Microsoft.WindowsAzure.Storage.Table;

namespace TiltMe.DataContext.Gravity
{
    public class GravityTableInfo : TableEntity
    {
        public GravityTableInfo()
        {
            
        }

        public GravityTableInfo(int gravity, string sensorId)
        {
            Gravity = gravity;
            MeasurementTime = DateTime.UtcNow;
            RowKey = Id = Guid.NewGuid().ToString();
            PartitionKey = SensorId = sensorId;
        }

        public string Id { get; set; }

        public string SensorId { get; set; }

        public int Gravity { get; set; }

        public DateTime MeasurementTime { get; set; }
    }
}