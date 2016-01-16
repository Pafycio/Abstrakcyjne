#pragma once
template<class T, typename A>
class SingleArgumentCommand
{
public:
	SingleArgumentCommand(Document* d, T f, A a);
	void Execute();

private:
	Document *doc;
	T function;
	A argument;
};

template<class T, typename A>
SingleArgumentCommand<T, A>::SingleArgumentCommand(Document* d, T f, A a)
{
	doc = d;
	function = f;
	argument = a;
}

template<class T, typename A>
void SingleArgumentCommand<T, A>::Execute()
{
	(doc->*function)(argument);
}