#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <arpa/inet.h>
#include <csignal>
#include <sys/types.h>
#include <iomanip>
#include <sstream>
#include <string>


#define PORT        50004
#define BUF_SIZE    90
#define CLIP_NAME   "mario"


using namespace std;
static int cnt_req = 0;
const char* clip_name = CLIP_NAME;
char path_file[50];

/*
void handler(int sig){
    cout << "\npackets: " << cnt_req << endl;
    cnt_req = 0;
}
*/

int main() {
//    signal(SIGINT, handler);

    int fd_socket;
    int nbyte;
    struct sockaddr_in addr_host, addr_client;
    socklen_t size_socket;
    size_socket = sizeof(struct sockaddr_in);
    uint8_t buf[BUF_SIZE];
    char buf_recv[8];

    if((fd_socket = socket(PF_INET, SOCK_DGRAM, 0)) == -1){
        perror("error while assigning socket");
        return 1;
    }

    addr_host.sin_family = AF_INET;
    addr_host.sin_port = htons(PORT);
    addr_host.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(&(addr_host.sin_zero), 0, 8);

    if(bind(fd_socket, (struct sockaddr*)&addr_host, sizeof(struct sockaddr)) < 0){
        perror("error while binding socket");
        return 1;
    }

    while(true){
        nbyte  = recvfrom(fd_socket, buf_recv, 7, 0, (struct sockaddr*)&addr_client, &size_socket);

        char* pos = find(buf_recv, buf_recv + nbyte, '_');
        char num_img_c[4], num_part_c[4];
        int idx = buf_recv+nbyte-pos-1;

        memcpy(num_img_c, buf_recv, pos-buf_recv);
        memcpy(num_part_c, pos+1, idx);
        num_img_c[pos-buf_recv] = '\0';
        num_part_c[idx] = '\0';

        int num_img = atoi(num_img_c);
        int num_part = atoi(num_part_c);
/*
        cout << "buf_recv" << hex << &buf_recv << endl;
        cout << "pos" << hex << &pos << endl;
        cout << "diff" << dec << pos-buf_recv << endl;
        cout << "nbyte" << dec << nbyte << endl;
        cout << "img" << dec << num_img << endl;
        cout << "part" << dec << num_part << endl;

        if(nbyte < 0){
            perror("ee");
            return 1;
        }

        stringstream ssbuf;
        ssbuf << "./static/conv_mario/mario" << setw(2) << setfill('0') << dec << num_img << '_' << num_part;
        const string& path_file = ssbuf.str();
        cout << sbuf << endl;
*/
        sprintf(path_file, "./static/conv_%s/%s%02d_%d", clip_name, clip_name, num_img, num_part);
//        cout << path_file << endl;

        FILE *fp;
        fp = fopen(path_file, "r");
        fread(buf, sizeof(uint8_t), 90, fp);
        fclose(fp);
        sendto(fd_socket, buf, 90, 0, (struct sockaddr*)&addr_client, size_socket);
/*
        for(const auto& e: buf){
            cout << setw(2) << hex << int(e) << flush;
        }
        cnt_req++;
*/
    }
}
