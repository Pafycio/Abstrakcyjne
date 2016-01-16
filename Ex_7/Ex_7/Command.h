#pragma once
#include "Document.h"

template<class T>
class Command
{
public:
	Command() = delete;
	Command(Document* d, T f);

	void Execute();

private:
	Document* doc;
	T func;
};
template<class T>
Command<T>::Command(Document* d, T f)
{
	doc = d;
	func = f;
}

template<class T>
void Command<T>::Execute()
{
	(doc->*func)();
}

//-------------------------------//

//
//template<class T, class A>
//SingleArgumentCommand<T, A>::SingleArgumentCommand(Document* d, T f, A a)
//{
//	doc = d;
//	func = f;
//	arg = a;
//}
//
//template<class T, class A>
//void SingleArgumentCommand<T, A>::Execute()
//{
//	std::cout << "Run \n";
//	(obj->*func)(arg);
//}
////-------------------------------//
//
//template<class T, typename A, typename B>
//class DoubleArgumentsCommand
//{
//public:
//
//	DoubleArgumentsCommand(Document* d, T f, A a, B b);
//
//	void Execute();
//
//private:
//	Document * obj;
//	T func;
//	A arg;
//	B arg2;
//};
//
//template<class T, class A>
//DoubleArgumentsCommand<T, A, B>::SingleArgumentCommand(Document* d, T f, A a, B b)
//{
//	obj = d;
//	func = f;
//	arg = a;
//	arg2 = b
//}
//
//template<class T, class A>
//void DoubleArgumentsCommand<T, A, B>::Execute()
//{
//	std::cout << "Run \n";
//	(obj->*func)(arg, arg2);
//}