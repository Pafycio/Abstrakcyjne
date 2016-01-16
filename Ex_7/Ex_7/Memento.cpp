#include "Memento.h"
#include <stdexcept>


Memento::Memento()
{
	fileContext = "";
	fileName = "";
	isBold = false;
	isItalic = false;
	isUnderlined = false;
}

Memento::Memento(std::string context, std::string name)
{
	fileContext = context;
	fileName = name;
	isBold = false;
	isItalic = false;
	isUnderlined = false;
}


Memento::~Memento()
{
}

bool Memento::IsTextBold() const
{
	return isBold;
}

bool Memento::IsTextItalic() const
{
	return isItalic;
}

bool Memento::IsTextUnderlined() const
{
	return isUnderlined;
}

const std::string Memento::GetDocumentName() const
{
	return fileName;
}

const std::string Memento::GetDocumentContext() const
{
	return fileContext;
}

unsigned Memento::GetNumberOfAdditionalAttributes()
{
	return attributes.size();
}

void Memento::SetDocumentName(std::string name)
{
	fileName = name;
}

void Memento::SetDocumentContext(std::string context)
{
	fileContext = context;
}

void Memento::ChangeBold()
{
	isBold = !isBold;
}

void Memento::ChangeItalics()
{
	isItalic = !isItalic;
}

void Memento::ChangeUnderlining()
{
	isUnderlined = !isUnderlined;
}

void Memento::SetAttribute(std::string type, std::string value)
{
	attributes.insert(std::make_pair(type, value));
}

std::string Memento::GetAttribute(std::string key) 
{
	std::string value = attributes[key];
	if (value.empty())
		throw std::invalid_argument("Key not found");
	else
		return value;
}

bool Memento::operator==(const Memento & rhs) const noexcept
{
	return (GetDocumentName() == rhs.GetDocumentName() &&
		GetDocumentContext() == rhs.GetDocumentContext() &&
		IsTextBold() == rhs.IsTextBold() &&
		IsTextItalic() == rhs.IsTextItalic() &&
		IsTextUnderlined() == rhs.IsTextUnderlined());
}

bool Memento::operator!=(const Memento & rhs) const noexcept
{
	return !operator ==(rhs);
}

Memento::Attributes Memento::GetAllAdditionalAttributes()
{
	return attributes;
}

