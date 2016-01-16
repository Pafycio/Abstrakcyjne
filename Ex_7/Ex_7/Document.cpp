#include "Document.h"
#include <iostream>


Document::Document() 
{
	memory.push(Memento());
}


Document::~Document()
{
}

bool Document::IsTextBold() const
{
	return memory.top().IsTextBold();
}

bool Document::IsTextItalic() const
{
	return memory.top().IsTextItalic();
}

bool Document::IsTextUnderlined() const
{
	return memory.top().IsTextUnderlined();
}

std::string Document::GetAttribute(std::string key)
{
	return memory.top().GetAttribute(key);
}

const std::string Document::GetDocumentName() const
{
	return memory.top().GetDocumentName();
}

const std::string Document::GetDocumentContext() const
{
	return memory.top().GetDocumentContext();
}

void Document::Undo()
{
	memory.pop();
}

void Document::SetDocumentContext(std::string context)
{
	Memento newMemory = memory.top();
	newMemory.SetDocumentContext(context);
	memory.push(newMemory);
}

void Document::SetDocumentName(std::string name)
{
	Memento newMemory = memory.top();
	newMemory.SetDocumentName(name);
	memory.push(newMemory);
}

void Document::SetDocumentAttribute(std::string key, std::string value)
{
	Memento newMemory = memory.top();
	newMemory.SetAttribute(key, value);
	memory.push(newMemory);
}

void Document::ChangeBold()
{
	Memento newMemory = memory.top();
	newMemory.ChangeBold();
	memory.push(newMemory);
}

void Document::ChangeItalics()
{
	Memento newMemory = memory.top();
	newMemory.ChangeItalics();
	memory.push(newMemory);
}

void Document::ChangeUnderlining()
{
	Memento newMemory = memory.top();
	newMemory.ChangeUnderlining();
	memory.push(newMemory);

	std::cout << IsTextBold() << std::endl;
	std::cout << IsTextItalic() << std::endl;
	std::cout << IsTextUnderlined() << std::endl;
}

Document & Document::get()
{
	return *this;
}

Memento::Attributes Document::GetAllAdditionalAttributes()
{
	return memory.top().GetAllAdditionalAttributes();
}

