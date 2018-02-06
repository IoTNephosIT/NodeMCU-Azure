using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using SensorsClient.Models;
using WebApplication2.Models;

namespace WebApplication2.Controllers
{
    public class HomeController : Controller
    {
        DocumentService cosmosAPIService = new DocumentService();

           public IActionResult Index()
        {
            return View();
        }

        public IActionResult Monitor()
        {
            return View();
        }

        public IActionResult GetUpdate()
        {
            var update = cosmosAPIService.GetUpdate();

            if(int.TryParse( update.Humidity.Split('.')[0], out int h) && int.TryParse(update.Temperature.Split('.')[0], out int t))
            {
                var data = new SensorData
                {
                    Humidity = h,
                    Temperature = t,
                    Timestamp = update.TimeStamp,
                    HumidityAverage = 10, //r.HumidityAverage,
                    TemperatureAverage = 12 //r.TemperatureAverage
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
