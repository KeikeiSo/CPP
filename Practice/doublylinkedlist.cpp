#include <iostream>
using namespace std;

class Node
{   
public:
    // default constructor
    Node():data(0),next(NULL),previous(NULL){};
    // personalized constructor
    Node(int data, Node* next, Node* previous):
    data(data),
    next(next),
    previous(previous)
    {};
    // destructor
    ~Node(){};

    // states
    int data;
    Node* next;
    Node* previous;
};


class DLLStructure
{
public:
    DLLStructure();
    DLLStructure(int array[], int size);
    ~DLLStructure();
    DLLStructure(const DLLStructure& dlls);

    // methods
    void PrintDLL();
    void InsertAfter(int valueToInsertAfter, int valueToBeInserted);
    void InsertBefore( int valueToInsertBefore, int valueToBeInserted);
    void Delete(int value);
    void Sort();
    bool IsEmpty();
    int GetHead();
    int GetTail();
    int GetSize();
    int GetMax();
    int GetMin();

private:
    int size;
    int max;
    int min;
    Node* first;
    Node* last;
};

DLLStructure::DLLStructure()
{
    Node* head = new Node();
    this->first = head;
    this->last = head;
    this->size = 0;
    this->min = 0;
    this->max = 0;
}

DLLStructure::DLLStructure(int array[], int size)
{
    Node* head = new Node(array[0], NULL, NULL);
    Node* tail = head;
    int max = array[0];
    int min = array[0];
    for (int i = 1; i < size; i++)
    {
        int currnum = array[i];
        if (currnum > max) max = currnum;
        if (currnum < min) min = currnum;
        Node* curr = new Node(currnum, NULL, tail);
        tail->next = curr;
        tail = curr;
    }
    this->first = head;
    this->last = tail;
    this->size = size;
    this->max = max;
    this->min = min;
}

DLLStructure::~DLLStructure()
{   
    Node* temp;
    while(this->first != this->last){
        temp = (this->first)->next;
        delete this->first;
        this->first = temp;
    }
    delete this->last;
}

void DLLStructure::PrintDLL()
{
    Node* curr = this->first;
    while(curr != this->last){
        cout << curr->data << ", ";
        curr = curr->next;
    }
    cout << this->last->data << endl;
}

void DLLStructure::InsertAfter( int valueToInsertAfter, int valueToBeInserted)
{
    Node* curr = this->first;
    if (this->IsEmpty())
    {
        curr->data = valueToBeInserted;
        this->size ++;
        this->max = valueToBeInserted;
        this->min = valueToBeInserted;
        return;
    }
    // find the value to insert after
    while(curr->data != valueToInsertAfter){
        if (curr == this->last) break;
        curr = curr->next;
    }
    // if it is the tail, make the new node tail
    if (curr == this->last)
    {
        Node* temp = new Node(valueToBeInserted, NULL, curr);
        curr->next = temp;
        this->last = temp;
    } else {
        Node* temp = new Node(valueToBeInserted, curr->next, curr);
        curr->next = temp;
        temp->next->previous = temp;
    }
    this->size ++;
    if (valueToBeInserted > this->max) this->max = valueToBeInserted;
    if (valueToBeInserted < this->min) this->min = valueToBeInserted;
}

void DLLStructure::InsertBefore( int valueToInsertBefore, int valueToBeInserted)
{
    Node* curr = this->first;
    if (this->IsEmpty())
    {
        curr->data = valueToBeInserted;
        this->size ++;
        this->max = valueToBeInserted;
        this->min = valueToBeInserted;
        return;
    }
    // find the node that have the value to be insert before
    while(curr->data != valueToInsertBefore){
        if (curr == this->last)
        {
            Node* temp = new Node(valueToBeInserted, this->first, NULL);
            this->first->previous = temp;
            this->first = temp;
            this->size ++;
            if (valueToBeInserted > this->max) this->max = valueToBeInserted;
            if (valueToBeInserted < this->min) this->min = valueToBeInserted;
            return;
        }
        curr = curr->next;
    }
    // if the valueToBeInserted before is the head, make it the new head
    if (curr == this->first)
    {
        Node* temp = new Node(valueToBeInserted, this->first, NULL);
        this->first->previous = temp;
        this->first = temp;
        this->size ++;
        if (valueToBeInserted > this->max) this->max = valueToBeInserted;
        if (valueToBeInserted < this->min) this->min = valueToBeInserted;
    } else {
        // exchange the data of curr and previous
        curr->data = curr->previous->data;
        curr->previous->data = valueToInsertBefore;
        // insert the value
        this->InsertAfter(valueToInsertBefore, valueToBeInserted);
        // exchange the data back
        curr->previous->previous->data = curr->data;
        curr->data = valueToInsertBefore;
    }
}

void DLLStructure::Delete(int value)
{
    // if the list is empty, return
    if (this->size == 0) return;

    Node* curr = this->first;
    while(curr->data != value){
        // if the value is not found, return
        if (curr == this->last) return;
        curr = curr->next;
    }
    // if the element to delete is the first element
    // let the second element be the head
    if (curr == this->first)
    {   
        curr->next->previous = NULL;
        this->first = curr->next;
        delete curr;
    // if the element to delete is the last element
    // let the second last element to be the tail
    } else if (curr == this->last) { 
        curr->previous->next = NULL;
        this->last = curr->previous;
        delete curr;
    } else {
        curr->previous->next = curr->next;
        curr->next->previous = curr->previous;
        delete curr;
    }
    this->size --;
    // change the max and min correpondingly
    if (this->size == 0)
    {
        this->min = 0;
        this->max = 0;
        return;
    }
    // if max is deleted, find the new max
    if (this->max == value)
    {
        Node* curr = this->first;
        int max = curr->data;
        while(curr != this->last)
        {
            curr = curr->next;
            if (curr->data > max) max = curr->data;
        }
        this->max = max;
    }
    // if min is deleted, find the new min.
    if (this->min == value)
    {
        Node* curr = this->first;
        int min = curr->data;
        while(curr != this->last)
        {
            curr = curr->next;
            if (curr->data < min) min = curr->data;
        }
        this->min = min;
    }
}

void DLLStructure::Sort()
{
    // I sort the string by swaping the current value with the minimum of the rest of the list.
    Node* curr = this->first;
    while(curr != this->last)
    {
        Node* min = curr;
        while(min != this->last)
        {
            int c = min->data;
            int n = min->next->data;
            if (c < n)
            {
                min->data = n;
                min->next->data = c;
            }
            min = min->next;
        }
        int t = curr->data;
        curr->data = min->data;
        min->data = t;
        curr = curr->next;
    }
}

bool DLLStructure::IsEmpty()
{
    // in my implementation, size is the only state that tells whether the dll empty or not
    // because the initial dll already have a head and tail with data value = 0.
    if (this->size == 0) return true;
    return false;
}

int DLLStructure::GetHead()
{   
    if (this->IsEmpty()) cout << "The array is empty" << endl;
    return this->first->data;
}

int DLLStructure::GetTail()
{
    if (this->IsEmpty()) cout << "The array is empty" << endl;
    return this->last->data;
}

int DLLStructure::GetSize()
{
    if (this->IsEmpty()) cout << "The array is empty" << endl;
    return this->size;
}

int DLLStructure::GetMax()
{
    if (this->IsEmpty()) cout << "The array is empty" << endl;
    return this->max;
}

int DLLStructure::GetMin()
{
    if (this->IsEmpty()) cout << "The array is empty" << endl;
    return this->min;
}

DLLStructure::DLLStructure(const DLLStructure& dlls)
{
    // deep copy the head
    Node* head = new Node(dlls.first->data, NULL, NULL);
    Node* tail = head;
    Node* curr = dlls.first;
    // deep copy the rest of the dll by iterate through the original dll
    while (curr != dlls.last)
    {
        curr = curr->next;
        Node* temp = new Node(curr->data, NULL, tail);
        tail->next = temp;
        tail = temp;
    }
    // copy the datas
    this->first = head;
    this->last = tail;
    this->size = dlls.size;
    this->max = dlls.max;
    this->min = dlls.min;
}


int main () 
{
    // Q 1, 2, 3 should obviously be implemented successfully
    // in order to run the following code
    int array [ 5 ] = { 11 , 2 , 7 , 22 , 4 };
    DLLStructure dll ( array , 5 ) ; // note that 5 is the size of the array
    dll.PrintDLL(); // the output should be: 11, 2, 7, 22, 4
    // Q 4
    dll.InsertAfter( 7 , 13 ); // To insert 13 after the first occurence of 7
    dll.PrintDLL(); // the output should be: 11, 2, 7, 13, 22, 4
    dll.InsertAfter( 25 , 7 ); // To insert 7 after the first occurence of 25
    dll.PrintDLL(); // the output should be: 11, 2, 7, 13, 22, 4, 7
    // Q 5
    dll.InsertBefore( 7 , 26 ); // To insert 26 before the first occurence of 7
    dll.PrintDLL(); // the output should be: 11, 2, 26, 7, 13, 22, 4, 7
    dll.InsertBefore( 19 , 12 ); // To insert 12 before the first occurence of 19
    dll.PrintDLL(); // the output should be: 12, 11, 2, 26, 7, 13, 22, 4, 7
    // Q 6
    dll.Delete( 22 );
    dll.PrintDLL(); // the output should be: 12, 11, 2, 26, 7, 13, 4, 7
    // Q 7
    dll.Sort();
    dll.PrintDLL(); // the output should be: 2, 4, 7, 7, 11, 12, 13, 26
    // Q 8
    if (dll.IsEmpty()) cout << "The list is empty" << endl ;
    // Q 9
    cout << "Head element is: " << dll.GetHead() << endl ;
    cout << "Tail element is: " << dll.GetTail() << endl ;
    // Q 10
    cout << "Number of elements in the list is: " << dll.GetSize() << endl ;
    // print to the screen the written answer for the theory question
    cout << "We can avoid looping by adding a new state called size ";
    cout << "and also change any method that will alter the size" << endl;
    cout << "I used this method in my implementation" << endl;

    // Q 11
    cout << "Max element is: " << dll.GetMax() << endl ;
    cout << "Min element is: " << dll.GetMin() << endl ;
    // Q 11 theory question
    // print to the screen the written answer for the theory question
    cout << "We can avoid looping by adding two states called max and min to DLLStructure ";
    cout << "and changing the methods that will affect the value of those two states" << endl;
    cout << "I used this method in my implementation" << endl;
    cout << endl;
    // Q 12 theory question
    // print to the screen the written answer for the theory question
    cout << "The default copy constructor only do shallow copy, ";
    cout << "but we have two pointer states. Thus if we use the default copy constructor, ";
    cout << "the copied DLL would change as the original DLL changes. ";
    cout << "Which may not be what we want to have while doing some operations" << endl;
    // Q 12
    DLLStructure dll2 (dll) ;
    dll2.PrintDLL(); // the output should be: 2, 4, 7, 7, 11, 12, 13, 26
    return 0 ;
}