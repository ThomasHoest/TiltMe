using System.Threading.Tasks;

namespace TiltMe.DataContext.Sensor
{
    public interface ISensorData
    {
        Task SaveNewSensor(string id);
    }
}