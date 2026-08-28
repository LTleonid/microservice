FROM python:3.14-slim-trixie

COPY --from=ghcr.io/astral-sh/uv:latest /uv /uvx /bin/

ENV UV_NO_DEV=1

WORKDIR /app

COPY pythonpart/pyproject.toml ./
COPY pythonpart/uv.lock ./
COPY pythonpart/README.md ./

COPY pythonpart/src ./src

RUN uv sync --locked

COPY pythonpart/main.py ./

COPY shared/ ./shared/

RUN uv run python -m grpc_tools.protoc \
    -I=./shared \
    --python_out=./src/ \
    --grpc_python_out=./src/ \
    ./shared/process.proto

CMD ["uv", "run", "main.py"]