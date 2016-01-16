#pragma once
#pragma once
template<class T, typename A, typename B>
class DoubleArgumentsCommand
{
public:
	DoubleArgumentsCommand(Document* d, T f, A a, B b);
	void Execute();

private:
	Document *doc;
	T function;
	A argument;
	B argument2;
};

template<class T, typename A, typename B>
DoubleArgumentsCommand<T, A, B>::DoubleArgumentsCommand(Document* d, T f, A a, B b)
{
	doc = d;
	function = f;
	argument = a;
	argument2 = b;
}

template<class T, typename A, typename B>
void DoubleArgumentsCommand<T, A, B>::Execute()
{
	(doc->*function)(argument, argument2);
}