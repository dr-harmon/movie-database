# Movie Database

For this assignment, you are to implement an in-memory database of Hollywood movies. Sample data is provided in the `HollywoodMovies.csv` file.

## Linked List Operations

The database should be implemented as a class called `MovieDatabase` with the following operations:

* isEmpty
* getSize
* getFront
* getBack
* addFront
* addBack
* removeFront
* removeBack

All of these operations should take constant time (`O(1)`) and be implemented as a linked list within `MovieDatabase`. You are _not_ allowed to use STL containers such as `vector` or `list`.

## Bulk Add Operations

The database should also be able to import batches of data using these methods:

* load: adds all entries from a CSV file of movies (similar to the "Dynamic Memory" lab)
* append: adds all entries from another database, filtering by a predicate (similar to the "Linked List Filter" lab)

Each bulk operation should take linear time (`O(n)`).

## Database Transactions

The database should also support a "transaction mode" that creates an internal cursor (similar to the cursor of a text editor) that points to an entry in the database. After starting transaction mode by calling `beginTransaction`, clients can move the cursor forward and back, add and delete entries where the cursor is, and retrieve the entry at the cursor. All of these operations should take constant time (`O(1)`).

It is legal to move the cursor one step before the first entry and one step after the last one. However, the only operation allowed in these cases is to insert a new entry when the cursor is before the first (or when there are no entries). Otherwise, an exception should be thrown.

Clients should end transaction mode by calling `endTransaction`. Any transaction operations that occur when not in transaction mode should cause an exception to be thrown.

For more details on how to implement transaction mode, refer to the source code comments in `moviedb.h` and the test cases in `moviedb_test.cpp`.

## Hint: CSV Reading

For the `load` function, you should use the provided `CSVReader` class in `csv.h`. Although the sample data in `HollywoodMovies.csv` has many fields, the database only needs two: title and year. Here is an example:

    io::CSVReader<2> in(filename);
    in.read_header(io::ignore_extra_column, "Title", "Year");
    string title;
    int year;
    while (in.read_row(title, year)) {
        // ...
    }

For testing purposes, the `MOVIEDB_PATH` string variable holds the file path to `HollywoodMovies.csv`. See the unit tests for an example of how it can be used.

## Hint: Code Reuse

Many of the methods in the `MovieDatabase` class can be implemented in terms of some reusable helper method. For example, `removeFront` and `removeBack` can be implemented as a call to the same shared function with different parameters.
