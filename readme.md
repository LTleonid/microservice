# microservice project for study
> | GO | Python | C#/.NET | CPP |

127.0.0.1:8080 ->(raw-Json) Go <-> (Json/HTTP) C# <-> (grpc/HTTP) -> Pythpn <-> RabbitMQ <-> C++ <br>

# Work in docker compose
Просьбы от ИИ:
# TODO

## P0 — Critical

### RabbitMQ

* [ ] Добавить manual ACK после успешной обработки сообщения в C++ worker
* [ ] Не подтверждать сообщения при ошибке обработки
* [ ] Определить поведение при ошибках: `requeue` или DLQ
* [ ] Обработать потерю RabbitMQ connection
* [ ] Добавить reconnect для C++ worker
* [ ] Добавить reconnect для Python
* [ ] Проверить потерю сообщений при перезапуске C++ worker
* [ ] Проверить duplicate message processing

### Go

* [ ] Добавить HTTP timeout
* [ ] Использовать `r.Context()` при downstream requests
* [ ] Корректно обрабатывать ошибки `io.ReadAll`
* [ ] Корректно обрабатывать ошибки `json.Marshal`
* [ ] Корректно обрабатывать ошибки `http.NewRequest`
* [ ] Корректно обрабатывать ошибки `http.Client.Do`
* [ ] Корректно обрабатывать ошибки чтения response body
* [ ] Всегда возвращать HTTP status code при ошибке
* [ ] Добавить `Content-Type: application/json`
* [ ] Добавить ограничение размера request body
* [ ] Закрывать `response.Body`

### Protobuf

* [ ] Исправить `lenght` на `length`
* [ ] Перегенерировать protobuf-код
* [ ] Проверить все места использования изменённого поля
* [ ] Разделить request и response messages
* [ ] Документировать protobuf fields
* [ ] Определить правила изменения protobuf contract

---

## P1 — Reliability

### Request ID

* [ ] Генерировать `request_id` на входе в Go
* [ ] Передавать `request_id` Go → C#
* [ ] Передавать `request_id` C# → Python
* [ ] Передавать `request_id` Python → RabbitMQ
* [ ] Возвращать `request_id` C++ → Python
* [ ] Передавать `request_id` обратно по всей цепочке
* [ ] Добавлять `request_id` в логи
* [ ] Рассмотреть использование `X-Request-ID`
* [ ] Рассмотреть RabbitMQ `correlation_id`

### Python

* [ ] Вынести `pending_requests` в отдельный компонент
* [ ] Удалять Future после успешного ответа
* [ ] Удалять Future после timeout
* [ ] Удалять Future после exception
* [ ] Обработать неизвестный `request_id`
* [ ] Обработать response, пришедший после timeout
* [ ] Ограничить количество pending requests
* [ ] Проверить поведение при нескольких Python instances

### Timeout / Retry

* [ ] Добавить timeout Go → C#
* [ ] Добавить timeout C# → Python
* [ ] Добавить timeout Python → C++
* [ ] Определить единый timeout policy
* [ ] Добавить retry только для retryable errors
* [ ] Добавить exponential backoff
* [ ] Ограничить количество retries

---

## P1 — Healthchecks

* [ ] Добавить `/health` в Go
* [ ] Добавить health endpoint в C#
* [ ] Добавить gRPC health checking в Python
* [ ] Добавить healthcheck C++ worker
* [ ] Добавить healthcheck RabbitMQ
* [ ] Добавить Docker healthcheck для сервисов
* [ ] Использовать `service_healthy` в `depends_on`
* [ ] Разделить liveness и readiness

---

## P1 — Graceful Shutdown

### Go

* [ ] Обработать `SIGTERM`
* [ ] Реализовать graceful HTTP shutdown
* [ ] Дождаться завершения активных requests

### C#

* [ ] Проверить graceful shutdown ASP.NET
* [ ] Корректно завершать gRPC connections

### Python

* [ ] Обработать `SIGTERM`
* [ ] Корректно остановить gRPC server
* [ ] Остановить RabbitMQ consumer
* [ ] Завершить pending requests
* [ ] Закрыть RabbitMQ connection

### C++

* [ ] Обработать `SIGTERM`
* [ ] Перестать принимать новые сообщения
* [ ] Дождаться текущего request
* [ ] Закрыть RabbitMQ channel
* [ ] Закрыть RabbitMQ connection

---

## P2 — C++ Worker

* [ ] Вынести RabbitMQ code из `main.cpp`
* [ ] Вынести message processing в отдельный модуль
* [ ] Вынести palindrome logic в отдельный модуль
* [ ] Добавить unit tests
* [ ] Добавить protobuf validation
* [ ] Добавить обработку malformed messages
* [ ] Добавить ACK после успешной обработки
* [ ] Настроить `prefetch_count`
* [ ] Проверить worker concurrency
* [ ] Рассмотреть worker threads
* [ ] Добавить compiler warnings
* [ ] Добавить `clang-format`
* [ ] Добавить `clang-tidy`
* [ ] Добавить AddressSanitizer
* [ ] Добавить UndefinedBehaviorSanitizer
* [ ] Добавить CTest

### Palindrome

* [ ] Проверить пустую строку
* [ ] Проверить строку из одного символа
* [ ] Проверить uppercase/lowercase
* [ ] Проверить пробелы
* [ ] Проверить punctuation
* [ ] Проверить специальные символы
* [ ] Проверить Unicode/UTF-8
* [ ] Проверить очень длинные строки

---

## P2 — Python

* [ ] Разделить gRPC server и RabbitMQ logic
* [ ] Вынести RabbitMQ client в отдельный модуль
* [ ] Вынести request correlation logic
* [ ] Вынести processing logic
* [ ] Добавить type hints
* [ ] Добавить `ruff`
* [ ] Добавить `mypy`
* [ ] Добавить `pytest`
* [ ] Добавить gRPC tests
* [ ] Добавить RabbitMQ integration tests
* [ ] Обработать cancellation
* [ ] Обработать timeout
* [ ] Обработать RabbitMQ reconnect
* [ ] Проверить memory leaks
* [ ] Проверить concurrent requests

---

## P2 — C#

### API

* [ ] Вынести business logic из Controller
* [ ] Создать service layer
* [ ] Вынести Python gRPC client в отдельный сервис
* [ ] Разделить HTTP DTO и protobuf DTO
* [ ] Добавить request validation
* [ ] Добавить error handling middleware
* [ ] Добавить корректные HTTP status codes
* [ ] Добавить structured logging

### gRPC

* [ ] Добавить timeout
* [ ] Передавать `CancellationToken`
* [ ] Обрабатывать `RpcException`
* [ ] Определить retry policy
* [ ] Передавать correlation ID
* [ ] Проверить поведение при недоступном Python

### Code quality

* [ ] Включить nullable reference types
* [ ] Включить .NET analyzers
* [ ] Проверить DI lifetimes
* [ ] Добавить unit tests
* [ ] Добавить integration tests

---

## P2 — Go

### HTTP

* [ ] Использовать нормальный router
* [ ] Явно определить API routes
* [ ] Возвращать `405` для неподдерживаемых методов
* [ ] Возвращать `400` для invalid JSON
* [ ] Возвращать `500` для внутренних ошибок
* [ ] Добавить request validation
* [ ] Добавить body size limit
* [ ] Добавить timeout
* [ ] Использовать request context

### Architecture

* [ ] Вынести C# HTTP client из handler
* [ ] Создать отдельный service/client layer
* [ ] Использовать interfaces для внешних dependencies
* [ ] Вынести configuration в отдельную структуру

### Code quality

* [ ] Использовать `log/slog`
* [ ] Добавить `go vet`
* [ ] Добавить `staticcheck`
* [ ] Добавить `golangci-lint`
* [ ] Добавить unit tests
* [ ] Добавить HTTP integration tests

---

## P2 — RabbitMQ

* [ ] Явно определить exchanges
* [ ] Явно определить queues
* [ ] Документировать routing
* [ ] Настроить durable queues
* [ ] Использовать persistent messages
* [ ] Добавить DLX
* [ ] Добавить DLQ
* [ ] Настроить message TTL где необходимо
* [ ] Настроить QoS/prefetch
* [ ] Определить delivery semantics
* [ ] Определить idempotency strategy
* [ ] Проверить duplicate messages

---

## P2 — Configuration

* [ ] Убрать `guest/guest` из кода
* [ ] Перенести RabbitMQ credentials в environment variables
* [ ] Убрать hardcoded service URLs
* [ ] Убрать hardcoded credentials
* [ ] Добавить `.env.example`
* [ ] Добавить `.env` в `.gitignore`
* [ ] Проверить Git history на случайно закоммиченные secrets
* [ ] Документировать environment variables

---

## P3 — Docker

### Dockerfiles

* [ ] Проверить multi-stage builds
* [ ] Использовать минимальные runtime images
* [ ] Не запускать сервисы от root без необходимости
* [ ] Добавить `.dockerignore`
* [ ] Зафиксировать версии base images
* [ ] Проверить размеры images
* [ ] Проверить images на CVE

### Docker Compose

* [ ] Добавить healthchecks
* [ ] Добавить restart policies
* [ ] Проверить `depends_on`
* [ ] Добавить resource limits
* [ ] Ограничить CPU
* [ ] Ограничить memory
* [ ] Настроить внутреннюю Docker network
* [ ] Не публиковать RabbitMQ наружу без необходимости
* [ ] Не публиковать внутренние сервисы наружу без необходимости

---

## P3 — Logging

* [ ] Ввести structured logging
* [ ] Использовать единый формат логов
* [ ] Добавить timestamp
* [ ] Добавить service name
* [ ] Добавить log level
* [ ] Добавить request ID
* [ ] Логировать duration downstream requests
* [ ] Логировать RabbitMQ processing time
* [ ] Логировать ошибки
* [ ] Убрать debug `print`/`cout` там, где нужен logger

---

## P3 — Metrics

* [ ] Добавить Prometheus
* [ ] Добавить request count
* [ ] Добавить request duration
* [ ] Добавить error count
* [ ] Добавить RabbitMQ processing duration
* [ ] Добавить queue depth
* [ ] Добавить количество pending Python requests
* [ ] Добавить timeout count
* [ ] Добавить retry count
* [ ] Добавить worker processing count

---

## P3 — Tests

### Unit tests

* [ ] Go handlers
* [ ] Go HTTP client
* [ ] C# services
* [ ] C# validation
* [ ] Python request manager
* [ ] Python processing
* [ ] C++ palindrome
* [ ] C++ protobuf processing

### Integration tests

* [ ] Go → C#
* [ ] C# → Python
* [ ] Python → RabbitMQ
* [ ] RabbitMQ → C++
* [ ] C++ → RabbitMQ
* [ ] Полный Go → C++ → Go pipeline

### Failure tests

* [ ] RabbitMQ unavailable
* [ ] Python unavailable
* [ ] C++ unavailable
* [ ] C++ crash during processing
* [ ] Python crash with pending requests
* [ ] RabbitMQ restart
* [ ] Duplicate message
* [ ] Invalid protobuf
* [ ] Invalid JSON
* [ ] Timeout
* [ ] Large request
* [ ] Empty request
* [ ] Concurrent requests

---

## P3 — CI/CD

* [ ] Добавить GitHub Actions
* [ ] Build Go
* [ ] Build C#
* [ ] Build Python
* [ ] Build C++
* [ ] Run unit tests
* [ ] Run integration tests
* [ ] Run linters
* [ ] Проверять protobuf generation
* [ ] Build Docker images
* [ ] Запускать Docker Compose в CI
* [ ] Проверять Docker images на vulnerabilities
* [ ] Добавить Dependabot или Renovate

---

## P3 — Documentation

* [ ] Переписать README
* [ ] Исправить опечатки
* [ ] Описать архитектуру
* [ ] Добавить architecture diagram
* [ ] Добавить sequence diagram
* [ ] Описать каждый сервис
* [ ] Описать HTTP API
* [ ] Описать gRPC API
* [ ] Описать RabbitMQ queues
* [ ] Описать protobuf contract
* [ ] Добавить пример HTTP request
* [ ] Добавить пример HTTP response
* [ ] Добавить инструкции запуска
* [ ] Добавить инструкции разработки
* [ ] Добавить troubleshooting
* [ ] Описать environment variables
* [ ] Описать запуск тестов

---

## P4 — Observability

### OpenTelemetry

* [ ] Добавить OpenTelemetry
* [ ] Создавать trace на входе Go
* [ ] Передавать trace Go → C#
* [ ] Передавать trace C# → Python
* [ ] Связать RabbitMQ messages с trace
* [ ] Передавать trace Python → C++
* [ ] Добавить Jaeger/Tempo для просмотра traces

---

## P4 — API

* [ ] Добавить OpenAPI/Swagger
* [ ] Описать request schema
* [ ] Описать response schema
* [ ] Описать error responses
* [ ] Описать HTTP status codes
* [ ] Описать timeout behavior
* [ ] Описать request limits

---

## P4 — Scaling

### Python

* [ ] Проверить несколько Python instances
* [ ] Исправить `pending_requests` для horizontal scaling
* [ ] Рассмотреть per-instance reply queues
* [ ] Рассмотреть RabbitMQ `correlation_id`

### C++

* [ ] Запустить несколько C++ workers
* [ ] Проверить распределение сообщений RabbitMQ
* [ ] Настроить prefetch
* [ ] Проверить duplicate processing
* [ ] Проверить graceful shutdown workers

---

## Definition of Done

* [ ] Все сервисы запускаются через Docker Compose
* [ ] Все сервисы имеют healthcheck
* [ ] Go корректно обрабатывает ошибки и timeout
* [ ] C# корректно обрабатывает gRPC errors и cancellation
* [ ] Python корректно управляет pending requests
* [ ] C++ корректно делает ACK
* [ ] Есть retry/requeue policy
* [ ] Есть DLQ
* [ ] Есть correlation ID
* [ ] Есть graceful shutdown
* [ ] Secrets не захардкожены
* [ ] Есть structured logs
* [ ] Есть unit tests
* [ ] Есть integration test полного pipeline
* [ ] CI проходит автоматически
* [ ] Docker images собираются в CI
* [ ] README описывает проект
* [ ] Есть architecture diagram
* [ ] Проверено поведение при падении каждого сервиса
* [ ] Проверен RabbitMQ restart
* [ ] Проверены concurrent requests
* [ ] Проверено масштабирование C++ workers
* [ ] Проверено масштабирование Python
* [ ] Все linters/formatters проходят
