#include "File.h"

//the basic action of grayscale and monochrome is to do nothing

void File::grayscale()
{
}

void File::monochrome()
{
}

const MyString& File::getFileName() const
{
	return fileName;
}

int File::getType() const
{
	return type;
}

size_t File::getWith() const
{
	return with;
}

size_t File::getHeight() const
{
	return height;
}
