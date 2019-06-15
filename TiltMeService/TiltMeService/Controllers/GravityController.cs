using Microsoft.AspNetCore.Mvc;
using TiltMe.DataContext;

namespace TiltMe.Service.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class GravityController : ControllerBase
    {
        private readonly ITiltMeDataService _tiltMeDataService;

        public GravityController(ITiltMeDataService tiltMeDataService)
        {
            _tiltMeDataService = tiltMeDataService;
        }

        // POST api/values
        [HttpPost]
        public void Post(int value, string sensorId)
        {
            _tiltMeDataService.CreateMeasurement(sensorId, value);
        }
    }
}
