#include "moviedb.h"

using namespace std;

MovieDatabase::MovieDatabase()
{
}

MovieDatabase::~MovieDatabase()
{
}

bool MovieDatabase::isEmpty() const
{
    return false;
}

int MovieDatabase::getSize() const 
{
    return 0;
}

const Movie& MovieDatabase::getFront() const
{
    abort();
}

const Movie& MovieDatabase::getBack() const
{
    abort();
}

void MovieDatabase::addFront(const Movie& movie)
{
}

void MovieDatabase::addBack(const Movie& movie)
{
}

void MovieDatabase::removeFront()
{
}

void MovieDatabase::removeBack()
{
}

void MovieDatabase::append(const MovieDatabase& otherDB, function<bool(const Movie&)> predicate)
{
}

void MovieDatabase::load(const std::string& filename)
{
}

void MovieDatabase::beginTransaction()
{
}

void MovieDatabase::next()
{
}

void MovieDatabase::previous()
{
}

void MovieDatabase::insert(const Movie& movie)
{
}

void MovieDatabase::remove()
{
}

const Movie& MovieDatabase::getCurrent() const
{
    abort();
}

void MovieDatabase::endTransaction()
{
}
