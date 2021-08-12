#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <thread>
#include <vector>
#include <utility>
#include <iostream>
#include <string>

#define Tamanho 1000
using namespace std;
char square[10] = {'o','1','2','3','4','5','6','7','8','9'};

int checkwin();
void board();
void game(){
  int player = 1,i,choice;

    char mark;
    do
    {
        board();
        player=(player%2)?1:2;

        cout << "Player " << player << ", enter a number:  ";
        cin >> choice;

        mark=(player == 1) ? 'X' : 'O';

        if (choice == 1 && square[1] == '1')

            square[1] = mark;
        else if (choice == 2 && square[2] == '2')

            square[2] = mark;
        else if (choice == 3 && square[3] == '3')

            square[3] = mark;
        else if (choice == 4 && square[4] == '4')

            square[4] = mark;
        else if (choice == 5 && square[5] == '5')

            square[5] = mark;
        else if (choice == 6 && square[6] == '6')

            square[6] = mark;
        else if (choice == 7 && square[7] == '7')

            square[7] = mark;
        else if (choice == 8 && square[8] == '8')

            square[8] = mark;
        else if (choice == 9 && square[9] == '9')

            square[9] = mark;
        else
        {
            cout<<"Invalid move ";

            player--;
            cin.ignore();
            cin.get();
        }
        i=checkwin();

        player++;
    }while(i==-1);
    board();
    if(i==1)

        cout<<"==>\aPlayer "<<--player<<" win ";
    else
        cout<<"==>\aGame draw";

    cin.ignore();
    cin.get();
    return 0;
}
class package{
public: 
    string n_sec, n_flujo, sec_flujo, last, size, data, checksum;
        string res;

       int n_sec_, n_flujo_, sec_flujo_, last_, size_, checksum_;
       string acc;

        paquete(int _n_sec, int _n_flujo, int _sec_flujo, int _last,string _acc, int _size, string _data, int _checksum)
        {       
                this->n_sec_ = _n_sec;
                this->n_flujo_ = _n_flujo;
                this->sec_flujo_ = _sec_flujo;
                this->last_ = _last;
                this->acc=_acc;
                this->size_ = _size;
                this->checksum_ = _checksum;

                this->n_sec = complete(_n_sec, 5);
                this->n_flujo = complete(_n_flujo, 3);
                this->sec_flujo = complete(_sec_flujo, 2);
                this->last = to_string(_last);
                this->size = complete(_size, 3);
                this->data = _data;
                this->checksum = complete(_checksum, 2);

                this->res = n_sec + n_flujo + sec_flujo + last + acc + size + data + checksum;
        }

         string complete(int n, int c)
        {
                string _res;
                _res = to_string(n);
                while (_res.size() != c)
                        _res = "0" + _res;
                return _res;
        }
};

class rdt {
public:
    int n_sec = 1;
    int n_flujo = 1;
    int sec_flujos;
    int checksum (string contenido){
        int total = 0;
        for (int i=0; i<contenido.size(); i++){
            if (contenido[i] != '\0'){
                total = total + (int)contenido[i];
            }
        }
        return total % 666;
    }
    pair<vector<string>,vector<int>> leer(int sockfd, sockaddr_in cliaddr){
        bool flag = 1;
        vector<string> deposito;
        vector<int> ack;
        while(flag){
            char buffer[TAM_BUFFER];
            int len, n;
            len = sizeof(cliaddr);
            
            n = recvfrom(sockfd, (char *)buffer, TAM_BUFFER,
                                                    MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                                                    (socklen_t*)&len);
            buffer[n] = '\0';
            string parte = buffer;
            deposito.push_back( parte.substr(15,n-15-2));
            int check = checksum(buffer);
            int acks = 0;
            if(check==stoi(parte.substr(15 + (n-15-2),2))){
                acks = stoi(parte.substr(0,5));
            }
            ack.push_back(acks);
            int termino = stoi(parte.substr(10,1));
            if(termino) flag = 0;
        }
        return make_pair(deposito,ack);
    }
    void mandar(int sockfd,sockaddr_in cliaddr, string mensaje){
        char *buff = &mensaje[0];
        sendto(sockfd,buff,strlen(buff),MSG_CONFIRM,(const struct sockaddr *) &cliaddr,sizeof(cliaddr));
    }
    vector<paquete> hacerPaquete(string contenido){
        int n_pack = contenido.size()/494;
        n_pack = contenido.size() % 494 == 0 ? n_pack : n_pack + 1;
        vector<paquete> envio;
        for(int i = 0; i < n_pack;i++){
            string parte = contenido.substr(494*i,494);
            envio.push_back(paquete(n_sec,n_flujo,i,i==n_pack-1?1:0,"A",parte.size(),parte,checksum(parte)));
            n_sec++;
        }
        n_flujo++;
        return envio;
    }
    string desempaquetar(int sockfd, sockaddr_in cliaddr){S
        string mensaje="";
        pair<vector<string>,vector<int>> m_pair;
        while(true){
            m_pair = leer(sockfd,cliaddr);
            string ack = hacerAck(m_pair.second);
            vector<int> aux = convert(ack);
            mandar(sockfd,cliaddr,ack);
            if(valido(m_pair.second))
                break;
            else{
                pair<vector<string>,vector<int>> m_pair_aux;
                m_pair_aux = leer(sockfd,cliaddr);
                m_pair.first = corregir(m_pair.first,m_pair_aux.first,m_pair_aux.second);
                aux = actualizarAck(aux,m_pair_aux.second);
                ack=hacerAck(aux);
                mandar(sockfd,cliaddr,ack);
            }
            if(valido(aux)) break;
        }
        for(int i =0; i < m_pair.first.size();i++){
            mensaje+=m_pair.first[i];
        }
        return mensaje;        
    }
    string hacerAck(vector<int> a){
        string dumb="";
        for(int i=0; i < a.size();i++){
            int ana=0;
            if(i+1==a[i]){
                ana = a[i];
            }
            dumb+= to_string(ana);
        }
        return dumb;
    }
    
    bool valido(vector<int> ack){
        for(int i=0; i < ack.size();i++){
            if(ack[i]==0)
                return false;
        }
        return true;
    }
    vector<int> actualizarAck(vector<int> ack, vector<int> actualizar){
        for(int i=0; i <actualizar.size();i++){
            vector<int>::iterator a = actualizar.begin()+actualizar[i]-1;
            ack.insert(a,actualizar[i]);
        }
        return ack;
    }
};
  int teclear(){
    if(kbhit()){
      int tecla = getch();
      return tecla;
    }
  }

  int main(void){
    struct sockaddr_in stSockAddr;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    int Res,n,mov;
    char buff[2];
    string aux;
    bool state=1;
    const char* ptr;

 
    if (-1 == SocketFD){
      perror("cannot create socket");
      exit(EXIT_FAILURE);
    }
 
    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
 
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(50008);
    Res = inet_pton(AF_INET, "51.15.220.108", &stSockAddr.sin_addr);
    //Res = inet_pton(AF_INET, "192.168.0.1", &stSockAddr.sin_addr);

    if (0 > Res){
      perror("error: first parameter is not a valid address family");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
	
    else if (0 == Res){
      perror("char string (second parameter does not contain valid ipaddress");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }

    if (connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)) == -1){
      perror("connect failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }

    do{
      mov= teclear();
      mov= teclear();

      aux=to_string(mov);

      n = write(SocketFD,aux.c_str(),aux.length());//envia la tecla presionada

      bzero(buff,2);
      n = read(SocketFD,buff,1); //lee la etiqueta del mensaje recibidos
      if (n < 0) perror("ERROR reading from socket");

      if(buff[0] == 'L'){ // si es indicando que el jugador perdio
        cout<<"Desafortunadamente, perdiste :c "<<endl;
        state=0;
      }

      else{ 
          continue;
      }

    }while(state);

    shutdown(SocketFD, SHUT_RDWR);
    close(SocketFD);

    return 0;
  }
int checkwin()
{
    if (square[1] == square[2] && square[2] == square[3])

        return 1;
    else if (square[4] == square[5] && square[5] == square[6])

        return 1;
    else if (square[7] == square[8] && square[8] == square[9])

        return 1;
    else if (square[1] == square[4] && square[4] == square[7])

        return 1;
    else if (square[2] == square[5] && square[5] == square[8])

        return 1;
    else if (square[3] == square[6] && square[6] == square[9])

        return 1;
    else if (square[1] == square[5] && square[5] == square[9])

        return 1;
    else if (square[3] == square[5] && square[5] == square[7])

        return 1;
    else if (square[1] != '1' && square[2] != '2' && square[3] != '3' 
                    && square[4] != '4' && square[5] != '5' && square[6] != '6' 
                  && square[7] != '7' && square[8] != '8' && square[9] != '9')

        return 0;
    else
        return -1;
}

void board()
{
    system("cls");
    cout << "\n\n\tTic Tac Toe\n\n";

    cout << "Player 1 (X)  -  Player 2 (O)" << endl << endl;
    cout << endl;

    cout << "     |     |     " << endl;
    cout << "  " << square[1] << "  |  " << square[2] << "  |  " << square[3] << endl;

    cout << "_____|_____|_____" << endl;
    cout << "     |     |     " << endl;

    cout << "  " << square[4] << "  |  " << square[5] << "  |  " << square[6] << endl;

    cout << "_____|_____|_____" << endl;
    cout << "     |     |     " << endl;

    cout << "  " << square[7] << "  |  " << square[8] << "  |  " << square[9] << endl;

    cout << "     |     |     " << endl << endl;
}
