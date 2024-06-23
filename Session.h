#pragma once
#include"File.h"

class Session
{
	File** images = nullptr;
	size_t size = 0;
	size_t capacity = 0;

	size_t id = 0;
	size_t transformations = 0;
	bool isChanged = 0;

	void resize(size_t newCap);
	void moveFrom(Session&& other);
	void copyFrom(const Session& other);
	void free();

	File* factory(const char* fileName);

	size_t position(const char* fileName);
public:
	Session() = default;
	Session(const char* fileName, size_t id);

	Session(const Session& other);
	Session(Session&& other) noexcept;

	Session& operator=(const Session&);
	Session& operator=(Session&&) noexcept;

	~Session() noexcept;

	void sessionInfo();
	void add(const char* fileName);
	void undo();
	void save();
	void saveAs(const char* fileName);
	void collage(const char* first, const char* second, char direction, const char* newFileName);
	void rotate(char ch);
	void negative();
	void monochrome();
	void grayscale();
};

