using System.Threading.Tasks;
using AutoMapper;
using Microsoft.WindowsAzure.Storage.Table;
using TiltMe.DataContext.TableStorage;
using TiltMe.Models;

namespace TiltMe.DataContext.Sensor
{
    public class SensorData : TableDbContext, ISensorData
    {
        private CloudTable _sensorTable;
        private Mapper _mapper;

        public SensorData(IApplicationConfiguration applicationConfiguration)
            : base(applicationConfiguration)
        {
            _sensorTable = _tableClient.GetTableReference(nameof(SensorData));
            _sensorTable.CreateIfNotExistsAsync();

            var config = new MapperConfiguration(cfg => {
                cfg.CreateMap<SensorInfo, SensorTableInfo>();
                cfg.CreateMap<SensorTableInfo, SensorInfo>();
            });
            _mapper = new Mapper(config);
        }

        public Task SaveNewSensor(string id)
        {
            throw new System.NotImplementedException();
        }
    }
}