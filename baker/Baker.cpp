#include <mutex>

#include "../includes/externs.h"
#include "../includes/baker.h"

using namespace std;

Baker::Baker(int id) :
		id(id) {
}

Baker::~Baker() {
}

void Baker::bake_and_box(ORDER &anOrder) {
	int total_boxes = anOrder.number_donuts / 12;
	int donut_extras = anOrder.number_donuts % 12;

	if (donut_extras > 0) {
		total_boxes++;
	}

	int donuts_remaining = anOrder.number_donuts;

	for (int i = 0; i < total_boxes; i++) {
		if (donuts_remaining >= 12) {
			Box newBox;
			for (int j = 1; j <= 12; j++) {
				DONUT newDonut;
				newBox.addDonut(newDonut);
				donuts_remaining--;
			}
			anOrder.boxes.push_back(newBox);
		} else {
			Box newSmallBox;
			while (donuts_remaining > 0) {
				DONUT aDonut;
				newSmallBox.addDonut(aDonut);
				donuts_remaining--;
			}
			anOrder.boxes.push_back(newSmallBox);
		}
	}

}

void Baker::beBaker() {
	bool cont = true;
	while (cont) {
		unique_lock<mutex> lock(mutex_order_inQ);
		while (!b_WaiterIsFinished && order_in_Q.size() == 0) {
			cv_order_inQ.wait(lock);
		}
		if (!order_in_Q.empty()) {
			ORDER next;
			next = order_in_Q.front();
			order_in_Q.pop();
			this->bake_and_box(next);

			lock_guard<mutex> lg(mutex_order_outQ);
			order_out_Vector.push_back(next);
			cv_order_inQ.notify_all();
		} else if (b_WaiterIsFinished) {
			cv_order_inQ.notify_all();
			cont = false;
		}
	}
}
