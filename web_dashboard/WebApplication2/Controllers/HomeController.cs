namespace WebApplication2.Controllers
{
    using System.Diagnostics;
    using Microsoft.AspNetCore.Mvc;
    using SensorsClient.Models;
    using WebApplication2.Models;

    public class HomeController : Controller
    {
        DeviceService deviceService = new DeviceService();

        public IActionResult Index()
        {
            return View();
        }

        public IActionResult Monitor()
        {
            return View();
        }

        public void SendToDevice(string message)
        {
            deviceService.SendMessageAsync(message).Wait();
        }

        public IActionResult GetUpdate()
        {
            var update = deviceService.GetUpdate();

            if (int.TryParse(update.Humidity.Split('.')[0], out int h) && int.TryParse(update.Temperature.Split('.')[0], out int t))
            {
                var data = new SensorData
                {
                    Humidity = h,
                    Temperature = t,
                    Timestamp = update.TimeStamp,
                    LedOn = update.LedOn
                };
                return base.Json(data);
            }

            return base.BadRequest();
        }

        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }
    }
}
