#include <iostream>
#include <string>
#include <algorithm>

#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>

#include "process.pb.h"

constexpr const char* RABBITMQ_HOST = "rabbitmq";
constexpr int RABBITMQ_PORT = 5672;
constexpr const char* RABBITMQ_USER = "guest";
constexpr const char* RABBITMQ_PASSWORD = "guest";
constexpr const char* RABBITMQ_VHOST = "/";
constexpr const char* REQUEST_QUEUE = "CPP_Request";
constexpr const char* RESPONSE_QUEUE = "CPP_Response";

constexpr int CHANNEL = 1;

bool isPalindrome(const std::string& text){
    std::string normalized;
    for (unsigned char c : text){
        if (std::isalnum(c)){
            normalized += static_cast<char>(std::tolower(c));
        }
    }
    std::string reversed = normalized;
    std::reverse(reversed.begin(), reversed.end());
    return normalized == reversed;
}

amqp_connection_state_t connectRabbitMQ(){
    auto connection = amqp_new_connection();
    if (!connection){
        throw std::runtime_error("amqp_new_connection failed");
    }
    auto* socket = amqp_tcp_socket_new(connection);
    if (!socket){
        amqp_destroy_connection(connection);
        throw std::runtime_error("amqp_tcp_socket_new failed");
    }

    int status = amqp_socket_open(socket, RABBITMQ_HOST, RABBITMQ_PORT);
    if (status != AMQP_STATUS_OK){
        amqp_destroy_connection(connection);
        throw std::runtime_error("amqp_socket_open failed");
    }

    amqp_rpc_reply_t login = amqp_login(
        connection,
        RABBITMQ_VHOST,
        0,
        131072,
        0,
        AMQP_SASL_METHOD_PLAIN,
        RABBITMQ_USER,
        RABBITMQ_PASSWORD
    );
    if (login.reply_type != AMQP_RESPONSE_NORMAL){
        std::cerr << "RabbitMQ login failed." << std::endl;
        std::cerr << "reply_type = " << login.reply_type << std::endl;
        std::cerr << "reply_id = " << login.reply.id << std::endl;
        amqp_connection_close(connection, AMQP_REPLY_SUCCESS);
        amqp_destroy_connection(connection);
        throw std::runtime_error("RabbitMQ login failed");
    }

    amqp_channel_open(connection, CHANNEL);
    amqp_rpc_reply_t channel_reply =amqp_get_rpc_reply(connection);
    if (channel_reply.reply_type != AMQP_RESPONSE_NORMAL){
        amqp_connection_close(connection, AMQP_REPLY_SUCCESS);
        amqp_destroy_connection(connection);
        throw std::runtime_error("RabbitMQ channel open failed");
    }

    return connection;
}

void declareQueues(amqp_connection_state_t connection){
    amqp_queue_declare_ok_t* request_queue = amqp_queue_declare(
            connection,
            CHANNEL,
            amqp_cstring_bytes("CPP_Request"),
            0, // passive
            1, // durable
            0, // exclusive
            0, // auto-delete
            amqp_empty_table
        );
    amqp_rpc_reply_t reply = amqp_get_rpc_reply(connection);
    if (reply.reply_type != AMQP_RESPONSE_NORMAL){
        std::cerr << "Failed to declare CPP_Request" << std::endl;
        throw std::runtime_error("Cannot declare CPP_Request");
    }

    // CPP_Response
    amqp_queue_declare_ok_t* response_queue = amqp_queue_declare(
            connection,
            CHANNEL,
            amqp_cstring_bytes("CPP_Response"),
            0, // passive
            1, // durable
            0, // exclusive
            0, // auto-delete
            amqp_empty_table
        );
    reply = amqp_get_rpc_reply(connection);

    if (reply.reply_type != AMQP_RESPONSE_NORMAL){
        std::cerr << "Failed to declare CPP_Response"<< std::endl;
        throw std::runtime_error("Cannot declare CPP_Response");
    }

}

void startConsumer(amqp_connection_state_t connection){
    amqp_basic_consume(
        connection,
        CHANNEL,
        amqp_cstring_bytes(
            REQUEST_QUEUE
        ),
        amqp_empty_bytes,
        0, // no_local
        0, // no_ack
        0, // exclusive
        amqp_empty_table
    );
}

void sendResponse(amqp_connection_state_t connection,const Response& response){
    std::string data;
    if (!response.SerializeToString(&data)){
        throw std::runtime_error("Failed to serialize Response");
    }

    amqp_bytes_t body;
    body.len = data.size();
    body.bytes = const_cast<char*>(data.data());

    int status = amqp_basic_publish(
        connection,
        CHANNEL,
        amqp_empty_bytes,
        amqp_cstring_bytes(
            RESPONSE_QUEUE
        ),
        0, // mandatory
        0, // immediate
        nullptr,
        body
    );
    if (status != AMQP_STATUS_OK){
        throw std::runtime_error("Failed to publish Response");
    }

}

void processRequest(amqp_connection_state_t connection, const amqp_envelope_t& envelope){
    Request request;

    bool parsed = request.ParseFromArray(envelope.message.body.bytes,static_cast<int>(envelope.message.body.len));

    if (!parsed){
        std::cerr << "Failed to parse Request protobuf" << std::endl;
        return;
    }
    Response response;

    response.set_request_id(request.request_id());
    response.set_text(request.text());
    response.set_lenght(request.lenght());
    response.set_count_words(request.count_words());

    for (const auto& provider : request.provide()){
        response.add_provide(provider);
    }
    
    response.add_provide("C++");
    response.set_is_palindrome(isPalindrome(request.text()));

    sendResponse(connection,response);

}

int main(){
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    try
    {
        amqp_connection_state_t connection = connectRabbitMQ();
        declareQueues(connection);
        startConsumer(connection);

        while (true){
            amqp_envelope_t envelope;
            amqp_rpc_reply_t result = amqp_consume_message(connection, &envelope, nullptr, 0);

            if (result.reply_type !=AMQP_RESPONSE_NORMAL){
                std::cerr << "RabbitMQ consumer error" << std::endl;
                break;
            }

            try{
                processRequest(connection, envelope);
            }
            catch (const std::exception& e){
                std::cerr << "Processing error: " << e.what() << std::endl;
            }

            amqp_destroy_envelope(&envelope);
        }

        amqp_channel_close(connection, CHANNEL, AMQP_REPLY_SUCCESS);
        amqp_connection_close(connection, AMQP_REPLY_SUCCESS);
        amqp_destroy_connection(connection);
    }
    catch (const std::exception& e){
        std::cerr << "Fatal error: "<< e.what()<< std::endl;
        google::protobuf::ShutdownProtobufLibrary();
        return 1;
    }

    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}