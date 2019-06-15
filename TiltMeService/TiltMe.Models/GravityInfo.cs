using System;

namespace TiltMe.Models
{
    public class GravityInfo
    {
        public string Id { get; set; }

        public string SensorId { get; set; }

        public int Gravity { get; set; }

        public DateTime MeasurementTime { get; set; }
    }
}