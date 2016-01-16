#pragma once
#include <string>
#include <map>

class Memento
{
public:
	using Attributes = std::map<std::string, std::string>;

	Memento();
	Memento(std::string context, std::string name);
	~Memento();

	bool IsTextBold() const;
	bool IsTextItalic() const;
	bool IsTextUnderlined() const;
	const std::string GetDocumentName() const;
	const std::string GetDocumentContext() const;
	unsigned GetNumberOfAdditionalAttributes();
	void SetDocumentName(std::string name);
	void SetDocumentContext(std::string context);
	void ChangeBold();
	void ChangeItalics();
	void ChangeUnderlining();
	void SetAttribute(std::string type, std::string value);
	std::string GetAttribute(std::string key);
	Attributes GetAllAdditionalAttributes();

	bool operator==(const Memento & rhs) const noexcept;
	bool operator!=(const Memento & rhs) const noexcept;
private:
	std::string fileName;
	std::string fileContext;

	bool isBold;
	bool isItalic;
	bool isUnderlined;

	Attributes attributes;
};

