using System;
using Microsoft.WindowsAzure.Storage.Table;

namespace TiltMe.DataContext.Gravity
{
    public class GravityTableInfo : TableEntity
    {
        public GravityTableInfo()
        {

        }
        public GravityTableInfo(int gravity)
        {
            Gravity = gravity;
            MeasurementTime = DateTime.UtcNow;
        }

        public string Id { get; set; }
        public int Gravity { get; set; }
        public DateTime MeasurementTime { get; set; }
    }
}