#pragma once
#include <stack>
#include "Memento.h"

class Document
{
public:
	using Attributes = std::map<std::string, std::string>;

	Document();
	~Document();

	bool IsTextBold() const;
	bool IsTextItalic() const;
	bool IsTextUnderlined() const;
	std::string GetAttribute(std::string key);
	const std::string GetDocumentName() const;
	const std::string GetDocumentContext() const;
	Memento::Attributes GetAllAdditionalAttributes();

	void Undo();
	void SetDocumentContext(std::string context);
	void SetDocumentName(std::string name);
	void SetDocumentAttribute(std::string key, std::string value);
	void ChangeBold();
	void ChangeItalics();
	void ChangeUnderlining();
	Document & get();

private:

	std::stack<Memento> memory;
	int state;
};

