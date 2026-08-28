from concurrent import futures
import grpc
import process_pb2
import process_pb2_grpc

class ProcessService(process_pb2_grpc.CSharpToPythonServicer):
    def GetCount(self, request, context):
        text: str = request.text
        response = process_pb2.Response()
        response.text = text
        response.lenght = request.lenght
        response.count_words = text.count(" ") +1
        response.provide.extend(request.provide)
        response.provide.append("Python")
        return response
def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    process_pb2_grpc.add_CSharpToPythonServicer_to_server(ProcessService(),server)
    server.add_insecure_port("[::]:50051")
    print("start at :50051")
    server.start()
    server.wait_for_termination()
    