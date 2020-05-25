#include <iostream>
#include <string>
using namespace std;

template<typename Type>
class List
{
public:
	List();
	~List();

	void put(Type data);
	int GetSize() { return Size;}
	void delet (Type element);
	Type& operator[](const int index);

private:
	template<typename Type>
	class Node
	{
	public:
		Node* Next,*Prev;
		Type data;
		Node(Type data=Type(), Node* Prev = nullptr ,Node* Next=nullptr)
		{
			this->data = data;
			this->Next = Next;
			this->Prev = Prev;
		}
	};
	int Size;
	Node<Type> *head;
};

template<typename Type>
List<Type>::List()
{
	Size = 0;
	head = nullptr;
}
template<typename Type>
List<Type>::~List()
{

}

template<typename Type>
void List<Type>::put(Type data)
{
	if (head == nullptr)
	{
		head = new Node<Type>(data);
	}
	else
	{
		Node<Type> *current = this->head;
		while (current->Next != nullptr)
		{
			current = current->Next;
		}
		Node<Type> *following = new Node<Type>(data);
		current->Next = following;
		following->Prev = current;
	}
	Size++;
}


template<typename Type>
void List<Type>::delet(Type element)
{
	Node<Type>* current = this->head;
	while (current != nullptr)
	{
		if (current->data == element)
		{
			Node<Type> *next = current->Next;
			Node<Type> *prev = current->Prev;
			next->Prev = prev;
			prev->Next = next;
			delete current;
			Size--;
			break;
		}
		current = current->Next;
	}
}

template<typename Type>
Type& List<Type>::operator[](const int index)
{
	int counter = 0;
	Node<Type>* current = this->head;
	while (current!=nullptr)
	{
		if (counter == index)
		{
			return current->data;
		}
		current = current->Next;
		counter++;
	}
}


int main()
{
	List<int> lst;
	lst.put(5);
	lst.put(10);
	lst.put(7);
	lst.put(11);
	cout << lst.GetSize() << endl<<endl;
	for (int i = 0; i < lst.GetSize(); i++)
	{
		cout << lst[i] << endl;
	}
	cout << endl;
	lst.delet(7);
	cout << lst.GetSize()<<endl<<endl;
	for (int i = 0; i < lst.GetSize(); i++)
	{
		cout << lst[i]<<endl;
	}
}