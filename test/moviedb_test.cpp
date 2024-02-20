#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark_all.hpp>
#include "moviedb.h"

using namespace std;

Movie movie1 = {"Spider-Man 3", 2007};
Movie movie2 = {"Vampires Suck", 2010};
Movie movie3 = {"Lovelace", 2013};

const int MOVIESDB_SIZE = 963;

TEST_CASE("Database is initialized correctly")
{
    MovieDatabase db;
    REQUIRE(db.getSize() == 0);
    REQUIRE(db.isEmpty());
    REQUIRE_THROWS(db.getFront());
    REQUIRE_THROWS(db.getBack());
    REQUIRE_THROWS(db.removeFront());
    REQUIRE_THROWS(db.removeBack());
}

TEST_CASE("Movies can be prepended to the database")
{
    MovieDatabase db;

    db.addFront(movie1);
    REQUIRE(db.getSize() == 1);
    REQUIRE_FALSE(db.isEmpty());
    REQUIRE(db.getFront().title == movie1.title);
    REQUIRE(db.getBack().title == movie1.title);

    db.addFront(movie2);
    REQUIRE(db.getSize() == 2);
    REQUIRE_FALSE(db.isEmpty());
    REQUIRE(db.getFront().title == movie2.title);
    REQUIRE(db.getBack().title == movie1.title);

    db.addFront(movie3);
    REQUIRE(db.getSize() == 3);
    REQUIRE_FALSE(db.isEmpty());
    REQUIRE(db.getFront().title == movie3.title);
    REQUIRE(db.getBack().title == movie1.title);
}

TEST_CASE("Movies can be appended to the database")
{
    MovieDatabase db;

    db.addBack(movie1);
    REQUIRE(db.getSize() == 1);
    REQUIRE_FALSE(db.isEmpty());
    REQUIRE(db.getFront().title == movie1.title);
    REQUIRE(db.getBack().title == movie1.title);

    db.addBack(movie2);
    REQUIRE(db.getSize() == 2);
    REQUIRE_FALSE(db.isEmpty());
    REQUIRE(db.getFront().title == movie1.title);
    REQUIRE(db.getBack().title == movie2.title);

    db.addBack(movie3);
    REQUIRE(db.getSize() == 3);
    REQUIRE_FALSE(db.isEmpty());
    REQUIRE(db.getFront().title == movie1.title);
    REQUIRE(db.getBack().title == movie3.title);
}

TEST_CASE("Movies can be removed from the beginning of the database")
{
    MovieDatabase db;
    db.addBack(movie1);
    db.addBack(movie2);
    db.addBack(movie3);
    REQUIRE(db.getSize() == 3);
    REQUIRE_FALSE(db.isEmpty());
    REQUIRE(db.getFront().title == movie1.title);
    REQUIRE(db.getBack().title == movie3.title);

    db.removeFront();
    REQUIRE(db.getSize() == 2);
    REQUIRE_FALSE(db.isEmpty());
    REQUIRE(db.getFront().title == movie2.title);
    REQUIRE(db.getBack().title == movie3.title);

    db.removeFront();
    REQUIRE(db.getSize() == 1);
    REQUIRE_FALSE(db.isEmpty());
    REQUIRE(db.getFront().title == movie3.title);
    REQUIRE(db.getBack().title == movie3.title);

    db.removeFront();
    REQUIRE(db.getSize() == 0);
    REQUIRE(db.isEmpty());
}

TEST_CASE("Movies can be removed from the end of the database")
{
    MovieDatabase db;
    db.addBack(movie1);
    db.addBack(movie2);
    db.addBack(movie3);
    REQUIRE(db.getSize() == 3);
    REQUIRE_FALSE(db.isEmpty());
    REQUIRE(db.getFront().title == movie1.title);
    REQUIRE(db.getBack().title == movie3.title);

    db.removeBack();
    REQUIRE(db.getSize() == 2);
    REQUIRE_FALSE(db.isEmpty());
    REQUIRE(db.getFront().title == movie1.title);
    REQUIRE(db.getBack().title == movie2.title);

    db.removeBack();
    REQUIRE(db.getSize() == 1);
    REQUIRE_FALSE(db.isEmpty());
    REQUIRE(db.getFront().title == movie1.title);
    REQUIRE(db.getBack().title == movie1.title);

    db.removeFront();
    REQUIRE(db.getSize() == 0);
    REQUIRE(db.isEmpty());
}

TEST_CASE("A movie database can be appended to another")
{
    MovieDatabase oldDB;
    oldDB.load(MOVIEDB_PATH);

    MovieDatabase newDB;
    newDB.addBack(movie1);

    REQUIRE(newDB.getSize() == 1);
    newDB.append(oldDB, [](const Movie& movie) { return movie.year == 2013; });
    REQUIRE(newDB.getSize() == 160);
    REQUIRE(newDB.getFront().title == "Spider-Man 3");
    REQUIRE(newDB.getBack().title == "Lovelace");
}

TEST_CASE("Database can be loaded from a file")
{
    MovieDatabase db;
    db.load(MOVIEDB_PATH);
    REQUIRE(db.getSize() == MOVIESDB_SIZE);
    REQUIRE_FALSE(db.isEmpty());

    db.beginTransaction();
    db.next();
    for (int i = 1; i <= MOVIESDB_SIZE; i++) {
        switch (i) {
            case 1:
            REQUIRE(db.getCurrent().title == movie1.title);
            break;
            case 500:
            REQUIRE(db.getCurrent().title == movie2.title);
            break;
            case 963:
            REQUIRE(db.getCurrent().title == movie3.title);
            break;
        }
        db.next();
    }
    db.endTransaction();
}

TEST_CASE("Movies can be added in a transaction")
{
    MovieDatabase db;

    db.beginTransaction();
    REQUIRE(db.getSize() == 0);
    db.insert(movie1);
    REQUIRE(db.getSize() == 1);
    REQUIRE(db.getCurrent().title == movie1.title);
    db.insert(movie2);
    REQUIRE(db.getSize() == 2);
    REQUIRE(db.getCurrent().title == movie2.title);
    db.insert(movie3);
    REQUIRE(db.getSize() == 3);
    REQUIRE(db.getCurrent().title == movie3.title);
    db.previous();
    REQUIRE(db.getCurrent().title == movie2.title);
    db.previous();
    REQUIRE(db.getCurrent().title == movie1.title);
    db.next();
    REQUIRE(db.getCurrent().title == movie2.title);
    db.endTransaction();
}

TEST_CASE("Movies can be removed in a transaction")
{
    MovieDatabase db;
    db.addBack(movie1);
    db.addBack(movie2);
    db.addBack(movie3);

    db.beginTransaction();
    REQUIRE(db.getSize() == 3);
    db.next();
    db.next();
    REQUIRE(db.getCurrent().title == movie2.title);
    db.remove();
    REQUIRE(db.getSize() == 2);
    REQUIRE(db.getCurrent().title == movie3.title);
    db.remove();
    REQUIRE(db.getSize() == 1);
    db.previous();
    REQUIRE(db.getCurrent().title == movie1.title);
    db.remove();
    REQUIRE(db.getSize() == 0);
    db.endTransaction();
}

TEST_CASE("Transaction operations must be within a transaction")
{
    MovieDatabase db;
    REQUIRE_THROWS(db.next());
    REQUIRE_THROWS(db.previous());
    REQUIRE_THROWS(db.insert(movie1));
    REQUIRE_THROWS(db.remove());
    REQUIRE_THROWS(db.getCurrent());
}

TEST_CASE("Transaction operations must not occur at an invalid location")
{
    MovieDatabase db;
    db.addBack(movie1);

    db.beginTransaction();

    REQUIRE_NOTHROW(db.next());
    REQUIRE_NOTHROW(db.next());
    REQUIRE_THROWS(db.next());

    REQUIRE_NOTHROW(db.previous());
    REQUIRE_NOTHROW(db.previous());
    REQUIRE_THROWS(db.previous());

    REQUIRE_NOTHROW(db.next());
    REQUIRE_NOTHROW(db.next());
    REQUIRE_THROWS(db.insert(movie2));

    REQUIRE_THROWS(db.remove());
    REQUIRE_THROWS(db.getCurrent());
    db.previous();
    db.previous();
    REQUIRE_THROWS(db.remove());
    REQUIRE_THROWS(db.getCurrent());

    db.endTransaction();
}
