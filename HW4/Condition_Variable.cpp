/**
 * @file Condition_Variable.cpp
 * @author Antonio Santana (asantana1@seattleu.edu)
 * @brief A condition variable is generally used to avoid busy waiting (looping repeatedly 
 * while checking a condition) while waiting for a resource to become available. For example, 
 * if you have a thread (or multiple threads) that can't continue onward until a queue is empty, 
 * the busy waiting approach would be to just wasting cycles. 
 * 
 * The problem with this is wasting processor time by having this thread repeatedly check the 
 * condition. Instead have a synchronization variable that can be signaled to tell the thread 
 * that the resource is available.
 * 
 * Synchronization variables can be used to let the barber thread wait/sleep while on a 
 * single particular condition (e.g. for the queue to be empty).
 * 
 * @version 0.1
 * @date 2022-03-12
 * 
 */

#include <iostream>
#include <queue>
#include <condition_variable>
#include <thread>

// This stuct simulates the customer instance.
struct Item
{
	int age;
	std::string name;
	std::string lastname;
};

std::queue<Item> queue; // Number of customer sitting in the waiting room chairs.
std::condition_variable cond;
std::mutex mut;

void Customer();
void Barber();
Item AddOneCustomer();
void CustomerGenerator();
const int MAXNUMBEROFSEATS = 10;
int MAXNUMBEROFCUSTOMERS = 13;

/**
 * @brief This is the main entry to the program. Start 
 * the customer and barber threads before generating
 * a random set of customers.
 * 
 * @return int 
 */
int main()
{
    std::thread t1 (Customer);
    std::thread t2 (Barber);

	CustomerGenerator();

    t1.join();
    t2.join();

    return 0;
}

/**
 * @brief Simulate a random start of customers. The customer 
 * range can be from 0 to 10 customers.
 * 
 */
void CustomerGenerator()
{
	int customers = 0;
	customers = 0 + (rand() % static_cast<int>(10));
	std::cout << "\nStarting with " << customers << std::endl;
	for (int i = 0; i < customers; i++)
	{
		// std::cout << "\nAdd to the queue: " << i << std::endl;
		Item item = AddOneCustomer();
		MAXNUMBEROFCUSTOMERS = MAXNUMBEROFCUSTOMERS - 1;
		// std::cout << "\nMAX NUMBER OF CUSTOMERS " << MAXNUMBEROFCUSTOMERS << std::endl;
		std::lock_guard<std::mutex> lock {mut};
		queue.push(item);
		cond.notify_one();
	}
}

/**
 * @brief Simulates a barber sleeping, waking up 
 * to a customer signal, and cutting hair before 
 * going back to sleep.
 */
void Barber()
{
	// std::cout << "Barber ... " << std::endl;
	while(true)
	{
		std::unique_lock<std::mutex> lck{mut};
		std::cout << "\nBarber ... is asleep ... loop ... START" << std::endl;
		cond.wait(lck);
		auto item = queue.front();
		queue.pop();
		std::cout << "\nBarber ... is awake ... Cut hair" << std::endl;
		// std::cout << "Age = " << item.age << " Name = " << item.name << " Lastname = " << item.lastname << std::endl;
		// std::cout << "Queue Size = " << queue.size() << std::endl;
		std::cout << "\nHaircut complete ... END" << queue.empty() << std::endl;
		// std::cout << "\nQueue Size = " << queue.size() << std::endl;
		lck.unlock();
	} // End of while loop.
} // End of Barber.

/**
 * @brief Simulates a customer checking to see if the waiting 
 * room chairs are empty and/or availble. If the max number of 
 * seats are taken, then the waiting area is full. The max number
 * of customers is just a simple way to limit the program from 
 * running too long. The program can also be set to run continuously.
 * 
 */
void Customer()
{
	// std::cout << "\nCustomer ... " << std::endl;
	while(MAXNUMBEROFCUSTOMERS > 0)
	{
		std::cout << "\n ... New Customer" << std::endl;
		std::cout << "\nNumber of open seats available is " << (MAXNUMBEROFSEATS - queue.size() ) << std::endl;
		if (queue.size() < MAXNUMBEROFSEATS && queue.size() >= 0 )
		{
			// std::cout << "queue is greater than 0 and . Add a customer to the queue" << std::endl;
			Item item = AddOneCustomer();
			MAXNUMBEROFCUSTOMERS = MAXNUMBEROFCUSTOMERS - 1;
			// std::cout << "\nMAXNUMBEROFCUSTOMERS " << MAXNUMBEROFCUSTOMERS << std::endl;
			std::lock_guard<std::mutex> lock {mut};
          	queue.push(item);
          	cond.notify_one();
		} 
	} // End of while loop.
} // End of Customer function.

/**
 * @brief Simple function to create a customer named 
 * Jack Sparrow. 
 * 
 * @return Item 
 */
Item AddOneCustomer()
{
	Item item;
	item.age = 45;
	item.name = "Jack";
	item.lastname = "Sparrow";

	return item;
} // End of AddOneCustomer function.