using System;
using System.Threading.Tasks;
using TiltMe.DataContext.Gravity;
using TiltMe.DataContext.Sensor;
using TiltMe.Models;

namespace TiltMe.DataContext
{
    public class TiltMeDataService : ITiltMeDataService
    {
        private readonly IGravityData _gravityData;
        private readonly ISensorData _sensorData;

        private string _ownerEmail = "zoglub@gmail.com";
        
        public TiltMeDataService(IGravityData gravityData, ISensorData sensorData)
        {
            _gravityData = gravityData;
            _sensorData = sensorData;
        }

        public async Task<bool> CreateSensor(string sensorId)
        {
            SensorInfo sensor = new SensorInfo() { OwnerEmail = _ownerEmail, SensorId = sensorId, Created = DateTime.UtcNow};
            return await _sensorData.SaveNewSensor(sensor);
        }

        public Task<SensorInfo> GetSensor(string sensorId)
        {
            return _sensorData.GetSensorById(sensorId, _ownerEmail);
        }

        public async Task CreateMeasurement(string sensorId, int gravity)
        {
            SensorInfo sensor = await _sensorData.GetSensorById(sensorId, _ownerEmail);

            if (sensor == null)
            {
                await CreateSensor(sensorId);
                sensor = await _sensorData.GetSensorById(sensorId, _ownerEmail);
            }

            GravityInfo gravityInfo = new GravityInfo()
            {
                SensorId = sensor.SensorId,
                Gravity = gravity,
                MeasurementTime = DateTime.UtcNow,
                Id = Guid.NewGuid().ToString()
            };

            await _gravityData.SaveGravity(gravityInfo);

        }
    }
}