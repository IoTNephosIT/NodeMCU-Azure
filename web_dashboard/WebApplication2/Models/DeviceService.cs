namespace WebApplication2.Models
{
    using Microsoft.Azure.Devices;
    using Microsoft.Azure.Documents;
    using Microsoft.Azure.Documents.Client;
    using Newtonsoft.Json;
    using System;
    using System.Linq;
    using System.Text;

    public class Item
    {
        [JsonProperty(PropertyName = "id")]
        public string Id { get; set; }

        [JsonProperty(PropertyName = "t")]
        public string Temperature { get; set; }

        [JsonProperty(PropertyName = "h")]
        public string Humidity { get; set; }

        [JsonProperty(PropertyName = "tavg")]
        public string TemperatureAverage { get; set; }

        [JsonProperty(PropertyName = "havg")]
        public string HumidityAverage { get; set; }

        [JsonProperty(PropertyName = "l")]
        public bool LedOn { get; set; }

        [JsonProperty(PropertyName = "_ts")]
        public string TimeStamp { get; set; }

    }

    public class DeviceService
    {
        private static readonly string Endpoint = "";
        private static readonly string Key = "";
        private static readonly string DatabaseId = "sensordata";
        private static readonly string CollectionId = "daily";
        private static readonly string IoTHubConnectionString = "";
        private static string deviceID = "";
        private static DocumentClient docClient;
        private static ServiceClient serviceClient;

        public DeviceService()
        {
            docClient = new DocumentClient(new Uri(Endpoint), Key);
            serviceClient = ServiceClient.CreateFromConnectionString(IoTHubConnectionString, TransportType.Amqp);
        }

        public async System.Threading.Tasks.Task SendMessageAsync(string messageToSend)
        {
            var message = new Message(Encoding.ASCII.GetBytes(messageToSend));
            await serviceClient.SendAsync(deviceID, message)    ;
        }

        public Item GetUpdate()
        {
            try
            {
                var doc = docClient.CreateDocumentQuery<Item>(UriFactory.CreateDocumentCollectionUri(DatabaseId, CollectionId)).ToArray();
                return doc.AsEnumerable().Where(item => item.Temperature != "nan" && item.Humidity != "nan").OrderByDescending(i => i.TimeStamp).First();
            }
            catch (DocumentClientException e)
            {
                if (e.StatusCode == System.Net.HttpStatusCode.NotFound)
                {
                    return null;
                }
                else
                {
                    throw;
                }
            }
        }
    }
}
