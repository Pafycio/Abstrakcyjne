#include <iostream>
#include <string>
#include <list>
#include <map>
#include <gtest/gtest.h>

#include "Memento.h"
#include "Document.h"
#include "Command.h"
#include "CommandSingleArgument.h"
#include "DoubleArgumentsCommand.h"

using namespace std;
using namespace ::testing;


struct MementoTests : public ::testing::Test
{
	const string fileName = "file.txt";    // from C++11 it is legal in C++
	const string fileContext = "Ala ma kota";

	Memento memento = Memento(fileContext, fileName);

	bool isBold = false, isItalic = false, isUnderlined = false;

	void CheckTextFont(unsigned callingLineNumber, bool expcecteIsBold, bool expcecteIsItalic, bool expcecteIsUnderlined)
	{
		SCOPED_TRACE(callingLineNumber);
		ASSERT_EQ(expcecteIsBold, memento.IsTextBold());
		ASSERT_EQ(expcecteIsItalic, memento.IsTextItalic());
		ASSERT_EQ(expcecteIsUnderlined, memento.IsTextUnderlined());
	}
};

TEST_F(MementoTests, testConstructionOfSimpleMemento)
{
	ASSERT_EQ(fileName, memento.GetDocumentName());
	ASSERT_EQ(fileContext, memento.GetDocumentContext());

	CheckTextFont(__LINE__, isBold, isItalic, isUnderlined);

	ASSERT_EQ(0u, memento.GetNumberOfAdditionalAttributes());
}

TEST_F(MementoTests, testRenaming)
{
	ASSERT_EQ(fileName, memento.GetDocumentName());
	ASSERT_EQ(fileContext, memento.GetDocumentContext());

	const string newfileName("path/to/file/file.txt");
	memento.SetDocumentName(newfileName);

	ASSERT_EQ(newfileName, memento.GetDocumentName());
	ASSERT_EQ(fileContext, memento.GetDocumentContext());

	CheckTextFont(__LINE__, isBold, isItalic, isUnderlined);

	ASSERT_EQ(0u, memento.GetNumberOfAdditionalAttributes());
}

TEST_F(MementoTests, testChangingContext)
{
	ASSERT_EQ(fileName, memento.GetDocumentName());
	ASSERT_EQ(fileContext, memento.GetDocumentContext());

	const string newFileContext(
		"Ciekawe zadanie do rozwiazania:\n"
		"  SEND\n"
		"+ MORE\n"
		"_________\n"
		" MONEY"
		);
	memento.SetDocumentContext(newFileContext);

	ASSERT_EQ(fileName, memento.GetDocumentName());
	ASSERT_EQ(newFileContext, memento.GetDocumentContext());

	CheckTextFont(__LINE__, isBold, isItalic, isUnderlined);

	ASSERT_EQ(0u, memento.GetNumberOfAdditionalAttributes());
}

TEST_F(MementoTests, testChangingFontStyle)
{
	CheckTextFont(__LINE__, isBold, isItalic, isUnderlined);

	ASSERT_EQ(0u, memento.GetNumberOfAdditionalAttributes());

	memento.ChangeBold();
	isBold = true;
	CheckTextFont(__LINE__, isBold, isItalic, isUnderlined);

	memento.ChangeItalics();
	isItalic = true;
	CheckTextFont(__LINE__, isBold, isItalic, isUnderlined);

	memento.ChangeUnderlining();
	isUnderlined = true;
	CheckTextFont(__LINE__, isBold, isItalic, isUnderlined);

	memento.ChangeBold();
	isBold = false;
	CheckTextFont(__LINE__, isBold, isItalic, isUnderlined);

	ASSERT_EQ(0u, memento.GetNumberOfAdditionalAttributes());
}

TEST_F(MementoTests, testChangingAdditionalAttributes)
{
	CheckTextFont(__LINE__, isBold, isItalic, isUnderlined);

	ASSERT_EQ(0u, memento.GetNumberOfAdditionalAttributes());

	typedef map<string, string> Attributes;
	Attributes attributes;
	attributes.insert(make_pair("author", "Grzegorz"));
	attributes.insert(make_pair("group", "programmers"));

	for (Attributes::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
	{
		const string& key = it->first;
		memento.SetAttribute(key, attributes[key]);
	}

	ASSERT_EQ(attributes.size(), memento.GetNumberOfAdditionalAttributes());

	for (Attributes::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
	{
		const string& key = it->first;
		ASSERT_EQ(attributes[key], memento.GetAttribute(key));
	}

	ASSERT_THROW(memento.GetAttribute("noe existing attribute"), invalid_argument);
}

TEST_F(MementoTests, testComparation)
{
	Memento mementoNew(fileContext, fileName);

	ASSERT_EQ(memento.GetDocumentName(), mementoNew.GetDocumentName());
	ASSERT_EQ(memento.GetDocumentContext(), mementoNew.GetDocumentContext());

	ASSERT_EQ(memento, mementoNew);

	memento.ChangeBold();
	ASSERT_NE(memento, mementoNew);
}

//****************************************************************************************************************************
struct DocumentTests : public ::testing::Test
{
	Document document;
};

TEST_F(DocumentTests, testConstructionOfSimpleDocument)
{
	ASSERT_EQ("", document.GetDocumentName());
	ASSERT_EQ("", document.GetDocumentContext());

	ASSERT_FALSE(document.IsTextBold());
	ASSERT_FALSE(document.IsTextUnderlined());
	ASSERT_FALSE(document.IsTextItalic());

	ASSERT_EQ(0u, document.GetAllAdditionalAttributes().size());
}

TEST_F(DocumentTests, testUndo)
{
	vector<string> contexts =
	{
		"Ala ma kota",
		"Ala nie ma psa",
		"Krzysztof ma matke, ma brata"
	};

	for (unsigned i = 0; i < contexts.size(); ++i)
	{
		document.SetDocumentContext(contexts[i]);
		ASSERT_EQ(contexts[i], document.GetDocumentContext());
	}
	ASSERT_EQ("", document.GetDocumentName());

	for (int i = static_cast<int>(contexts.size()) - 2; i >= 0; --i)
	{
		document.Undo();
		ASSERT_EQ(contexts[i], document.GetDocumentContext());
	}
}

//****************************************************************************************************************************
struct CommandTests : public ::testing::Test
{
	Document * document = new Document();

	void CheckTextFont(unsigned callingLineNumber, bool expcecteIsBold, bool expcecteIsItalic, bool expcecteIsUnderlined)
	{
		SCOPED_TRACE(callingLineNumber);
		ASSERT_EQ(expcecteIsBold, document->IsTextBold());
		ASSERT_EQ(expcecteIsItalic, document->IsTextItalic());
		ASSERT_EQ(expcecteIsUnderlined, document->IsTextUnderlined());
	}

	template<typename Commands>
	void ExecuteAllCommands(Commands& c)
	{
		for (typename Commands::iterator it = c.begin(); it != c.end(); ++it)
			it->Execute();
	}
};

TEST_F(CommandTests, testNoArgumentsCommands)
{
	bool isBold = false, isItalic = false, isUnderlined = false;
	CheckTextFont(__LINE__, isBold, isItalic, isUnderlined);

	typedef Command<void(Document::*)()> CommandType;
	vector<CommandType> commandsToExecute =
	{
		CommandType(document, &Document::ChangeBold),
		CommandType(document, &Document::ChangeItalics),
		CommandType(document, &Document::ChangeUnderlining)
	};

	CheckTextFont(__LINE__, isBold, isItalic, isUnderlined); // no changes yet

	isBold = isItalic = isUnderlined = true;
	ExecuteAllCommands(commandsToExecute);
	CheckTextFont(__LINE__, isBold, isItalic, isUnderlined);
}

TEST_F(CommandTests, testSingleArgumentCommands)
{
	const string newDocumentContext = "Ale urwal, ale to bylo dobre!";
	const string newDocumentName("najlepszeCytatyInternetu.txt");

	typedef SingleArgumentCommand<void(Document::*)(const string), string> CommandType;
	vector<CommandType> commandsToExecute =
	{
		CommandType(document, &Document::SetDocumentContext, newDocumentContext),
		CommandType(document, &Document::SetDocumentName, newDocumentName)
	};

	ASSERT_EQ("", document->GetDocumentName()); // no changes yet
	ASSERT_EQ("", document->GetDocumentContext());

	ExecuteAllCommands(commandsToExecute);
	ASSERT_EQ(newDocumentName, document->GetDocumentName());
	ASSERT_EQ(newDocumentContext, document->GetDocumentContext());
}

TEST_F(CommandTests, testCommandToAddAttributes)
{
	typedef map<string, string> Attributes;

	Attributes attributes;
	attributes.insert(make_pair("autor", "Grzegorz"));
	attributes.insert(make_pair("numer zadania", "7"));
	attributes.insert(make_pair("data inspiracji do wymyslania zadania", "1 grudnia 2015"));

	typedef DoubleArgumentsCommand<void(Document::*)(const string, const string), string, string> CommandType;

	vector<CommandType> commandsToExecute;

	for (Attributes::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
		commandsToExecute.push_back(CommandType(document, &Document::SetDocumentAttribute, it->first, it->second));

	ASSERT_EQ(0u, document->GetAllAdditionalAttributes().size()); // no changes yet

	ExecuteAllCommands(commandsToExecute);

	ASSERT_EQ(attributes.size(), document->GetAllAdditionalAttributes().size());
	for (Attributes::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
	{
		const string& key = it->first;
		ASSERT_EQ(attributes[key], document->GetAttribute(key));
	}
}

//****************************************************************************************************************************
int main(int argc, char* argv[])
{
	InitGoogleTest(&argc, argv);
	int retVal = RUN_ALL_TESTS();
	cin.get();
	return retVal;
}