

FROM golang:1.25 AS build
WORKDIR /app
COPY go.mod ./
RUN go mod download

COPY . .

RUN CGO_ENABLED=0 go build -o gateway .

FROM alpine:latest

WORKDIR /app

COPY --from=build /app/gateway .

EXPOSE 8080

CMD [ "./gateway" ]
