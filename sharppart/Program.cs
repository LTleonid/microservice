var builder = WebApplication.CreateBuilder(args);
builder.Services.AddAuthorization();
builder.Services.AddControllers();
builder.Services.AddGrpcClient<CSharpToPython.CSharpToPythonClient>(options =>
{
    options.Address= new Uri("http://localhost:50051");

});
var app = builder.Build();

// Configure the HTTP request pipeline.
if (!app.Environment.IsDevelopment())
{
    app.UseExceptionHandler("/Error");
    // The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
    app.UseHsts();
}

app.UseRouting();
app.UseAuthentication();
app.UseAuthorization();

app.MapControllers();

app.Run();
