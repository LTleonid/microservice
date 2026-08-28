FROM alpine:3.22 AS builder

RUN apk add --no-cache \
    build-base \
    cmake \
    ninja \
    pkgconf \
    protobuf-dev \
    protoc \
    rabbitmq-c-dev

WORKDIR /app

COPY cpppart/CMakeLists.txt ./

COPY cpppart/src ./src

COPY shared ./shared

RUN protoc --proto_path=./shared --cpp_out=./src ./shared/process.proto

RUN cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release

RUN cmake --build build --parallel



FROM alpine:3.22

RUN apk add --no-cache protobuf rabbitmq-c

WORKDIR /app

COPY --from=builder /app/build/cpp_worker /app/cpp_worker

CMD ["/app/cpp_worker"]