#include "moviedb.h"

#include "csv.h"
#include <stdexcept>

using namespace std;

MovieDatabase::MovieDatabase()
{
    header = new Node;
    trailer = new Node;
    header->next = trailer;
    trailer->prev = header;
    size = 0;
    current = nullptr;
}

MovieDatabase::~MovieDatabase()
{
    endTransaction();

    while (!isEmpty()) {
        removeFront();
    }
    delete header;
    delete trailer;
}

bool MovieDatabase::isEmpty() const
{
    return header->next == trailer;
}

int MovieDatabase::getSize() const 
{
    return size;
}

const Movie& MovieDatabase::getFront() const
{
    if (isEmpty()) {
        throw runtime_error("Database is empty");
    }

    return header->next->movie;
}

const Movie& MovieDatabase::getBack() const
{
    if (isEmpty()) {
        throw runtime_error("Database is empty");
    }

    return trailer->prev->movie;
}

void MovieDatabase::addFront(const Movie& movie)
{
    add(header->next, movie);
}

void MovieDatabase::addBack(const Movie& movie)
{
    add(trailer, movie);
}

void MovieDatabase::removeFront()
{
    if (isEmpty()) {
        throw runtime_error("Database is empty");
    }

    remove(header->next);    
}

void MovieDatabase::removeBack()
{
    if (isEmpty()) {
        throw runtime_error("Database is empty");
    }

    remove(trailer->prev);
}

void MovieDatabase::append(const MovieDatabase& otherDB, function<bool(const Movie&)> predicate)
{
    for (Node *current = otherDB.header->next; current != otherDB.trailer; current = current->next) {
        if (predicate(current->movie)) {
            addBack(current->movie);
        }
    }
}

void MovieDatabase::load(const std::string& filename)
{
    io::CSVReader<2> in(filename);
    in.read_header(io::ignore_extra_column, "Title", "Year");
    std::string title;
    int year;
    while (in.read_row(title, year)) {
        Movie movie;
        movie.title = title;
        movie.year = year;
        addBack(movie);
    }
}

void MovieDatabase::beginTransaction()
{
    current = header;
}

void MovieDatabase::next()
{
    if (current == nullptr) {
        throw runtime_error("Transaction not started");
    }

    if (current == trailer) {
        throw runtime_error("Invalid transaction");
    }

    current = current->next;
}

void MovieDatabase::previous()
{
    if (current == nullptr) {
        throw runtime_error("Transaction not started");
    }

    if (current == header) {
        throw runtime_error("Invalid transaction");
    }

    current = current->prev;
}

void MovieDatabase::insert(const Movie& movie)
{
    if (current == nullptr) {
        throw runtime_error("Transaction not started");
    }

    if (current == trailer) {
        throw runtime_error("Invalid transaction");
    }

    add(current->next, movie);

    current = current->next;
}

void MovieDatabase::remove()
{
    if (current == nullptr) {
        throw runtime_error("Transaction not started");
    }

    if (current == header || current == trailer) {
        throw runtime_error("Invalid transaction");
    }

    Node* next = current->next;
    remove(current);
    current = next;
}

const Movie& MovieDatabase::getCurrent() const
{
    if (current == nullptr) {
        throw runtime_error("Transaction not started");
    }

    if (current == header || current == trailer) {
        throw runtime_error("Invalid transaction");
    }

    return current->movie;
}

void MovieDatabase::endTransaction()
{
    current = nullptr;
}

void MovieDatabase::add(Node* node, const Movie& movie)
{
    Node* newNode = new Node;
    newNode->movie = movie;
    newNode->next = node;
    newNode->prev = node->prev;
    node->prev->next = newNode;
    node->prev = newNode;
    size++;
}

void MovieDatabase::remove(Node* node)
{
    Node* before = node->prev;
    Node* after = node->next;
    before->next = after;
    after->prev = before;
    delete node;
    size--;
}
