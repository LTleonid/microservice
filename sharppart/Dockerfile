FROM mcr.microsoft.com/dotnet/sdk:10.0 AS build

WORKDIR /App

COPY . ./

RUN dotnet restore ./sharppart/sharppart.csproj

RUN dotnet publish ./sharppart/sharppart.csproj -o out -p:PublishSingleFile=trued

FROM mcr.microsoft.com/dotnet/aspnet:10.0

WORKDIR /App

COPY --from=build /App/out .

ENTRYPOINT ["./sharppart"]