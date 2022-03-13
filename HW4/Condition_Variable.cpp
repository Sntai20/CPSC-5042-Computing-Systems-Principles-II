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
void BarberCoordinator();
void barber(Item item);
Item AddOneCustomer();

int main()
{
    std::thread t1 (Customer);
    std::thread t2 (BarberCoordinator);

    t1.join();
    t2.join();

    return 0;
}

void barber(Item item)
{
	std::cout << "Cut hair" << std::endl;
	std::cout << "Age = " << item.age << " Name = " << item.name << " Lastname = " << item.lastname << std::endl;
	std::cout << "Queue Size = " << queue.size() << std::endl;
	std::cout << "Haircut complete ... END" << std::endl;
}

void BarberCoordinator()
{
	std::cout << "BarberCoordinator ... " << std::endl;
	while(true)
	{
		std::unique_lock<std::mutex> lck{mut};
		// std::cout << "Consumer ... loop ... START" << std::endl;
		cond.wait(lck);
		// cond.wait(lck, []{ return !queue.empty();});
          	auto item = queue.front();
          	queue.pop();
		barber(item);
		lck.unlock();
	}
} // End of BarberCoordinator.

void Customer()
{
	std::cout << "Customer ... " << std::endl;
	while(true)
	{
		if (queue.size() > 0 && queue.size() < 2)
		{
			std::cout << "queue is greater than 0. Add a customer to the queue" << std::endl;
			Item item = AddOneCustomer();

			std::lock_guard<std::mutex> lock {mut};
          	queue.push(item);
          	cond.notify_one();
		} 
		else if (queue.size() < 1)
		{
			std::cout << "queue is 0. Add a customer to the queue" << std::endl;
			Item item = AddOneCustomer();

			std::lock_guard<std::mutex> lock {mut};
          	queue.push(item);
          	cond.notify_one();
		}
	} // End of while loop.
} // End of Customer function.

Item AddOneCustomer()
{
	Item item;
	item.age = 35;
	item.name = "Jack";
	item.lastname = "Sparrow";

	// std::cout << "Push " << item.name << " to the queue." << std::endl;
	// queue.push(item);
	// std::cout << "Number of customer sitting in the waiting room chairs = " << waitingRoomChairs.size() << std::endl;
	return item;
} // End of AddOneCustomer function.