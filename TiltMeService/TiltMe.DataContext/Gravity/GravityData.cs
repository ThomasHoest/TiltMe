using System.Threading.Tasks;
using AutoMapper;
using Microsoft.WindowsAzure.Storage.Table;
using TiltMe.DataContext.TableStorage;
using TiltMe.Models;

namespace TiltMe.DataContext.Gravity
{
    public class GravityData : TableDbContext, IGravityData
    {
        private CloudTable _gravityTable;
        private IMapper _mapper;

        public GravityData(IApplicationConfiguration applicationConfiguration)
            : base(applicationConfiguration)
        {
            _gravityTable = _tableClient.GetTableReference(nameof(GravityData));
            _gravityTable.CreateIfNotExistsAsync();
            var config = new MapperConfiguration(cfg => {
                cfg.CreateMap<GravityInfo, GravityTableInfo>()
                    .ForMember(m => m.PartitionKey, s => s.MapFrom(si => si.SensorId))
                    .ForMember(m => m.RowKey, s => s.MapFrom(si => si.Id));
                cfg.CreateMap<GravityTableInfo, GravityInfo>();
            });
            _mapper = new Mapper(config);

        }

        public async Task SaveGravity(GravityInfo gravity)
        {
            GravityTableInfo gravityTableInfo = _mapper.Map<GravityTableInfo>(gravity);
            await Insert(_gravityTable, gravityTableInfo);
        }
    }
}
