
using System.Threading.Tasks;

namespace TiltMe.DataContext.Gravity
{
    public interface IGravityData
    {
        Task SaveGravity(int gravity, string sensorId);
    }
}