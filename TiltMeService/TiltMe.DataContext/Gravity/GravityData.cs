using System.Threading.Tasks;
using AutoMapper;
using Microsoft.WindowsAzure.Storage.Table;
using TiltMe.DataContext.TableStorage;
using TiltMe.Models;

namespace TiltMe.DataContext.Gravity
{
    public class GravityData : TableDbContext, IGravityData
    {
        private CloudTable _userTable;
        private IMapper _mapper;

        public GravityData(IApplicationConfiguration applicationConfiguration)
            : base(applicationConfiguration)
        {
            _userTable = _tableClient.GetTableReference(nameof(GravityData));
            _userTable.CreateIfNotExistsAsync();
            var config = new MapperConfiguration(cfg => {
                cfg.CreateMap<GravityInfo, GravityTableInfo>();
                cfg.CreateMap<GravityTableInfo, GravityInfo>();
            });
            _mapper = new Mapper(config);

        }

        public Task SaveGravity(int gravity, string sensorId)
        {
            throw new System.NotImplementedException();
        }
    }
}
