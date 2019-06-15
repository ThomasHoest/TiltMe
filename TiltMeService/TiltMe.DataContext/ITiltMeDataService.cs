using System.Threading.Tasks;
using TiltMe.DataContext.Sensor;
using TiltMe.Models;

namespace TiltMe.DataContext
{
    public interface ITiltMeDataService
    {
        Task<bool> CreateSensor(string sensorId);
        Task<SensorInfo> GetSensor(string sensorId);
        Task CreateMeasurement(string sensorId, int gravity);
    }
}