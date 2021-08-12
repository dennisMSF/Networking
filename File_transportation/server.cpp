#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <sstream>
#include <thread>  
#include <chrono>
#include <fstream>
#define PORT    60005
#define MAXLINE 1000
using namespace std;
int dt_size = 3;
int msg_id_sz = 5;
int hash_sz = 3;
int tp_msg_sz = 1;
int size_padd = MAXLINE - (3+5+3+1);
string dt_sz;
string msg_id;
string hash_number;
string type_mssg;
string msg_client;
vector <string> type_message = {"0","1","2","3","4","5","6"}; 
void response(struct sockaddr_in, int ,string, int );
void solicitation(struct sockaddr_in , int );
void send_ack(struct sockaddr_in , int ,string , int );
void notification();
bool check_hash(string mensaje, int hash_obtenido){
    int new_hash = 0;
    for (int i = 0;i<mensaje.size();i++){
        new_hash += int(mensaje[i]);
    }
    if(new_hash == hash_obtenido){
        return 1;
    }
    else{
        return 0;
    }
}
string message_id(int mssg_id){
    int size = 5;
    string mss_id = to_string(mssg_id);
    for (int i= mss_id.size() ; i<msg_id_sz ; i++){
        mss_id = "0" + mss_id;
    }
    return mss_id;
}
string data_size(int dt_size, string mensaje){
    string dt_sz = to_string(mensaje.size());
    for (int i= dt_sz.size();i<dt_size;i++){
        dt_sz = "0" + dt_sz;
    }
    return dt_sz;
}
int make_hash(string mensaje){
    int hash = 0;
    for (int i =0 ;i<mensaje.size();i++){
        hash += int(mensaje[i]);
    }
    return hash%666;
}
string padd(int padding, string mensaje){
    string paddng;
    for(int i = mensaje.size(); i<padding;i++){
        paddng += '0';
    } 
    return paddng;
}
string crear_mensaje(string type_of_message, int mss_id, int data_sz, string mensaje, int padding){
    string msg_to_send;
    msg_to_send = msg_to_send + type_of_message + message_id(mss_id) + data_size(data_sz,mensaje) + mensaje + to_string(make_hash(mensaje)) + padd(padding, mensaje);
    return msg_to_send;
}
int desenvolver_mensaje(string mensaje){
    int i=0;
    dt_sz ="";
    msg_id="";
    hash_number="";
    type_mssg  ="";
    msg_client="";
    for (i;i<tp_msg_sz;i++){
        type_mssg += mensaje[i];
    }
    for(i; i<tp_msg_sz+msg_id_sz;i++){
        msg_id += mensaje[i];
    }
    for(i;i<tp_msg_sz+msg_id_sz+dt_size;i++){
        dt_sz += mensaje[i];
    }
    stringstream convert(dt_sz);
    int data_sizes = 0;
    convert >> data_sizes;
    for(i;i<tp_msg_sz+msg_id_sz+dt_size+data_sizes;i++){
        msg_client+= mensaje[i];
    }
    for(i;i<tp_msg_sz+msg_id_sz+dt_size+data_sizes+hash_sz;i++){
        hash_number += mensaje[i];
    }
    stringstream geek(type_mssg); 
    int tp_ms_aux=0 ;
    geek>> tp_ms_aux;
    cout<<tp_ms_aux<<endl;
    stringstream manin(hash_number);
    int hash_aux = 0;
    manin >> hash_aux;
    if(check_hash(msg_client,hash_aux)){
        cout<<"mensaje recibido sin errores"<<endl;
        switch (tp_ms_aux){
        case 1: 
            cout<<"tipo de mensaje: "<<"request and response"<<endl;
            cout<<"id del mensaje: "<<msg_id<<endl;
            cout<<"tamaño de la data: "<<dt_sz<<endl;
            cout<<"mensaje: "<<msg_client<<endl;
            cout<<"hash: "<<hash_number<<endl;
            return 1;
        case 2:
            cout<<"tipo de mensaje: "<<"request and response"<<endl;
            cout<<"id del mensaje: "<<msg_id<<endl;
            cout<<"tamaño de la data: "<<dt_sz<<endl;
            cout<<"mensaje: "<<msg_client<<endl;
            cout<<"hash: "<<hash_number<<endl;
            return 2;
        case 3:
            cout<<"tipo de mensaje: "<<"solicitation and replay"<<endl;
            cout<<"id del mensaje: "<<msg_id<<endl;
            cout<<"tamaño de la data: "<<dt_sz<<endl;
            cout<<"mensaje: "<<msg_client<<endl;
            cout<<"hash: "<<hash_number<<endl;
            return 3;
        case 4:
            cout<<"tipo de mensaje: "<<"solicitation and replay"<<endl;
            cout<<"id del mensaje: "<<msg_id<<endl;
            cout<<"tamaño de la data: "<<dt_sz<<endl;
            cout<<"mensaje: "<<msg_client<<endl;
            cout<<"hash: "<<hash_number<<endl;

            return 4;
        case 5:
            cout<<"tipo de mensaje: "<<"send and forget"<<endl;
            cout<<"id del mensaje: "<<msg_id<<endl;
            cout<<"tamaño de la data: "<<dt_sz<<endl;
            cout<<"mensaje: "<<msg_client<<endl;
            cout<<"hash: "<<hash_number<<endl;
            return 5;
        case 6:
            cout<<"tipo de mensaje: "<<"notification"<<endl;
            cout<<"id del mensaje: "<<msg_id<<endl;
            cout<<"tamaño de la data: "<<dt_sz<<endl;
            cout<<"mensaje: "<<msg_client<<endl;
            cout<<"hash: "<<hash_number<<endl;
            return 6;
        }
    }
    return 0; 
} 
void solicitation(struct sockaddr_in cliaddr, int sockcliente){
    string mensaje = "ingrese el nombre del archivo a subir: ";
    int mss_id = 45;
    string mensaje_enviar = crear_mensaje(type_message[3],45,dt_size,mensaje,size_padd);
    sendto(sockcliente, mensaje_enviar.c_str(), mensaje_enviar.size(),MSG_CONFIRM, (const struct sockaddr*) &cliaddr, sizeof(cliaddr));
}
void response(struct sockaddr_in cliaddr, int sockcliente,string msn_rcb, int hash_obtenido){
    if(check_hash(msn_rcb,hash_obtenido)){
        string mensaje = "mensaje recibido";
        int id = 78;
        string mensaje_env=crear_mensaje(type_message[2],id,dt_size,mensaje,size_padd);
        sendto(sockcliente, mensaje_env.c_str(), mensaje_env.size(),MSG_CONFIRM, (const struct sockaddr*) &cliaddr, sizeof(cliaddr));

    }
    else {
        string mensaje = "mensaje no recibido";
        int id = 79;
        string mensaje_env=crear_mensaje(type_message[2],id,dt_size,mensaje,size_padd);
        sendto(sockcliente, mensaje_env.c_str(), mensaje_env.size(),MSG_CONFIRM, (const struct sockaddr*) &cliaddr, sizeof(cliaddr));
    }
}
void send_ack(struct sockaddr_in cliaddr, int sockcliente,string msn_rcb, int hash_obtenido){
    if(check_hash(msn_rcb,hash_obtenido)){
        string mensaje = "1";
        int id = 78;
        string mensaje_env=crear_mensaje(type_message[2],id,dt_size,mensaje,size_padd);
        sendto(sockcliente, mensaje_env.c_str(), mensaje_env.size(),MSG_CONFIRM, (const struct sockaddr*) &cliaddr, sizeof(cliaddr));

    }
    else if(!check_hash(msn_rcb,hash_obtenido)) {
        string mensaje = "2";
        int id = 79;
        string mensaje_env=crear_mensaje(type_message[2],id,dt_size,mensaje,size_padd);
        sendto(sockcliente, mensaje_env.c_str(), mensaje_env.size(),MSG_CONFIRM, (const struct sockaddr*) &cliaddr, sizeof(cliaddr));
    }
    else {
        string mensaje = "3";
        int id = 79;
        string mensaje_env=crear_mensaje(type_message[2],id,dt_size,mensaje,size_padd);
        sendto(sockcliente, mensaje_env.c_str(), mensaje_env.size(),MSG_CONFIRM, (const struct sockaddr*) &cliaddr, sizeof(cliaddr));
    }
}
void notification(struct sockaddr_in cliaddr, int sockcliente){
    string mensaje = "Notificacion enviada ";
    int mss_id = 77;
    string mensaje_enviar = crear_mensaje(type_message[3],45,dt_size,mensaje,size_padd);
    sendto(sockcliente, mensaje_enviar.c_str(), mensaje_enviar.size(),MSG_CONFIRM, (const struct sockaddr*) &cliaddr, sizeof(cliaddr));
}
bool timeout(string mensaje){
    int intentos = 1;
    while (intentos <4){
        if(mensaje == ""){
            cout<<intentos<<" intento de timeout"<<endl;
            std::this_thread::sleep_for (std::chrono::seconds(1));
            intentos --;
        }
    }
    return 1;
}
void clearBuf(char* b) 
{ 
    int i; 
    for (i = 0; i < MAXLINE; i++) 
        b[i] = '\0'; 
} 
int recvFile(char* buf, int s) 
{ 
    int i; 
    char ch; 
    for (i = 0; i < s; i++) { 
        ch = buf[i]; 
        if (ch == EOF) 
            return 1; 
        else
            printf("%c", ch); 
    } 
    return 0; 
} 
void recibir_archivo(struct sockaddr_in cliaddr, int sockcliente,string name){
    fstream my_file;
	my_file.open(name, ios::in);
    string aux;
    char buffer[MAXLINE];
    char ch;
    socklen_t len = sizeof(cliaddr);
	while (1) {
        cout<<"preparanadose para recibir ..."<<endl;
        recvfrom(sockcliente, (char *)buffer, MAXLINE,MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        aux = buffer;
        my_file << buffer;
        clearBuf(buffer);
        aux= "";
        if(buffer[0]){
            my_file.close();
            break;
        }
	}
}
void crear_socket_cliente(){
    int sockfd;
    char buffer[MAXLINE];
    string mensaje_rcb;
    struct sockaddr_in servaddr, cliaddr;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    socklen_t len = sizeof(cliaddr);
    cout<<"Server listening ..."<<endl;
    bind(sockfd, (const struct sockaddr *)&servaddr,sizeof(servaddr));
    int n;
    int nBytes;
    FILE* fp;
    //solicitation(cliaddr,sockfd);
    while(1){
        n = recvfrom(sockfd, (char *)buffer, MAXLINE,MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        mensaje_rcb = buffer;
        if (mensaje_rcb == ""){
            timeout(mensaje_rcb);
            continue;
        }
        desenvolver_mensaje(mensaje_rcb);
        solicitation(cliaddr,sockfd);
        while(1){
            clearBuf(buffer); 
            nBytes = recvfrom(sockfd, buffer, MAXLINE, MSG_WAITALL, (struct sockaddr*)&cliaddr,&len); 
  
            // process 
            if (recvFile(buffer, MAXLINE)) { 
                break; 
            } 
        }
    }
}
int main(){
    crear_socket_cliente();
}