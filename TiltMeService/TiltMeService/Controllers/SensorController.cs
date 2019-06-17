using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using TiltMe.DataContext;
using TiltMe.Models;

namespace TiltMe.Service.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class SensorController : ControllerBase
    {
        private readonly ITiltMeDataService _tiltMeDataService;

        public SensorController(ITiltMeDataService tiltMeDataService)
        {
            _tiltMeDataService = tiltMeDataService;
        }

        [HttpGet]
        public Task<SensorInfo> Get(string sensorId)
        {
            return _tiltMeDataService.GetSensor(sensorId);
        }

        [HttpGet]
        [Route("sensors")]
        public async Task<SensorInfo[]> GetSensors()
        {
            var sensors = await _tiltMeDataService.GetSensors();
            return sensors?.ToArray();
        }
    }
}