using System.Threading.Tasks;
using TiltMe.DataContext.Gravity;
using TiltMe.DataContext.Sensor;

namespace TiltMe.DataContext
{
    public class TiltMeDataService : ITiltMeDataService
    {
        private readonly IGravityData _gravityData;
        private readonly ISensorData _sensorData;
        
        public TiltMeDataService(IGravityData gravityData, ISensorData sensorData)
        {
            _gravityData = gravityData;
            _sensorData = sensorData;
        }

        public Task CreateSensor(string sensorId)
        {
            throw new System.NotImplementedException();
        }

        public Task<SensorTableInfo> GetSensor(string sensorId)
        {
            throw new System.NotImplementedException();
        }

        public Task CreateMeasurement(string sensorId, int gravity)
        {
            throw new System.NotImplementedException();
        }
    }
}