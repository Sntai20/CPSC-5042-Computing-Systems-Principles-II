# Sleeping Barber Problem: Chapter 6, Question 26

## Problem Statement

Edsger Dijkstra introduced the Sleeping Barber Problem (Dijkstra, 1965): A barbershop is divided into two rooms. The waiting room has n chairs, and the work room only has the barber chair. When the waiting room is empty, the barber goes to sleep in the barber chair. If a customer comes in and the barber is asleep, he knows it’s his turn to get his hair cut. So he wakes up the barber
and takes his turn in the barber chair. But if the waiting room is not empty, the customer must take a seat in the waiting room and wait his turn. Write a program that will coordinate the barber and his customers.

## Solution

The solution to this problem includes three semaphores.

1. First is for the customer which counts the number of customers present in the waiting room (customer in the barber chair is not included because he is not waiting).

1. Second, the barber 0 or 1 is used to tell whether the barber is idle or is working, And the third mutex is used to provide the mutual exclusion which is required for the process to execute. In the solution, the customer has the record of the number of customers waiting in the waiting room if the number of customers is equal to the number of chairs in the waiting room then the upcoming customer leaves the barbershop.

When the barber shows up in the morning, he executes the procedure barber, causing him to block on the semaphore customers because it is initially 0. Then the barber goes to sleep until the first customer comes up.

When a customer arrives, he executes customer procedure the customer acquires the mutex for entering the critical region, if another customer enters thereafter, the second one will not be able to anything until the first one has released the mutex. The customer then checks the chairs in the waiting room if waiting customers are less then the number of chairs then he sits otherwise he leaves and releases the mutex.

If the chair is available then customer sits in the waiting room and increments the variable waiting value and also increases the customer’s semaphore this wakes up the barber if he is sleeping.

At this point, customer and barber are both awake and the barber is ready to give that person a haircut. When the haircut is over, the customer exits the procedure and if there are no customers in waiting room barber sleeps.

## Build

To build, navigate to the `./HW4/`

`cmake --preset macos-default`

Available configure presets:

 - "linux-default"   - Use this preset for Linux Debug
 - "macos-default"   - Use this preset for macOS Debug
 - "windows-default" - Use this preset for Windows x64 Debug

## Run the Code

To run the code, navigate to the build Project Folder `~/CPSC-5042-Computing-Systems-Principles-II/out/build/macos-default/HW4/`.

To run the HW4, `./Condition_Variable`