#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>


namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>
using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>

std::vector<std::string> StringProcessing(std::string& request) //обработчик строк
{
    std::string sub_request; //строка sub_request
    std::remove_copy(request.begin(), request.end(), std::back_inserter(sub_request), ' '); //удаляет пробелы и помещает строку в sub_request

    std::vector <std::string> string; //создание вектора из строк
    std::string act;
    std::string::size_type s_pos = request.find("-");
    //string::size_type  тип, который достаточно велик, чтобы представлять размер строки, независимо от того, насколько велика эта строка.
    // request.find("-") ищет знак "-" в введённой строке
    size_t pos = 0; 
    size_t act_pos;


    if (s_pos != std::string::npos && sub_request[s_pos] == '-') //если spos не равен пустой строке, а sub_request = "-"
        act = "-";
    else
        act = "+";
  

    string.push_back(act); //добавление знака в конец вектора
  
    while ((act_pos = sub_request.find(act, pos)) != std::string::npos) 
    {
        string.push_back(sub_request.substr(pos, act_pos - pos));
        pos = act_pos + 1;
    }
    string.push_back(sub_request.substr(pos));
    //Считывает число до запятой и функцией push_back возвращает его в конец вектора
    //Далее число после знака так же возвращает пока строка не закончится

    return string; //возвращает строку со знаком в начале
}

// Performs an HTTP GET and prints the response
int main(int argc, char** argv) {
    setlocale(0, "");

    std::string request;
    std::getline(std::cin, request); //прочитай всю строку и положи в request
    std::vector <std::string> sub_request = StringProcessing(request);

    try {
        //http://157.230.27.215/calc/diff/25/12 >> 25 - 12 = 13
        //http://157.230.27.215/calc/sum/35/15 >> 35 + 15 = 50
        std::string a = sub_request[1];
        std::string b = sub_request[2];
        std::string const host = "157.230.27.215";
        std::string const port = "80";
        std::stringstream url;
        std::string action;

        if (sub_request[0] == "-")
            action = "/calc/diff/";
        else
            action = "/calc/sum/";

        url << action << a << "/" << b;

        std::string const target = url.str();//
        int const version = 11; 
        std::cout << target;
        // The io_context is required for all I/O | Input Output conext
        net::io_context ioc;

        // These objects perform our I/O
        //Tcp resolver - утилита для резолва имён.Ищет соответствие hostname и ip адреса
        tcp::resolver resolver(ioc);
        auto const results = resolver.resolve(host, port);

        beast::tcp_stream stream(ioc); //поток
        stream.connect(results); // по порту и хосту соединяемся с данным сервером

        // Set up an HTTP GET request message
        //формируется request. Задаём метод, указываем ресурс к которому подсоединяемся, версию
        http::request<http::string_body> req{ http::verb::get, target, version }; 
        
        //Формируются headers
        req.set(http::field::host, host);
        req.set(http::field::user_agent, "gg/test");

        // Send the HTTP request to the remote host
        http::write(stream, req); //переходит в режим передачи сообщения на удалённый хост. Послали запрос

        // This buffer is used for reading and must be persisted
        beast::flat_buffer buffer; //определяем переменную чтобы получить ответ

        // Declare a container to hold the response
        http::response<http::dynamic_body> res; //ответ

        // Receive the HTTP response
        http::read(stream, buffer, res); //из стрима читаем через буфер наш ответ

        // Write the message to standard out
        std::cout << res << std::endl; //выводим ответ 

        // Gracefully close the socket
        beast::error_code ec;
        stream.socket().shutdown(tcp::socket::shutdown_both, ec); //закрытие сокетов

        // not_connected happens sometimes
        // so don't bother reporting it.
        //
        if (ec && ec != beast::errc::not_connected)
            throw beast::system_error{ ec };

        // If we get here then the connection is closed gracefully
    }
    catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}