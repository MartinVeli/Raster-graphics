#include "PBM.h"

PBM::PBM(const char* fileName)
{
    if (!fileName)
        throw std::invalid_argument("Name of the file is nullptr");
    size_t len = std::strlen(fileName);
    if (len > 4) {
        if (fileName[len] == 'm' && fileName[len - 1] == 'b' && fileName[len - 2] == 'p' && fileName[len - 3] == '.') {
            std::ifstream file(fileName, std::ios::binary);

            if (!file.is_open())
                throw std::runtime_error("Can`t open the file");
            file.read(reinterpret_cast<char*>(&with), sizeof(size_t));
            file.read(reinterpret_cast<char*>(&height), sizeof(size_t));
            file.read(reinterpret_cast<char*>(&pixels), with * height * sizeof(unsigned char));
            File::fileName = fileName;
            File::type = 2;
            file.close();
        }
        else
            throw std::invalid_argument("The file is not 'pbm' type");
    }
    else
        throw std::invalid_argument("The file is not 'pbm' type");
}

PBM::PBM(const PBM& other, const char* file)
{
    if (!file)
        throw std::runtime_error("Name of the file is nullptr");
    size_t len = std::strlen(file);
    if (len > 4) {
        if (file[len] == 'm' && file[len - 1] == 'b' && file[len - 2] == 'p' && file[len - 3] == '.') {
            pixels = other.pixels;
            snapshot = nullptr;
            height = other.height;
            with = other.with;
            fileName = file;
            type = other.type;
        }
        else
            throw std::runtime_error("The file is not 'pgm' type");
    }
    else
        throw std::runtime_error("The file is not 'pgm' type");
}

size_t getFileSize(std::ifstream& inFile) {
    size_t currPos = inFile.tellg();
    inFile.seekg(0, std::ios::end);
    size_t res = inFile.tellg();
    inFile.seekg(currPos);

    return res;
}

File* PBM::clone() const
{
	File* newObj = new (std::nothrow)PBM(*this);
	return newObj;
}


void PBM::rotate(char direction)
{
    if (snapshot) {
        delete snapshot;
    }
    snapshot = new Vector<bool>(pixels);
    std::swap(File::with, File::height);

    if (direction == 'l') {
        for (size_t i = File::with - 1, z = 0; i >= 0; i--, z += File::height) {
            for (size_t j = 0, k = File::height - 1, g = pixels.getSize() - 1 - z; k >= 0; g--, j += with, k--) {
                pixels[i + j] = (*snapshot)[g];
            }
        }
    }
    else {
        for (size_t i = File::with - 1, z = 0; i >= 0; i--, z += File::height) {
            for (size_t j = 0, k = File::height - 1, g = 0 + z; k >= 0; g++, j += with, k--) {
                pixels[i + j] = (*snapshot)[g];
            }
        }
    }
}

void PBM::negative()
{
    for (size_t i = 0; i < pixels.getSize(); i++)
    {
        pixels[i] = !pixels[i];
    }
}

void PBM::undo()
{
    if (snapshot) {
        for (size_t i = 0; i < snapshot->getSize(); i++)
        {
            pixels[i] = (*snapshot)[i];
        }
    }
    delete snapshot;
    snapshot = nullptr;
}

void PBM::save()
{
    
        std::ofstream file(File::fileName.c_str(), std::ios::binary);
        if (!file.is_open())
            throw std::runtime_error("Can`t open the file");
        file.write(reinterpret_cast<const char*>(&with), sizeof(size_t));
        file.write(reinterpret_cast<const char*>(&height), sizeof(size_t));
        file.write(reinterpret_cast<const char*>(&pixels), pixels.getSize() * sizeof(unsigned char));
        file.close();
    
}

void PBM::saveAs(const char* file1)
{
    std::ofstream file(file1, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Can`t open the file");
    file.write(reinterpret_cast<const char*>(&with), sizeof(size_t));
    file.write(reinterpret_cast<const char*>(&height), sizeof(size_t));
    file.write(reinterpret_cast<const char*>(&pixels), pixels.getSize() * sizeof(unsigned char));
    file.close();
}

const MyString& PBM::collage(const char* first, const char* second, char direction, const char* newFile)const
{
    PBM firstFile(first);
    PBM secondFile(second);
    
        if (firstFile.getHeight() == secondFile.getHeight() &&
            firstFile.getWith() == secondFile.getWith())
        {
            PBM toReturn(first, newFile);
            if (direction == 'h') {
                for (size_t i = firstFile.pixels.getSize(), j = 0; j < secondFile.pixels.getSize(); i++, j++)
                {
                    toReturn.pixels[i] = secondFile.pixels[j];
                }
                toReturn.height *= 2;
            }
            else {
                for (size_t i = 0, j = firstFile.getWith(), k = 0; i < secondFile.pixels.getSize(); i++, j += firstFile.getWith(), k += firstFile.getWith()) {

                    for (size_t g = firstFile.getWith(); g > 0; g--, j++, k++)
                    {
                        toReturn.pixels[k] = firstFile.pixels[i];
                        toReturn.pixels[j] = secondFile.pixels[i];
                    }
                }
                toReturn.with *= 2;
            }
            
            toReturn.save();
            return toReturn.fileName;
        }
        else
            throw std::runtime_error("Difference in widths or heights of images");
    
}
