namespace SensorsClient.Models
{
    public class SensorData
    {
        public int Temperature { get; set; }
        public int Humidity { get; set; }
        public int TemperatureAverage { get; set; }
        public int HumidityAverage { get; set; }
        public string Timestamp { get; set; }

        public bool LedOn { get; set; }
    }
}
