#include "Route.h"
#include "Customer.h"
#include<iostream>
#include<cassert>
#include<vector>
#include<cmath>
#include<limits>
#include<ctime>
#include<cstdlib>

const float MAX_FLOAT = numeric_limits<float>::max();

using namespace std;
Route::Route(Customer &headNode, Customer &rearNode, float capacity):capacity(capacity), quantityL(0), quantityB(0){ // ���캯��
	head = new Customer;
	*head = headNode;  // ���ƽڵ�
	rear = new Customer;
	*rear = rearNode; 
	head->front = NULL;
	head->next = rear;
	rear->front = head;
	rear->next = NULL;
	current = head;  // ��ʼ��currentָ��ָ��head�ڵ�
	size = 0;
	carIndex = -1;
	arrivedTime.push_back(0);
}

Route::~Route(){ // ��������
	this->clear();
}

void Route::copy(const Route &L){
	// Ӧ����L��head�ڵ��rear�ڵ㶼����
	this->size = L.size;
	this->carIndex = L.carIndex;
	this->capacity = L.capacity;
	this->quantityL = L.quantityL;
	this->quantityB = L.quantityB;
	Customer* originPtr = L.head;
	Customer* copyPtr = head;
	Customer* temp;
	while(originPtr!=NULL){
		// ��ͷ�ڵ�һֱ���Ƶ�β�ڵ�
		if(originPtr == L.head){  // ���ڸ��Ƶ�һ���ڵ�
			copyPtr = new Customer;
			copyPtr->front = NULL;
			head = copyPtr;
			*copyPtr = *L.head;
		} else{
			temp = new Customer;
			*temp = *originPtr;
			temp->front = copyPtr;
			copyPtr->next = temp;
			copyPtr = temp;
		}
		if(L.current == originPtr){
			current = copyPtr;
		}
		originPtr = originPtr->next;
	}
	temp->next = NULL;
	rear = temp;
}

Customer& Route::operator[] (int k){
	assert(k>=0 && k<size);
	Customer* temp = head->next;
	for(int i=0; i<k; i++){
		temp = temp->next;
	}
	return *temp;
}

const Customer& Route::operator[] (int k) const{
	assert(k>=0 && k<size);
	Customer* temp = head->next;
	for(int i=0; i<k; i++){
		temp = temp->next;
	}
	return *temp;
}

Route::Route(const Route &L){ // ���ƹ��캯��	
	this->copy(L);
}


Route& Route::operator= (const Route &L){ 
	// ����"="�����������������
	this->clear();  // ��յ�ǰ����	
	this->copy(L);
	return *this;
}

bool Route::isEmpty(){ //�ж������Ƿ�Ϊ��
	return (size==0);
}

bool Route::insertAfter(Customer &item1, Customer &item2){
	// ����������item1��ͬ�Ľڵ�������ڵ�item2
	Customer* temp = new Customer;
	*temp = item2;
	Customer* ptr = head;
	while(ptr!=rear){
		if (ptr->id == item1.id){  // ����id���ж������ڵ��Ƿ���ͬ
			break;
		}
		ptr = ptr->next;
	}
	if(ptr == rear) {
		// û���ҵ�������false
		return false;
	} 
	switch (item2.type){
	case 'L':
		quantityL = quantityL + item2.quantity;
		break;
	case 'B':
		quantityB = quantityB + item2.quantity;
		break;
	}
	temp->next = ptr->next;
	ptr->next->front = temp;
	temp->front = ptr;
	ptr->next = temp;
	size++;
	return true;
}

void Route::insertAtFront(Customer &item){ // �ڱ�ͷ����item
	Customer *temp = new Customer;
	*temp = item;
	if(size == 0){
		// ����ǿ�����
		head->next = temp;
		temp->front = head;
		temp->next = rear;
		rear->front = temp;
		current = head;
	}else{  // ����frontָ����һ��Customer�ڵ㣬ӵ������next
		temp->next = head->next;
		head->next->front = temp;
		head->next = temp;
		temp->front = head;
	}
	switch (item.type){
	case 'L':
		quantityL = quantityL + item.quantity;
		break;
	case 'B':
		quantityB = quantityB + item.quantity;
		break;
	}
	size++;
}

bool Route::deleteNode(Customer &item){
	// ɾ����������item��ͬ�Ľڵ�
	Customer* temp1 = head->next;
	while(temp1!=rear) {
		if(temp1->id == item.id) {  // ����ҵ���temp1ָ��ǰ�ڵ㣬temp2->next=temp1;
			break;
		}
		temp1 = temp1->next;
	}
	if(temp1 == rear) {  // û���ҵ�
		return false;
	} else {
		Customer* nextNode = temp1->next;
		Customer* frontNode = temp1->front;
		frontNode->next = nextNode;
		nextNode->front = frontNode;
		delete temp1;
		size--;
		switch (item.type){
		case 'L':
			quantityL = quantityL - item.quantity;
			break;
		case 'B':
			quantityB = quantityB - item.quantity;
			break;
		}
		return true;
	}
}

bool Route::next(){ // currentָ����ǰ��
	current = current->next;
	if(current == rear) {  // �ߵ��˾�ͷ
		return false;
	} else {
		return true;
	}
}

Customer& Route::currentPos(){ // ���ص�ǰλ��
	return *current;
}

void Route::clear(){  // ��������������head�ڵ��rear�ڵ�?
	Customer* ptr1 = head;
	Customer* ptr2;
	while(ptr1!=NULL){
		ptr2 = ptr1->next;
		delete ptr1;
		ptr1 = ptr2;
	}
	head = NULL;
	rear = NULL;
	current = NULL;
	size = 0;
	carIndex = -1;  
}

void Route::assignCar(int carIndex){ // Ϊroute���䳵��
	this->carIndex = carIndex;
}

Route& Route::capture(){ 
	// ץȡcurrentָ����·��
	// currentָ�뵱ǰ�ڵ㽫��Ϊhead�ڵ�
	Route* ptr1 = new Route(*current, *rear, capacity);
	if(current->next == rear) { // currentָ����Ѿ�û��·��
		return *ptr1;
	}
	Customer* ptr2 = current->next;
	Customer *ptr3, *ptr4;
	ptr4 = ptr1->head;
	while(ptr2 != rear) {
		ptr3 = new Customer;
		*ptr3 = *ptr2;
		ptr4->next = ptr3;
		ptr3->front = ptr4;
		ptr4 = ptr3;
		switch (ptr2->type){
		case 'L':
			ptr1->quantityL = ptr1->quantityL + ptr2->quantity;
			break;
		case 'B':
			ptr1->quantityB = ptr1->quantityB + ptr2->quantity;
			break;
		}
		ptr2 = ptr2->next;
		ptr1->size++;
	}
	ptr3->next = ptr1->rear;
	ptr1->rear->front = ptr3;
	return *ptr1;
}

void Route::replaceRoute(const Route &route) {  // ��route�滻��currentָ����·��
	if(current == head) { // ��·����ֱ�Ӹ��Ƽ���
		this->clear();   
		this->copy(route);
		return;
	}
	Customer* ptr1 = route.head->next;
	Customer *ptr2, *ptr3;
	ptr2 = current->next;
	// ���ԭ·����currentָ������Ԫ��
	// ��������rear�ڵ�����
	while(ptr2 != rear) { 
		switch (ptr2->type){
		case 'L':
			quantityL = quantityL - ptr2->quantity;
			break;
		case 'B':
			quantityB = quantityB - ptr2->quantity;
			break;
		}
		ptr3 = ptr2->next;
		delete ptr2;
		ptr2 = ptr3;
		size--;
	}
	// ��route�г�head��rear��Ľڵ㶼���Ƶ�currentָ���
	ptr3 = current;
	while(ptr1 != route.rear){  
		switch (ptr1->type){
		case 'L':
			quantityL = quantityL + ptr1->quantity;
			break;
		case 'B':
			quantityB = quantityB + ptr1->quantity;
			break;
		}
		ptr2 = new Customer;
		*ptr2 = *ptr1;
		ptr3->next = ptr2;
		ptr2->front = ptr3;
		ptr3 = ptr2;
		ptr1 = ptr1->next;
		size++;
	}
	ptr3->next = rear;
	rear->front = ptr3;
	return;
}

int Route::getSize() {
	return this->size;
}

void Route::printRoute(){ // ��ӡ����
	Customer* ptr = head;
	for(; ptr!=NULL; ptr=ptr->next) {
		cout << "id:" << ptr->id << ' ' << "type:" << ' ' << ptr->type << endl;
	}
}

vector<Customer*> Route::getAllCustomer(){  // �õ�·�������еĹ˿ͽڵ�
	vector<Customer*> customerSet(size);
	Customer* ptr = head->next;
	Customer* ptr2;
	for(int i=0; i<size; i++){
		ptr2 = new Customer;
		*ptr2 = *ptr;
		customerSet[i] = ptr2;
		ptr = ptr->next;
	}
	return customerSet;
}

vector<float> Route::computeReducedCost(){ 
	// �õ����й˿ͽڵ���Ƴ�����
	// ֵԽС��ʾ�Ƴ������Խ�ʡ����Ĵ���
	vector<float> costArr(0);
	Customer *ptr1 = head;   // ǰ�ڵ�
	Customer *ptr2, *ptr3;
	for(int i=0; i<size; i++){
		ptr2 = ptr1->next;  // ��ǰ�ڵ�
		ptr3 = ptr2->next;  // ��ڵ�
		float temp =  -sqrt(pow(ptr1->x - ptr2->x, 2) + pow(ptr1->y - ptr2->y, 2)) - 
			sqrt(pow(ptr2->x - ptr3->x, 2) + pow(ptr2->y - ptr3->y, 2)) +
			sqrt(pow(ptr1->x - ptr3->x, 2) + pow(ptr1->y - ptr3->y, 2));
		costArr.push_back(temp);
		ptr1 = ptr1->next;
	}
	return costArr;
} 

bool Route::timeWindowJudge(Customer *pre, int pos, Customer item){  
	// �����item���뵽pre�����Ƿ��Υ��ʱ�䴰Լ��
	// ��ʱ�����ǲֿ�Ĺز�ʱ��
	// pos��pre��λ��, 0��ʾ�ֿ�
	float time = arrivedTime[pos];
	Customer *ptr1, *ptr2;

	// ���������жϲ���item��᲻��Υ��item�������̽ڵ��ʱ�䴰Լ��
	if(time < pre->startTime){   // arrivedTime[pos]ֻ�ӵ���pre��arrived time��û���ж��Ƿ���ǰ����
		time = pre->startTime;
	}
	time += pre->serviceTime;
	time = time + sqrt(pow(pre->x - item.x, 2) + pow(pre->y - item.y, 2));
	if(time > item.endTime) {  // Υ����ʱ�䴰Լ��
		return false;
	} else{
		if(time < item.startTime) {
			time = item.startTime;
		}
		time = time + item.serviceTime;
		ptr2 = pre->next;
		if(ptr2 == rear){  // item��������յ㣬��ʱ������
			return true;
		} else {
			time = time + sqrt(pow(ptr2->x - item.x, 2) + pow(ptr2->y - item.y, 2));
			if(time > ptr2->endTime) {
				return false;
			} else {
				if(time < ptr2->startTime) {
					time = ptr2->startTime;
				}
				time = time + ptr2->serviceTime;
			}
		}
	}

	// Ȼ���жϻ᲻��Υ��������Ľڵ��ʱ�䴰Լ��
	bool mark = true;
	ptr1 = pre->next;
	ptr2 = ptr1->next;
	while(mark == true && ptr2 !=rear){ 
		time = time + sqrt(pow(ptr1->x - ptr2->x, 2) + pow(ptr1->y - ptr2->y, 2));	
		if(time > ptr2->endTime){
			mark = false;
			break;
		} else {
			if(time < ptr2->startTime){
				time = ptr2->startTime;
			}
			time = time + ptr2->serviceTime;
		}
		ptr1 = ptr1->next;
		ptr2 = ptr2->next;
	}
	return mark;
}

void Route::computeInsertCost(Customer item, float &minValue, Customer &customer1, float &secondValue, Customer &customer2,
							  float noiseAmount, bool noiseAdd){
	// ����item�ڵ���·���е���С������ۺʹ�С�������
	// ���������/�μѲ����ǰ��Ĺ˿ͽڵ�
	Customer *pre = head;
	Customer *succ = pre->next;
	minValue = MAX_FLOAT;
	secondValue = MAX_FLOAT;
	customer1.id = -1;
	customer2.id = -1;
	for(int i=0; i<=size; i++) {  // һ����size+1��λ�ÿ��Կ��ǲ���
		switch(item.type){
		case 'L':
			if(pre->type == 'D' || pre->type == 'L') { // ���ȼ�Լ��
				if(quantityL + item.quantity <= capacity){   // ����Լ��
					if(timeWindowJudge(pre, i, item) == true) { // ����ʱ�䴰Լ��
						float temp = sqrt(pow(pre->x - item.x, 2) + pow(pre->y - item.y, 2)) +
								sqrt(pow(item.x - succ->x, 2) + pow(item.y - succ->y, 2)) -
								sqrt(pow(pre->x - succ->x, 2) + pow(pre->y - succ->y, 2));
						if(noiseAdd == true) { // �����Ҫ�����������
							float y = rand()/(RAND_MAX+1.0f);   // y in (0,1)
							float noise = -noiseAmount + 2*noiseAmount*y;
							temp = max(temp+noise, 0.0f);
						}
						if(temp <= minValue){  // �ҵ��˸�С�ģ�����minValue��secondValue
							secondValue = minValue;
							customer2 = customer1;
							minValue = temp;
							customer1 = *pre;
							int aa = 0;
						}
					}
				}
			}
			break;
		case 'B':
			if((pre->type == 'L' && succ->type == 'B') || (pre->type == 'L' && succ->type == 'D') || (pre->type == 'B') == true){  
				// ���ȼ�Լ��
				if(quantityB + item.quantity <= capacity){    // ����Լ��
					if(timeWindowJudge(pre, i, item) == true) {  // ����ʱ�䴰Լ��
						float temp = sqrt(pow(pre->x - item.x, 2) + pow(pre->y - item.y, 2)) +
								sqrt(pow(item.x - succ->x, 2) + pow(item.y - succ->y, 2)) -
								sqrt(pow(pre->x - succ->x, 2) + pow(pre->y - succ->y, 2));
						if(noiseAdd == true) { // �����Ҫ�����������
							float y = rand()/(RAND_MAX+1.0f);   // y in (0,1)
							float noise = -noiseAmount + 2*noiseAmount*y;
							temp = max(temp+noise, 0.0f);
						}
						if(temp <= minValue){  // �ҵ��˸�С�ģ�����minValue��secondValue
							secondValue = minValue;
							customer2 = customer1;
							minValue = temp;
							customer1 = *pre;
						}
					
					}
				}
			}
			break;
		}
		pre = pre->next;
		if(succ != rear){
			succ = succ->next;
		}
	}
}

void Route::refreshArrivedTime(){   
	// ����һ�¸����ڵ�ĵ���ʱ��
	// ��һ��ֵΪ0
	arrivedTime.clear();
	arrivedTime.push_back(0);
	Customer* tfront = head;
	Customer* tcurrent = head->next;
	float time = 0;
	while(tcurrent != rear){
		time = time + sqrt(pow(tfront->x - tcurrent->x, 2) + pow(tfront->y - tcurrent->y, 2));
		arrivedTime.push_back(time);
		tcurrent->arrivedTime = time;
		if(time < tcurrent->startTime){
			time = tcurrent->startTime;
		}
		time = time + tcurrent->serviceTime;
		tfront = tfront->next;
		tcurrent = tcurrent->next;
	}
}

void Route::changeCarIndex(int newIndex){  // ���³������
	carIndex = newIndex;
}

float Route::getLen(){   // �õ�·������
	Customer *ptr1 = head;
	Customer *ptr2 = head->next;
	float len = 0;
	while(ptr2 != NULL){
		len = len + sqrt(pow(ptr1->x - ptr2->x, 2)+pow(ptr1->y - ptr2->y, 2));
		ptr2 = ptr2->next;
		ptr1 = ptr1->next;
	}
	return len;
}

vector<float> Route::getArrivedTime(){     // �õ��������нڵ��arrivedTime
	return arrivedTime;
}