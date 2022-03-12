#include <iostream>
#include <queue>
#include <condition_variable>
#include <thread>
#include <map>
#include <fstream> 

constexpr unsigned int MAX_BUFFER = 128;

void producer();
void ProcessRPC();
void server();

// std::string readIncoming(int sockfd)
// {
//     std::string readBuffer (MAX_BUFFER, 0);
//     if (read(sockfd, &readBuffer[0], MAX_BUFFER-1) < 0)
//     {
//         std::cerr << "read from socket failed" << std::endl;
//         // return 5;
//     }
    
//     return readBuffer;
// }

// Data contract between producer and consumer.
struct Item
{
	int age;
	std::string firstName;
	std::string lastName;
	std::string password;
	std::string message;
	std::string token;
};

char* writeOutgoingStream(Item item)
{
	char *writeBuffer=(char*)malloc(sizeof(item));
	memcpy(writeBuffer,(const unsigned char*)&item,sizeof(item));

	
    return writeBuffer;
}

/**
 * @brief A condition variable is a named queue on which processes 
 * can wait for some condition to become true.
 * 
 */
std::condition_variable cond; 
std::queue<Item> queue;
std::mutex mut;

enum rpcStatus { notReady, ready, processing, processed };
// Create a map of the RPC tokens (strings, int) pairs
std::map<std::string, rpcStatus> rpcTable 
{ 
	{"connect", ready}, 
	{"disconnect", ready}, 
	{"status", ready}, 
	{"markBoard", ready}, 
	{"setTime", ready}, 
	{"setMaxNum", ready} 
};

int main()
{
    std::thread t1 (producer);
    std::thread t2 (server);

    t1.join();
    t2.join();

    return 0;
}

enum serverStatus { systemError, connected, disconnected };

void server()
{
	int rpcCount = 0;
	serverStatus currentServerState;
	// std::cout << "Consumer ... " << std::endl;
	while(true)
	{
		// Process the incoming RPC.
		while (currentServerState == connected)
		{
			ProcessRPC();
		}	
	}
}

void ProcessRPC()
{
	rpcStatus currentRPCState;

	while(true)
	{
		std::unique_lock<std::mutex> lck{mut};
		std::cout << "Consumer ... loop ... START" << std::endl;
		// cond.wait(lck);
		cond.wait(lck, []{ return !queue.empty();});
          	auto item = queue.front();
          	queue.pop();
		// std::cout << "Age = " << item.age << " First Name = " << item.firstName << " Last Name = " << item.lastName << std::endl;
		std::cout << "Queue Size = " << queue.size() << std::endl;
		// std::cout << "Consumer ... loop ... END" << std::endl;
		// std::cout << "Message: " << item.message << std::endl;
		
	// Lookup the incoming token.
		// std::cout << "Lookup Token from RPC Table. " << std::endl;
		 
		for (auto iterator = rpcTable.find(item.token); iterator != rpcTable.end(); iterator++)
		{
			currentRPCState = iterator->second;
			std::cout << "Match found " << iterator->first << " <" << item.token << "> token key in RPC Table. " << std::endl;
			std::cout << "Token key is: " << iterator->first << "\t Value is: " << currentRPCState << std::endl;

			
			/**
			 * @brief Check the state of the server.
			 * Server should be connected first, then check if the incoming rpc was processed correctly.
			 * 
			 */
			if ( currentRPCState == ready || currentRPCState == processed)
			{
				currentRPCState = processed;
				rpcTable.insert_or_assign(item.token, currentRPCState);
				// std::cout << currentServerState;
				break;
			} else if ( currentRPCState == notReady)
			{
				// currentServerState = systemError;
				currentRPCState = notReady;
				rpcTable.insert_or_assign(item.token, currentRPCState);
				// std::cout << currentServerState;
				break;
			} else if ( currentRPCState == processing)
			{
				// currentServerState = systemError;
				currentRPCState = notReady;
				rpcTable.insert_or_assign(item.token, currentRPCState);
				// std::cout << currentServerState;
				break;
			} 
		} 
		// std::cout << "token not found" << std::endl;
		// Set status to connected, then check authentication.

		lck.unlock();
	}
}

void producer()
{
	std::cout << "Producer ... " << std::endl;
	while(true)
	{
        Item item;
        item.age = 35;
		item.firstName = "Antonio";
		item.lastName = "Santana";
		item.message = "What is your message for the server? ";
		item.token = "connect";
		
		std::lock_guard<std::mutex> lock {mut};
		// std::string writeBuffer (MAX_BUFFER, 0);

		
		int i;

		char *writeBuffer = writeOutgoingStream(item);
		// char *writeBuffer=(char*)malloc(sizeof(item));
		// memcpy(writeBuffer,(const unsigned char*)&item,sizeof(item));
		//copying....
		// memcpy(writeBuffer,(const unsigned char*)&item,sizeof(item));
	
		//printing..
		printf("Copied byte array is:\n");
		for(i=0;i<sizeof(item);i++)
			printf("%02X ",writeBuffer[i]);
		printf("\n");
		
		//freeing memory..
		free(writeBuffer);
		
		// std::cout << "Producer ... loop ... START" << std::endl;
          	queue.push(item);
          	cond.notify_one();
		// std::cout << "Producer ... loop ... END" << std::endl;
     	}
}