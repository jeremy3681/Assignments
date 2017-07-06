#ifndef _SHOP_H_
#define _SHOP_H_
#include <pthread.h> // the header file for the pthread library
#include <queue> // the STL library: queue
#include<string>
#include<iostream>
#include<sstream>
#include<stdlib.h>
#include<vector>
using namespace std;
#define DEFAULT_CHAIRS 3 // the default number of chairs for waiting = 3
#define DEFAULT_BARBERS 1 // the default number of barbers = 1
class Shop {
public:
	struct ticket{
		int custid;
		int barbid;
		pthread_cond_t cv=PTHREAD_COND_INITIALIZER;
	};
	Shop(int nBarbers, int nChairs); // initialize a Shop object with nBarbers and nChairs
	Shop(); // initialize a Shop object with 1 barber and 3 chairs
	~Shop();
	int visitShop(int id); // return a non-negative number only when a customer got a service
	void leaveShop(int customerId, int barberId);
	void helloCustomer(int id);
	void byeCustomer(int id);
	int nDropsOff; // the number of customers dropped off
private:
	string int2string(int i);
	void print(int person, string message);
	unsigned int availchairs;
	pthread_mutex_t shopmutex = PTHREAD_MUTEX_INITIALIZER;
	queue<ticket*> sleepingcustomers;
	queue<ticket*> sleepingbarbers;
	vector<ticket*> barberchairs;
};
#endif

