using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.ObjectPool;
using sharppart.Models;
using System.Text.Json;
namespace MyApp.Namespace
{
    [Route("api/[controller]")]
    [ApiController]
    public class Process : ControllerBase
    {

        // Вот и интерфейсы бы пригодились) но лень хотя
        // public record class DataInput(string text, List<string> provide);
        // public record class DataOutput(string text, int lenght , List<string> provide);
        // POST api/<process>
        [HttpPost]
        public DataOutput Post(DataOutput data) 
        {
            data.provide.Add("C#");
            return data; 

        }

    }
}
