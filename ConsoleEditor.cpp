#include "ConsoleEditor.h"

void ConsoleEditor::load()
{
	try {
		std::cout << "Enter file:";
		char fileName[MaxFileNameSize];
		std::cin >> fileName;
		std::cout << std::endl;
		current = sessions.getSize();
		sessions.insert(Session(fileName, ++id), sessions.getSize());
		std::cout << "Session with ID: " << id << " started" << std::endl;
		std::cout << "Image '" << fileName << "' added" << std::endl;
	}
	catch (const std::runtime_error er) {
		std::cout << er.what();
	}
	catch (const std::invalid_argument er) {
		std::cout << er.what();
	};
	
}

void ConsoleEditor::exit()
{
	doYouWantToSave();
	std::cout << "Successfully closed the program.";
}

void ConsoleEditor::doYouWantToSave() {
	char ch;
	std::cout << "Do you want to save changes?" << std::endl;
	std::cout << "If you want enter 'y' for yes or anything else for no:";
	std::cin >> ch;
	std::cout << std::endl;
	if (ch == 'y')
		save();
}

void ConsoleEditor::close()
{
	if (sessions.getSize() > 0) {
		doYouWantToSave();
		sessions.clear();
		current = -1;
		id = 0;
		load();
	}
	else {
		std::cout << "No session started" << std::endl;
	}
}

void ConsoleEditor::save()
{
	try {
		sessions.operator[](current).save();
		std::cout << "Successfully saved changes" << std::endl;
	}
	catch (const std::runtime_error er) {
		std::cout << er.what();
	};
}

void ConsoleEditor::saveAs()
{
	try {
		std::cout << "Enter file:";
		char fileName[MaxFileNameSize];
		std::cin >> fileName;
		sessions.operator[](current).saveAs(fileName);
		std::cout << "Successfully saved new image" << std::endl;
	}
	catch (const std::runtime_error er) {
		std::cout << er.what();
	};
	
}

void ConsoleEditor::help()
{
	std::cout << "Òo start you have to enter a file to begin session." << std::endl;
	std::cout << "After that you must choose command!" << std::endl;
	std::cout << "Command is number between 1 and 14." << std::endl;
	std::cout << "1 is for 'load' - loading a new session" << std::endl;
	std::cout << "2 is for 'close' - closes current sessions" << std::endl; 
	std::cout << "3 is for 'help'" << std::endl;
	std::cout << "4 is for 'session information'" << std::endl;
	std::cout << "5 is for 'save' - saves changes to the same file" << std::endl;
	std::cout << "6 is for 'save as' - saves changes to a new file" << std::endl;
	std::cout << "7 is for 'collage' - a combination between 2 images that adds a new image to the session" << std::endl;
	std::cout << "8 is for 'undo'" << std::endl;
	std::cout << "9 is for 'grayscale'" << std::endl;
	std::cout << "10 is for 'rotate'" << std::endl;
	std::cout << "11 is for 'negative'" << std::endl;
	std::cout << "12 is for 'monochrome'" << std::endl;
	std::cout << "13 is for 'add'" << std::endl;
	std::cout << "14 is for 'exit'" << std::endl;
}

void ConsoleEditor::switchSession()
{
	std::cout << "Enter session number: ";
	size_t num;
	std::cin >> num;
	std::cout<< std::endl;
	if (num >= sessions.getSize() || num < 0)
		std::cout << "No session with such number exists: " << num << std::endl;
	else {
		std::cout << "Successfully switched to session with number: " << num << std::endl;
		current = num;
	}
}

void ConsoleEditor::collage()
{
	std::cout << "Enter first file:";
	char fileName1[MaxFileNameSize];
	std::cin >> fileName1;
	std::cout << std::endl;
	std::cout << "Enter second file:";
	char fileName2[MaxFileNameSize];
	std::cin >> fileName2;
	std::cout << std::endl;
	std::cout << "Enter first file:";
	char col[MaxFileNameSize];
	std::cin >> col;
	std::cout << std::endl;
	char direction = 0;
	while (true)
	{
		std::cout << "Enter direction 'h' for horizontal or 'v' for vertical:";
		char ch;
		std::cin >> ch;
		std::cout << std::endl;
		if (ch == 'h' || ch == 'v') {
			direction = ch;
			break;
		}
	}
	try{
		sessions.operator[](current).collage(fileName1, fileName2, direction, col);
	}
	catch (const std::runtime_error er) {
		std::cout << er.what();
	}
	catch (const std::invalid_argument er) {
		std::cout << er.what();
	};
}

void ConsoleEditor::undo()
{
	sessions.operator[](current).undo();
}

void ConsoleEditor::sessionInfo()
{
	sessions.operator[](current).sessionInfo();
}

void ConsoleEditor::rotate()
{
	while (true)
	{
		char ch;
		std::cout << "Choose direction" << std::endl;
		std::cout << "'l' for left or 'r' for right:";
		std::cin >> ch;
		std::cout << std::endl;
		if (ch == 'l' || ch == 'r') {
			sessions.operator[](current).rotate(ch);
			break;
		}
	}
}

void ConsoleEditor::negative()
{
	sessions.operator[](current).negative();

}

void ConsoleEditor::monochrome()
{
	sessions.operator[](current).monochrome();

}

void ConsoleEditor::grayscale()
{
	sessions.operator[](current).grayscale();

}

void ConsoleEditor::add()
{
	
	try {
		std::cout << "Enter file:";
		char fileName[MaxFileNameSize];
		std::cin >> fileName;
		std::cout << std::endl;
		sessions.operator[](current).add(fileName);
	}
	catch (const std::runtime_error er) {
		std::cout << er.what();
	}
	catch (const std::invalid_argument er) {
		std::cout << er.what();
	};

}

void ConsoleEditor::userCommands()
{
	help();
	load();
	while (true)
	{
		int command;
		std::cout << "Enter command:";
		std::cin >> command;
		std::cout << std::endl;
		switch (command)
		{
		case 1: load();
		case 2: close();
		case 3: help();
		case 4: sessionInfo();
		case 5: save();
		case 6: saveAs();
		case 7: collage();
		case 8: undo();
		case 9: grayscale();
		case 10: rotate();
		case 11: negative();
		case 12: monochrome();
		case 13: add();
		case 14: exit();
			break;
		default: std::cout << "Invalid command!" << std::endl;
		}
	}
}
