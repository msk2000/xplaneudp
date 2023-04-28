/*
	Adapter from C and C++ codes from:
    https://www.binarytides.com/programming-udp-sockets-c-linux/
    https://www.geeksforgeeks.org/udp-server-client-implementation-c/
*/
#include<iostream>// Added by me
#include<stdio.h>	//printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h> // Added by me
#include<cstring> //Added by me
#include<vector> // Added for cout test
#include<iomanip> // for setfill and stuff
#include<signal.h> // FOR EVENT HANDLING

#define BUFFER 1024	//Max length of buffer
#define PORT 49003	//The port on which to listen for incoming data

static volatile bool keep_continue = true;


int main()
{
	signal(SIGINT, [](int) {keep_continue = false;});

    struct sockaddr_in server_address, client_address;
	
	int s_id, i, slen = sizeof(client_address) , recv_len;
    unsigned char buf[BUFFER];
	
	//create a UDP socket
	if ((s_id=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		perror("Couldn't create socket");
        exit(EXIT_FAILURE);
	}
	
	// zero out the structure
	memset(&server_address, 0, sizeof(server_address));
	
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	server_address.sin_addr.s_addr = INADDR_ANY;
	
	//bind socket to port
	if( bind(s_id , (const struct sockaddr *)&server_address, sizeof(server_address) ) < 0)
	{
		perror("Couldn't bind!!");
        exit(EXIT_FAILURE);
	}
	
	//keep listening for data
	while(keep_continue)
	{
        socklen_t slen;
		
		
		//try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(s_id,(char *)buf, BUFFER, 0, (struct sockaddr *) &client_address, &slen)) == -1)
		{
		    perror("Couldn't receive anything");
            exit(EXIT_FAILURE);
		}
		// Because buf is 1024 with mostly nonsense after what it gets from XP. Making buf2
		unsigned char buf2[recv_len];

		for (size_t i=0; i<recv_len; i++)
		{
			buf2[i] = buf[i];
			printf("%02X ",buf[i]);//debug
			//std::cout<<buf2[i]<<"\t"<<std::endl;
		}
		
		std::cout<<"\n";
		std::cout <<"Message length(bytes): "<< recv_len <<std::endl;//debug

        // Trying to extract and organise data
        float floats[8]; // for the 8 we will get from XP
        unsigned char buf3[36];

		
			for (size_t i=5; i<recv_len; i+=36)
				{
					std::cout << "Index:" <<"\t"<< (unsigned int)buf2[i]<<std::endl;
					std::cout<< "Values:"<<std::endl;					
					for (size_t j=0; j<32; j++)
					{
						buf3[j] = buf2[j+i+4];
                        //printf("%02X ",buf3[j]);

                        for (size_t k = 0; k < 8; k++)
                         {
                                unsigned char* bytes = buf3 + k * 4;
                                floats[k] = *reinterpret_cast<float*>(bytes);
                                
                            }
                        

						//printf("%02X ",buf2[j+i+4]);
						//std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(buf2[j+i+4]);
					}
                    /* for (int f : floats)
                   {std::cout << std::fixed << std::setprecision(2) << floats[f]<<"\t";} */
				   for (size_t l =0; l<8;l++)
				   {
					std::cout << std::fixed << std::setprecision(2) << floats[l]<<"\t";
				   }

					std::cout<<"\n";
				}
				
		
  			std::cout <<"\n";
			
			std::cout <<"Next set: \n";
			
		
		
	}
        
	close(s_id);
    std::cout << "closing socket. END END";
	return 0;
}