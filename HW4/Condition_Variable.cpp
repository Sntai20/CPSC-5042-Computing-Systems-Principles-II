#include <iostream>
#include <queue>
#include <condition_variable>
#include <thread>

struct Item
{
	int age;
	std::string name;
	std::string lastname;
};

std::queue<Item> queue;
std::condition_variable cond;
std::mutex mut;

void Customer();
void Barber();
Item AddOneCustomer();
void CustomerGenerator();
const int MAXNUMBEROFSEATS = 10;
int MAXNUMBEROFCUSTOMERS = 13;

int main()
{
    std::thread t1 (Customer);
    std::thread t2 (Barber);

	CustomerGenerator();

    t1.join();
    t2.join();

	std::cout << "\nQueue Size = " << queue.size() << std::endl;

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
	}
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

Item AddOneCustomer()
{
	Item item;
	item.age = 45;
	item.name = "Jack";
	item.lastname = "Sparrow";

	return item;
} // End of AddOneCustomer function.