using Google.Protobuf.Collections;
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
        private readonly CSharpToPython.CSharpToPythonClient _python;

        public Process(CSharpToPython.CSharpToPythonClient python)
        {
            _python = python;
        }

        [HttpPost]
        public async Task<global::Response> Post(DataOutput data) 
        {
            data.provide.Add("C#");
            //Сюда ща полетит grpc
            global::Request req = new();
            req.Text = data.text;
            req.Length = data.Length;
            req.Provide.AddRange(data.provide);
            var res = await _python.GetCountAsync(req);
            return res; 

        }

    }
}
