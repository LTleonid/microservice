FROM python:3.14-slim-trixie
COPY --from=ghcr.io/astral-sh/uv:latest /uv /uvx /bin/

COPY . /app
ENV UV_NO_DEV=1
WORKDIR /app
RUN uv sync --locked

CMD ["uv", "run", "main.py"]