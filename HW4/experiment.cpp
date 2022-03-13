#include <iostream>
#include <queue>
#include <condition_variable>
#include <thread>

struct Item
{
	int age;
	std::string name;
	std::string surname;
};

std::queue<Item> waitingRoomChairs;
std::condition_variable globalConditionVariable;
std::mutex globalLock;
int count = 0;

void Customer();
void Barber();
void AddOneCustomer();
void CustomerGenerator();

int main()
{
	std::thread t1 (CustomerGenerator);
    std::thread t2 (Customer);
    std::thread t3 (Barber);
	
    t1.join();
    t2.join();
	t3.join();

    return 0;
}

/**
 * @brief Simulate a random start of customers. The customer 
 * range can be from 1 to 10 customers.
 * 
 */
void CustomerGenerator()
{
	int customers = 0;
	customers = 1 + (rand() % static_cast<int>(3));
	std::cout << "Starting with " << customers << std::endl;
	// int seconds = 1;
	for (int i = 0; i < customers; i++)
	{
		std::cout << "Add to the queue: " << i << std::endl;
		void AddOneCustomer();
		// count++;
		// std::this_thread::sleep_for(std::chrono::seconds(seconds));
	}
	
}

void AddOneCustomer()
{
	Item nextCustomerInLine;
	nextCustomerInLine.age = 35;
	nextCustomerInLine.name = "Jack";
	nextCustomerInLine.surname = "Sparrow";

	std::cout << "Push " << nextCustomerInLine.name << " to the queue." << std::endl;
	waitingRoomChairs.push(nextCustomerInLine);
	std::cout << "Number of customer sitting in the waiting room chairs = " << waitingRoomChairs.size() << std::endl;
}

void Barber()
{
	std::cout << "Barber/Consumer ... " << std::endl;
	std::cout << "Number of customer sitting in the waiting room chairs = " << waitingRoomChairs.size() << std::endl;
	// int seconds = 1 + (rand() % static_cast<int>(3));
	// std::this_thread::sleep_for(std::chrono::seconds(seconds));
	
	while(true)
	{	
		if (waitingRoomChairs.size() > 0)
		{
			// Critical section.
			std::unique_lock<std::mutex> lck{globalLock};
			std::cout << "\nBarber is giving a customer a haircut." << std::endl;
			// globalConditionVariable.wait(lck);
			// a lambda can be passed also to protects against spurious wake up 
			globalConditionVariable.wait(lck, []{ return !waitingRoomChairs.empty();});
          	auto nextCustomerInLine = waitingRoomChairs.front();
          	waitingRoomChairs.pop();
			// count--;
			std::cout << "Customer haircut complete." << std::endl;
			std::cout << " Name = " << nextCustomerInLine.name << std::endl;
			lck.unlock(); // Release the lock.

		} else
		{
			// std::cout << "Number of customer sitting in the waiting room chairs = " << waitingRoomChairs.size() << std::endl;
			/**
			 * @brief sleep_for 1 to 10 seconds randomly
			 * https://www.cplusplus.com/reference/thread/this_thread/sleep_for/
			 */ 
			int seconds = 1 + (rand() % static_cast<int>(2));
			// std::cout << "\nBarber is Sleeping..." << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(seconds));
		}
		
		
	} // End of while loop.
} // End of Barber function.

void Customer()
{
	// int seconds = 1 + (rand() % static_cast<int>(3));
	// std::this_thread::sleep_for(std::chrono::seconds(seconds));

	while(true)
	{
		if (waitingRoomChairs.size() > 1 )
		{
			std::cout << "Number of customer sitting in the waiting room chairs = " << waitingRoomChairs.size() << std::endl;
			std::cout << "Waiting room chairs are not empty. Add one customer to waiting queue." << std::endl;
			std::lock_guard<std::mutex> lock {globalLock};
			void AddOneCustomer();

			// Wake up the Barber
			globalConditionVariable.notify_one();

			int seconds = 1;// + (rand() % static_cast<int>(2));
			std::cout << "\nSleeping..." << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(seconds));
		} else {
			// Critical section.
			std::lock_guard<std::mutex> lock {globalLock};
			// std::cout << "Wake up the barber. Waiting room chairs are empty." << std::endl;
			void AddOneCustomer();

			// Wake up the Barber
			globalConditionVariable.notify_one();
		}
    } // End of while loop.
} // End of Customer function.