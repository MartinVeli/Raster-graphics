#include "PGM.h"


PGM::PGM(const char* fileName)
{
  
        if (!fileName)
            throw std::invalid_argument("Name of the file is nullptr");
        size_t len = std::strlen(fileName);
        if (len > 4) {
            if (fileName[len] == 'm' && fileName[len - 1] == 'g' && fileName[len - 2] == 'p' && fileName[len - 3] == '.') {

                std::ifstream file(fileName, std::ios::binary);

                if (!file.is_open())
                    throw std::runtime_error("Can`t open the file");
                file.read(reinterpret_cast<char*>(&with), sizeof(size_t));
                file.read(reinterpret_cast<char*>(&height), sizeof(size_t));
                file.read(reinterpret_cast<char*>(&maxValue), sizeof(unsigned char));
                file.read(reinterpret_cast<char*>(&pixels), with * height * sizeof(unsigned char));
                this->fileName = fileName;
                this->type = 1;
                file.close();
            }
            else
                throw std::invalid_argument("The file is not 'pgm' type");
        }
        else
            throw std::invalid_argument("The file is not 'pgm' type");
        
    }

PGM::PGM(const PGM& other, const char* file)
{
    if (!file)
        throw std::runtime_error("Name of the file is nullptr");
    size_t len = std::strlen(file);
    if (len > 4) {
        if (file[len] == 'm' && file[len - 1] == 'g' && file[len - 2] == 'p' && file[len - 3] == '.') {
            pixels = other.pixels;
            snapshot = nullptr;
            height = other.height;
            with = other.with;
            fileName = file;
            maxValue = other.maxValue;
            type = other.type;
        }
        else
            throw std::runtime_error("The file is not 'pgm' type");
    }
    else
        throw std::runtime_error("The file is not 'pgm' type");
    
}



File* PGM::clone() const
{
    File* newObj = new (std::nothrow)PGM(*this);
    return newObj;
}


void PGM::monochrome()
{
    if (snapshot) {
        delete snapshot;
    }
    snapshot = new Vector<unsigned char>(pixels);

    unsigned char half = maxValue / 2;
    for (size_t i = 0; i < pixels.getSize(); i++)
    {
        pixels[i] > half ? pixels[i] = '.' : pixels[i] = '#';
    }
}

void PGM::rotate(char direction)
{
    if (snapshot) {
        delete snapshot;
    }
    snapshot = new Vector<unsigned char>(pixels);
    std::swap(File::with, File::height);

    if (direction == 'l') {
        for (size_t i = File::with - 1, z = 0; i >= 0; i--, z += File::height) {
            for (size_t j = 0, k = File::height - 1, g = pixels.getSize() -1 - z; k >= 0; g--, j += with, k--) {
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

void PGM::negative()
{
    if (snapshot) {
        delete snapshot;
    }
    snapshot = new Vector<unsigned char>(pixels);

    for (size_t i = 0; i < pixels.getSize(); i++)
    {
        pixels[i] = maxValue - pixels[i];
    }
}

void PGM::undo()
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

void PGM::save()
{
    
        std::ofstream file(File::fileName.c_str(), std::ios::binary);
        if (!file.is_open())
            throw std::runtime_error("Can`t open the file");
        file.write(reinterpret_cast<const char*>(&with), sizeof(size_t));
        file.write(reinterpret_cast<const char*>(&height), sizeof(size_t));
        file.write(reinterpret_cast<const char*>(&maxValue), sizeof(int));
        file.write(reinterpret_cast<const char*>(&pixels), pixels.getSize() * sizeof(unsigned char));
        file.close();
    
}

void PGM::saveAs(const char* file1)
{
    std::ofstream file(file1, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Can`t open the file");
    file.write(reinterpret_cast<const char*>(&with), sizeof(size_t));
    file.write(reinterpret_cast<const char*>(&height), sizeof(size_t));
    file.write(reinterpret_cast<const char*>(&maxValue), sizeof(int));
    file.write(reinterpret_cast<const char*>(&pixels), pixels.getSize() * sizeof(unsigned char));
    file.close();
}

const MyString& PGM::collage(const char* first, const char* second, char direction, const char* newFile)const
{
    PGM firstFile(first);
    PGM secondFile(second);
    
        if (firstFile.getHeight() == secondFile.getHeight() &&
            firstFile.getWith() == secondFile.getWith())
        {
            PGM toReturn(first, newFile);
            for (size_t i = firstFile.pixels.getSize(), j = 0; j < secondFile.pixels.getSize(); i++, j++)
            {
                toReturn.pixels[i] = secondFile.pixels[j];
            }
            direction == 'h' ? toReturn.height *= 2 : toReturn.with *= 2;
            if (firstFile.maxValue < secondFile.maxValue)
                toReturn.maxValue = secondFile.maxValue;
            toReturn.save();
            return toReturn.fileName;
        }
        else
            throw std::runtime_error("Difference in widths or heights of images"); 

}

