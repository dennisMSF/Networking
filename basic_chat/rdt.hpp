#ifndef AHORCADO_RDT_HPP
#define AHORCADO_RDT_HPP


#include "udp.hpp"

string ack_ok = "OK";
string ack_error = "ERROR";
string ack_end = "END_ACK_BULK";

string get_hash(string p)
{
    int hash = 0;
    for (int i = 0; i < p.size(); i++)
    {
        hash += int(p[i]);
    }
    hash = hash % 100;
    string rsp;
    rsp += ('a' + hash / 10);
    rsp += ('a' + hash % 10);
    return rsp;
}

bool check_hash(string p, string &msg)
{
    msg = "";
    for (int i = 0; i < p.size() - 2; i++)
    {
        msg += p[i];
    }
    string hash;
    hash += p[p.size() - 2];
    hash += p[p.size() - 1];
    return get_hash(msg) == hash;
}

int get_hash_file_key(string name, int total_slaves)
{
    int res = 0;
    for (int i = 0; i < name.size(); i++)
    {
        res += (int)(name[i]);
    }
    return res % total_slaves;
}


class rdt_server
{
public:
    udp_server serv;
    vector<int> sequence;
    int num_clients = 0;

    bool request_client()
    {
        return num_clients != serv.clients.size();
    }

    int accept_client()
    {
        sequence.push_back(0);
        return num_clients++;
    }

    rdt_server(int port)
    {
        struct sockaddr_in servaddr;
        if ((serv.sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }

        memset(&servaddr, 0, sizeof(servaddr));

        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = INADDR_ANY;
        servaddr.sin_port = htons(port);

        if (bind(serv.sockfd, (const struct sockaddr *)&servaddr,
                 sizeof(servaddr)) < 0)
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        serv.listen();
    }

    void write_rdt(int client_id, string _message)
    {
        string message = _message + get_hash(_message);
        serv.write_udp(client_id, message);
        string ack = serv.read_udp(client_id);
        if (ack != ack_ok)
        {
            write_rdt(client_id, _message);
        }
    }

    void write_rdt_num(int client_id, int num) {
        string number = to_string(num);
        return write_rdt(client_id, number);
    }

    int read_rdt_num(int client_id) {
        string number = read_rdt(client_id);
        return stoi(number);
    }

    string read_rdt(int client_id)
    {
        string message = serv.read_udp(client_id);
        string msg;
        if (check_hash(message, msg))
        {
            serv.write_udp(client_id, ack_ok);
            return msg;
        }
        else
        {
            serv.write_udp(client_id, ack_error);
            return read_rdt(client_id);
        }
    }
};

class rdt_client
{
public:
    udp_client client;
    int sequence;

    rdt_client(string direction, int port)
    {
        struct hostent *host;

        host = (struct hostent *)gethostbyname(direction.c_str());

        if ((client.sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }

        memset(&client.servaddr, 0, sizeof(client.servaddr));

        client.servaddr.sin_family = AF_INET;
        client.servaddr.sin_port = htons(port);
        client.servaddr.sin_addr = *((struct in_addr *)host->h_addr);
        client.register_client();
    }

    void write_rdt(string _message)
    {
        string message = _message + get_hash(_message);
        client.write_udp(message);
        string ack = client.read_udp();
        if (ack != ack_ok)
        {
            write_rdt(_message);
        }
    }

    string read_rdt()
    {
        string message = client.read_udp();
        string msg;
        if (check_hash(message, msg))
        {
            client.write_udp(ack_ok);
            return msg;
        }
        else
        {
            client.write_udp(ack_error);
            return read_rdt();
        }
    }

    void write_rdt_num(int num) {
        string number = to_string(num);
        return write_rdt(number);
    }

    int read_rdt_num() {
        string number = read_rdt();
        return stoi(number);
    }
};

#endif //AHORCADO_RDT_HPP
