#include "Session.h"
#include "PPM.h"
#include "PGM.h"
#include "PBM.h"

Session::Session(const Session& other)
{
	copyFrom(other);
}
Session::Session(Session&& other) noexcept
{
	moveFrom(std::move(other));
}

Session& Session::operator= (const Session& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}
Session& Session::operator=(Session&& other) noexcept
{
	if (this != &other)
	{
		free();
		moveFrom(std::move(other));
	}
	return *this;
}

Session::~Session() noexcept
{
	free();
}

void Session::sessionInfo()
{
	std::cout << "Session ID: " << id << std::endl << "Number of included images: "
	<< size << std::endl << "Number of transformations: " << transformations << std::endl;
	std::cout << "Files:" << std::endl;
	for (size_t i = 0; i < size; i++)
	{
		std::cout<< images[i]->getFileName().c_str() << std::endl;
	}
}

void Session::add(const char* fileName)
{ 
	File* toAdd = factory(fileName);
	
	if (size == capacity)
		resize(capacity * 2);
	images[size++] = toAdd;
}

void Session::undo()
{
	if (isChanged) {
		for (size_t i = 0; i < size; i++)
		{
			images[i]->undo();
		}
	}
	isChanged = 0;
}

void Session::save()
{
	for (size_t i = 0; i < size; i++)
	{
		images[i]->save();
	}
}

void Session::saveAs(const char* fileName)
{
	images[0]->saveAs(fileName);
}

Session::Session(const char* fileName, size_t id)
{
	capacity = 8;
	this->id = id;
	images = new File * [capacity] {nullptr};
	add(fileName);
}
void Session::moveFrom(Session&& other)
{
	images = other.images;
	other.images = nullptr;

	size = other.size;
	capacity = other.capacity;
	id = other.id;
	transformations = other.transformations;
	isChanged = other.isChanged;

	other.size = other.capacity = other.id = other.transformations = other.isChanged =0;
}
void Session::copyFrom(const Session& other)
{
	images = new File * [other.capacity];

	for (int i = 0; i < other.size; i++)
		images[i] = other.images[i]->clone();

	size = other.size;
	capacity = other.capacity;
	id = other.id;
	transformations = other.transformations;
	isChanged = other.isChanged;
}

void Session::free()
{
	for (int i = 0; i < size; i++)
		delete images[i]; 
	delete[] images;
}

File* Session::factory(const char* fileName)
{
	if (!fileName)
		throw std::invalid_argument("The file name is nullptr");
	size_t len = std::strlen(fileName);
	if (len > 4) {
		if (fileName[len] == 'm' && fileName[len - 2] == 'p' && fileName[len - 3] == '.') {
			switch (fileName[len - 1])
			{
			case 'p': return new PPM(fileName);

			case 'g': return new PGM(fileName);

			case 'b': return new PBM(fileName);

			default: throw std::invalid_argument("Òhe program does not support this file type");
			}
		}
	}
	throw std::invalid_argument("File name is too short");
}

size_t Session::position(const char* fileName)
{
	MyString file(fileName);
	for (size_t i = 0; i < size; i++)
	{
		if (images[i]->getFileName() == file)
			return i;
	}
	throw std::invalid_argument("There is no such image");
}

void Session::resize(size_t newCap)
{
	File** newData = new File * [newCap];
	for (int i = 0; i < size; i++)
		newData[i] = images[i];
	delete[] images;
	images = newData;
	capacity = newCap;
}



void Session::collage(const char* first, const char* second, char direction, const char* newFile)
{   
	add(images[position(first)]->collage(first, second, direction, newFile).c_str());
}

void Session::rotate(char ch)
{
	for (size_t i = 0; i < size; i++)
	{
		images[i]->rotate(ch);
	}
	transformations++;
	isChanged = 1;
}

void Session::negative()
{
	for (size_t i = 0; i < size; i++)
	{
		images[i]->negative();
	}
	transformations++;
	isChanged = 1;
}

void Session::monochrome()
{
	for (size_t i = 0; i < size; i++)
	{
		images[i]->monochrome();
	}
	transformations++;
	isChanged = 1;
}

void Session::grayscale()
{
	for (size_t i = 0; i < size; i++)
	{
		images[i]->grayscale();
	}
	transformations++;
	isChanged = 1;
}

