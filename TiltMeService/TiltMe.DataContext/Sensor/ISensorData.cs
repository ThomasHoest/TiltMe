using System.Collections.Generic;
using System.Threading.Tasks;
using TiltMe.Models;

namespace TiltMe.DataContext.Sensor
{
    public interface ISensorData
    {
        Task<bool> SaveNewSensor(SensorInfo sensor);

        Task<IList<SensorInfo>> GetSensors(string ownerEmail);

        Task<bool> SensorExists(SensorInfo sensor);

        Task<SensorInfo> GetSensorById(string sensorId, string ownerEmail);
    }
}