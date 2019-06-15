
using System.Threading.Tasks;
using TiltMe.Models;

namespace TiltMe.DataContext.Gravity
{
    public interface IGravityData
    {
        Task SaveGravity(GravityInfo gravity);
    }
}