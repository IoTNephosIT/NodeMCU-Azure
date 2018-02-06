namespace WebApplication2.Models
{
    using Microsoft.Azure.Documents;
    using Microsoft.Azure.Documents.Client;
    using Newtonsoft.Json;
    using System;
    using System.Linq;

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

        [JsonProperty(PropertyName = "_ts")]
        public string TimeStamp { get; set; }

    }

    public class DocumentService
    {
        private static readonly string Endpoint = "";
        private static readonly string Key = "";
        private static readonly string DatabaseId = "";
        private static readonly string CollectionId = "";
        private static DocumentClient docClient;

        public DocumentService()
        {
            docClient = new DocumentClient(new Uri(Endpoint), Key);
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
