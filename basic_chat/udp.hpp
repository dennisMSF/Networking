#ifndef AHORCADO_UDP_HPP
#define AHORCADO_UDP_HPP

#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>

using namespace std;
////////////////////////////////////////////////////////////////////////////////////////
class udp_server
{
public:
    vector<sockaddr> clients;
    vector<deque<string>> wait_messages;
    int sockfd;
    int MAXLINE = 2048;

    void listen()
    {
        thread(call_listener, this).detach();
    }

    void write_udp(int client_id, string message)
    {
        char buffer[message.size()];
        for (int i = 0; i < message.size(); ++i)
        {
            buffer[i] = message[i];
        }
        sockaddr cliaddr = clients[client_id];
        socklen_t len = sizeof(cliaddr);
        sendto(sockfd, buffer, message.size(), MSG_CONFIRM, &cliaddr, len);
    }

    void register_client(sockaddr cliaddr)
    {
        int id = clients.size();
        clients.push_back(cliaddr);
        deque<string> novo;
        wait_messages.push_back(novo);
        char char_id = 'a' + id;
        string rep;
        rep += char_id;
        write_udp(id, rep);
    }

    static void call_listener(udp_server *udp_serv)
    {
        udp_serv->listener();
    }

    void listener()
    {
        while (1)
        {
            char buffer[MAXLINE];
            sockaddr cliaddr;
            socklen_t len = sizeof(cliaddr);
            int n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                             MSG_WAITALL, (struct sockaddr *)&cliaddr,
                             &len);
            buffer[n] = '\0';
            if (buffer[0] == 'R')
            {
                register_client(cliaddr);
            }
            else
            {
                string msg;
                for (int i = 1; i < n; i++)
                {
                    msg += buffer[i];
                }
                int client_id = buffer[0] - 'a';
                wait_messages[client_id].push_back(msg);
            }
        }
    }

    string read_udp(int client_id)
    {
        while (wait_messages[client_id].size() == 0)
        {
        }
        string msg = wait_messages[client_id][0];
        wait_messages[client_id].pop_front();
        return msg;
    }
};
////////////////////////////////////////////////////////////////////////////////////////
class udp_client
{
public:
    struct sockaddr_in servaddr;
    int sockfd;
    int client_id = -1;
    int MAXLINE = 2048;
    int sequence;

    void register_client()
    {
        char buffer[1];
        buffer[0] = 'R';
        socklen_t len = sizeof(servaddr);
        sendto(sockfd,
               buffer,
               1,
               MSG_CONFIRM,
               (const struct sockaddr *)&servaddr,
               len);
        string msg = read_udp();
        client_id = msg[0] - 'a';
    }

    void write_udp(string message)
    {
        string id_str;
        id_str += 'a' + client_id;
        message = id_str + message;
        char buffer[message.size()];
        for (int i = 0; i < message.size(); ++i)
        {
            buffer[i] = message[i];
        }
        socklen_t len = sizeof(servaddr);
        sendto(sockfd, buffer, message.size(), MSG_CONFIRM, (const struct sockaddr *)&servaddr, len);
    }

    string read_udp()
    {
        char buffer[MAXLINE];
        socklen_t len = sizeof(servaddr);
        memset(&servaddr, 0, sizeof(servaddr));
        int n = recvfrom(sockfd,
                         (char *)buffer,
                         MAXLINE,
                         MSG_WAITALL,
                         (struct sockaddr *)&servaddr,
                         &len);
        string message;
        for (int i = 0; i < n; i++)
        {
            message += buffer[i];
        }
        return message;
    }
};

#endif //AHORCADO_UDP_HPP
