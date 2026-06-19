// books_analysis.cpp
// Beginner C++ project: reads books.csv and prints analysis using plain C++.
//
// Compile:   g++ -std=c++17 -o books_analysis books_analysis.cpp
// Run:       ./books_analysis

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

// One row of the dataset
struct Book {
    string name;
    string author;
    double rating;
    int reviews;
    double price;
    int year;
    string genre;
};

// Splits a CSV line into fields, respecting simple double-quoted fields
// (handles commas inside quotes, e.g. "Smith, John")
vector<string> splitCSVLine(const string& line) {
    vector<string> fields;
    string field;
    bool inQuotes = false;

    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            fields.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }
    fields.push_back(field); // last field
    return fields;
}

// Reads the CSV file into a vector of Book records
vector<Book> loadBooks(const string& filename) {
    vector<Book> books;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "ERROR: Could not open file '" << filename << "'." << endl;
        cerr << "Make sure books.csv is in the same folder as the executable." << endl;
        exit(1);
    }

    string line;
    bool isHeader = true;

    while (getline(file, line)) {
        if (line.empty()) continue;

        if (isHeader) {
            isHeader = false; // skip the header row (Name,Author,User Rating,...)
            continue;
        }

        vector<string> f = splitCSVLine(line);
        if (f.size() < 7) continue; // skip malformed rows

        Book b;
        b.name    = f[0];
        b.author  = f[1];
        b.rating  = stod(f[2]);
        b.reviews = stoi(f[3]);
        b.price   = stod(f[4]);
        b.year    = stoi(f[5]);
        b.genre   = f[6];

        books.push_back(b);
    }

    return books;
}

void printSeparator() {
    cout << string(50, '=') << endl;
}

int main() {
    vector<Book> books = loadBooks("books.csv");

    if (books.empty()) {
        cerr << "No data loaded. Exiting." << endl;
        return 1;
    }

    // ---------------------------------------------------------
    // OVERVIEW
    // ---------------------------------------------------------
    printSeparator();
    cout << "BOOKS DATASET OVERVIEW" << endl;
    printSeparator();

    int minYear = books[0].year, maxYear = books[0].year;
    double ratingSum = 0.0, priceSum = 0.0;

    for (const auto& b : books) {
        minYear = min(minYear, b.year);
        maxYear = max(maxYear, b.year);
        ratingSum += b.rating;
        priceSum += b.price;
    }

    double avgRating = ratingSum / books.size();
    double avgPrice = priceSum / books.size();

    cout << "Total Books    : " << books.size() << endl;
    cout << "Years Covered  : " << minYear << " - " << maxYear << endl;
    cout << fixed << setprecision(2);
    cout << "Average Rating : " << avgRating << endl;
    cout << "Average Price  : $" << avgPrice << endl;

    // ---------------------------------------------------------
    // TOP 5 HIGHEST RATED
    // ---------------------------------------------------------
    vector<Book> byRating = books;
    sort(byRating.begin(), byRating.end(), [](const Book& a, const Book& b) {
        return a.rating > b.rating;
    });

    cout << "\nTOP 5 HIGHEST RATED:" << endl;
    cout << left << setw(40) << "Name"
         << right << setw(10) << "Rating"
         << setw(8) << "Year" << endl;
    for (int i = 0; i < 5 && i < (int)byRating.size(); ++i) {
        cout << left << setw(40) << byRating[i].name
             << right << setw(10) << fixed << setprecision(1) << byRating[i].rating
             << setw(8) << byRating[i].year << endl;
    }

    // ---------------------------------------------------------
    // TOP 5 MOST REVIEWED
    // ---------------------------------------------------------
    vector<Book> byReviews = books;
    sort(byReviews.begin(), byReviews.end(), [](const Book& a, const Book& b) {
        return a.reviews > b.reviews;
    });

    cout << "\nTOP 5 MOST REVIEWED:" << endl;
    cout << left << setw(40) << "Name"
         << right << setw(12) << "Reviews" << endl;
    for (int i = 0; i < 5 && i < (int)byReviews.size(); ++i) {
        cout << left << setw(40) << byReviews[i].name
             << right << setw(12) << byReviews[i].reviews << endl;
    }

    // ---------------------------------------------------------
    // AVERAGE RATING BY GENRE
    // ---------------------------------------------------------
    map<string, double> genreRatingSum;
    map<string, int> genreCount;

    for (const auto& b : books) {
        genreRatingSum[b.genre] += b.rating;
        genreCount[b.genre] += 1;
    }

    cout << "\nAVERAGE RATING BY GENRE:" << endl;
    for (const auto& entry : genreRatingSum) {
        const string& genre = entry.first;
        double avg = entry.second / genreCount[genre];
        cout << left << setw(15) << genre
             << right << setw(8) << fixed << setprecision(2) << avg << endl;
    }

    // ---------------------------------------------------------
    // BOOKS BY YEAR (2024 and 2025, like the Python script)
    // ---------------------------------------------------------
    auto printBooksForYear = [&](int targetYear) {
        cout << "\n" << targetYear << " BOOKS:" << endl;
        cout << left << setw(38) << "Name"
             << setw(22) << "Author"
             << right << setw(8) << "Rating" << endl;

        bool found = false;
        for (const auto& b : books) {
            if (b.year == targetYear) {
                found = true;
                cout << left << setw(38) << b.name
                     << setw(22) << b.author
                     << right << setw(8) << fixed << setprecision(1) << b.rating << endl;
            }
        }
        if (!found) {
            cout << "(no books found for this year)" << endl;
        }
    };

    printBooksForYear(2024);
    printBooksForYear(2025);

    cout << "\nDONE! (Note: chart generation was skipped in the C++ version \n"
         << "-- see the Python version for visual charts.)" << endl;

    return 0;
}
