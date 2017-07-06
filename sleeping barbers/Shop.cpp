#include "Shop.h"
using namespace std;


Shop::Shop(int nBarbers, int nChairs)
{
	availchairs=nChairs;
	nDropsOff=0;
	barberchairs.resize(nBarbers);

	//tickets that last between function calls
	for(int i=0; i<nBarbers;i++)
	{
		ticket* ticketptr=new ticket;
		ticketptr->barbid=i;
		barberchairs[i] = ticketptr;
	}
}

Shop::Shop()
{
	availchairs=DEFAULT_CHAIRS;
	nDropsOff=0;
	barberchairs.resize(DEFAULT_BARBERS);
	for(int i=0; i<DEFAULT_BARBERS;i++)
	{
		ticket* ticketptr=new ticket;
		ticketptr->barbid=i;
		barberchairs[i] = ticketptr;
	}
}

string Shop::int2string(int i) {
	stringstream out;
	out << " ["<< i << "] ";
	return out.str();
}
void Shop::print(int person, string message) {
	cout << ((person > 0) ? "customer[" : "barber  [")
		<< abs(person) << "]: " << message << endl;
}

Shop::~Shop()
{
}

int Shop::visitShop(int id)
{
	pthread_mutex_lock(&shopmutex);
	int barbid;
	ticket wakeme;
	wakeme.custid=id;
	if(sleepingbarbers.size()==0)//barber wakes me
	{	
		if(availchairs==0)
		{
			print(id,"Leaves the shop because of no available waiting chairs.");
			pthread_mutex_unlock(&shopmutex);
			nDropsOff++;
			return -1;
		}
		availchairs--;
		sleepingcustomers.push(&wakeme);

		print(id,"Takes a waiting chair. Waiting chairs availiable ="+int2string(availchairs));
		pthread_cond_wait(&(wakeme.cv),&shopmutex);

		barbid = wakeme.barbid;
		print(id,"Woken by barber"+int2string(barbid));		
		availchairs++;
	}
	else//I wake barber
	{
		//get a sleeping barber and tell him my id
		barbid=sleepingbarbers.front()->barbid;
		sleepingbarbers.front()->custid=id;
		sleepingbarbers.pop();
		//I'll signal barber when I get to his chair
	}
	print(id,"Moves to Barber"+int2string(barbid)+"Waiting chairs availiable ="+int2string(availchairs));
	pthread_mutex_unlock(&shopmutex);
	return barbid;
}



void Shop::helloCustomer(int id)
{
	pthread_mutex_lock(&shopmutex);
	int custid;
	if(sleepingcustomers.size()==0)//customer wakes me
	{
		//put barber in sleeping pool
		sleepingbarbers.push(barberchairs[id]);

		print(-id,"Sleeps because of no customers");
		pthread_cond_wait(&(barberchairs[id]->cv),&shopmutex);

		//record cust who's in my chair
		custid=barberchairs[id]->custid;
		print(-id,"Woken by customer"+int2string(custid));
	}
	else//I wake customer
	{
		//record sleeping cust number and tell my number
		custid=sleepingcustomers.front()->custid;
		sleepingcustomers.front()->barbid=id;
		barberchairs[id]->custid=custid;

		print(-id,"Calls over customer"+int2string(custid));
		pthread_cond_signal(&(sleepingcustomers.front()->cv));
		sleepingcustomers.pop();		

		//print(-id,"Wait until customer"+int2string(custid)+"is in the chair");
		pthread_cond_wait(&(barberchairs[id]->cv),&shopmutex);
	}
	pthread_cond_signal(&(barberchairs[id]->cv));
	print(-id,"Starts haircut for customer"+int2string(custid));
	pthread_mutex_unlock(&shopmutex);
}

void Shop::leaveShop(int customerId, int barberId)
{
	pthread_mutex_lock(&shopmutex);

	//signal the barber if barber was waiting on me
	pthread_cond_signal(&(barberchairs[barberId]->cv));

	//wait for the barber to signal if I was waiting on him
	pthread_cond_wait(&(barberchairs[barberId]->cv),&shopmutex);
	
	print(customerId,"Wait for barber"+int2string(barberId)+"to be done with haircut");
	pthread_cond_wait(&(barberchairs[barberId]->cv),&shopmutex);

	print(customerId,"Goodbye barber"+int2string(barberId));
	pthread_mutex_unlock(&shopmutex);
}

void Shop::byeCustomer(int id)
{
	pthread_mutex_lock(&shopmutex);
	int custid=barberchairs[id]->custid;
	print(-id,"Haircut done. Goodbye customer"+int2string(custid));
	pthread_cond_signal(&(barberchairs[id]->cv));
	pthread_mutex_unlock(&shopmutex);
}
