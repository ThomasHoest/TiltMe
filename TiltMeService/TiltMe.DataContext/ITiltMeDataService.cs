using System.Threading.Tasks;
using TiltMe.DataContext.Sensor;

namespace TiltMe.DataContext
{
    public interface ITiltMeDataService
    {
        Task CreateSensor(string sensorId);
        Task<SensorTableInfo> GetSensor(string sensorId);
        Task CreateMeasurement(string sensorId, int gravity);
    }
}