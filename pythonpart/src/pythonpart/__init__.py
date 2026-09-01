import asyncio
import uuid
import grpc
import process_pb2
import process_pb2_grpc
from faststream import FastStream
from faststream.rabbit import RabbitBroker

broker = RabbitBroker("amqp://guest:guest@rabbitmq:5672/")
app = FastStream(broker)
pending_requests: dict[str, asyncio.Future] = {}

@broker.subscriber("CPP_Response")
async def cpp_response(data: bytes):
    response = process_pb2.Response()
    response.ParseFromString(data)
    request_id = response.request_id
    future = pending_requests.get(request_id)

    if future is not None and not future.done():
        future.set_result(response)


class ProcessService(process_pb2_grpc.CSharpToPythonServicer):

    async def GetCount(self,request,context):
        #Пред обработка 
        request.request_id = str(uuid.uuid4())
        request.provide.append("Python")
        request.count_words = len(request.text.split())
        request.length = len(request.text)

        loop = asyncio.get_running_loop()
        future = loop.create_future()
        pending_requests[request.request_id] = future #Кидаем задачу в ожидание асинхрона 

        try:
            await broker.publish(request.SerializeToString(), queue="CPP_Request") #Отпрвляем в рмк
            response = await asyncio.wait_for(future,timeout=30) # ждём ответ от плюсов

            return response

        except asyncio.TimeoutError:
            await context.abort(grpc.StatusCode.DEADLINE_EXCEEDED,"Timeout ERR")

        finally:
            pending_requests.pop(request.request_id,None) #Удаляем из задач


async def grpc_server():

    server = grpc.aio.server()
    process_pb2_grpc.add_CSharpToPythonServicer_to_server(ProcessService(),server)
    server.add_insecure_port("[::]:50051")
    await server.start()
    await server.wait_for_termination()


async def serve():

    rabbit_task = asyncio.create_task(app.run())
    grpc_task = asyncio.create_task(grpc_server())
    await asyncio.gather(rabbit_task, grpc_task)
