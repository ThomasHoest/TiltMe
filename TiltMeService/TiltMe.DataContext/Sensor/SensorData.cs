using System.Collections.Generic;
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
        private IMapper _mapper;

        public SensorData(IApplicationConfiguration applicationConfiguration)
            : base(applicationConfiguration)
        {
            _sensorTable = _tableClient.GetTableReference(nameof(SensorData));
            _sensorTable.CreateIfNotExistsAsync();

            var config = new MapperConfiguration(cfg =>
            {
                cfg.CreateMap<SensorInfo, SensorTableInfo>()
                    .ForMember(m => m.PartitionKey, s => s.MapFrom(si=>si.OwnerEmail))
                    .ForMember(m => m.RowKey, s => s.MapFrom(si => si.SensorId));
                cfg.CreateMap<SensorTableInfo, SensorInfo>();
            });

            _mapper = new Mapper(config);
        }

        public Task<bool> SaveNewSensor(SensorInfo sensor)
        {
            SensorTableInfo tableSensor = _mapper.Map<SensorTableInfo>(sensor);
            return Insert(_sensorTable, tableSensor);
        }

        public Task<bool> SensorExists(SensorInfo sensor)
        {
            SensorTableInfo tableSensor = _mapper.Map<SensorTableInfo>(sensor);
            return Exists(_sensorTable, tableSensor);
        }

        public async Task<SensorInfo> GetSensorById(string sensorId, string ownerEmail)
        {
            SensorTableInfo sensor = new SensorTableInfo(sensorId, ownerEmail);
            sensor = await Get(_sensorTable, sensor);
            return _mapper.Map<SensorInfo>(sensor);
        }

        public async Task<IList<SensorInfo>> GetSensors(string ownerEmail)
        {
            var sensors = await GetAll<SensorTableInfo>(_sensorTable, ownerEmail);
            return _mapper.Map<IList<SensorTableInfo>, IList<SensorInfo>>(sensors);
        }
    }
}