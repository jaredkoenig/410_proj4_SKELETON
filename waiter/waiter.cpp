/*
 * waiter.cpp
 *
 * Jared Koenig and Emery Weist
 *
 * 12/10/2020
 */

#include <string>
#include "stdlib.h"

#include "../includes/waiter.h"
#include "../includes/externs.h"

using namespace std;

Waiter::Waiter(int id, std::string filename) :
		id(id), myIO(filename) {
}

Waiter::~Waiter() {
}

int Waiter::getNext(ORDER &anOrder) {
	return myIO.getNext(anOrder);
}

void Waiter::beWaiter() {
	ORDER order;
	while (getNext(order) == SUCCESS) {
		{
			lock_guard<mutex> lock(mutex_order_inQ);
			order_in_Q.push(order);
			cv_order_inQ.notify_all();
		}
	}
	b_WaiterIsFinished = true;
	cv_order_inQ.notify_all();
}

