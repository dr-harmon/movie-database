#pragma once

#include <functional>
#include <string>

// Data for a single movie
struct Movie {
    std::string title;
    int year;
};

// An in-memory database of movies. Supports linked-list style operations
// and a transactional cursor.
class MovieDatabase {
public:
    // Constructs a new movie database with no entries.
    MovieDatabase();
    ~MovieDatabase();

    // Returns true if the database has no entries, otherwise false
    bool isEmpty() const;
    // Returns the number of entries in the database
    int getSize() const;
    // Returns the movie at the start of the database, or throws an exception
    // if the database is empty
    const Movie& getFront() const;
    // Returns the movie at the end of the database, or throws an exception
    // if the database is empty
    const Movie& getBack() const;
    // Adds the movie to the beginning of the database
    void addFront(const Movie& movie);
    // Adds the movie at the end of the database
    void addBack(const Movie& movie);
    // Removes the movie at the start of the database, or throws an exception
    // if the database is empty
    void removeFront();
    // Removes the movie at the end of the database, or throws an exception
    // if the database is empty
    void removeBack();

    // Appends all movies from otherDB that match the given predicate
    void append(const MovieDatabase& otherDB, std::function<bool(const Movie&)> predicate);

    // Loads movie data from a CSV file and appends them to the database
    void load(const std::string& filename);

    // Starts a new series of database operations with a "cursor" placed immediately
    // before the first element
    void beginTransaction();
    // Moves the transaction cursor forward, or throws an exception if the
    // transactions haven't started or the cursor is already past the last element
    void next();
    // Moves the transaction cursor backward, or throws an exception if the
    // transactions haven't started or the cursor is already before the first element
    void previous();
    // Inserts a new movie after the transaction cursor and positions the cursor at
    // the new movie, or throws an exception if the transactions haven't started
    // or the cursor is past the last element
    void insert(const Movie& movie);
    // Removes the movie at the transaction cursor, or throws an exception if the
    // transactions haven't started or the cursor isn't at a valid movie. After
    // removal, the cursor should be pointing to the element that comes next (after
    // the one that was removed).
    void remove();
    // Returns the movie at the transaction cursor, or throws an exception if the
    // transactions haven't started yet or the cursor isn't at a valid movie
    const Movie& getCurrent() const;
    // Ends the series of database operations
    void endTransaction();
};
