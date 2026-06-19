// books_analysis.cpp
//
// Beginner C++ project: reads books.csv, computes summary statistics,
// and rewrites the JavaScript "books" array inside index.html so the
// website always reflects whatever is in books.csv.
//
// HOW IT WORKS:
//   1. Reads books.csv
//   2. Builds the JS array text:  const books = [ ... ];
//   3. Opens index.html as plain text
//   4. Replaces everything between the markers:
//        /* AUTO-GENERATED-START ... */
//        ...
//        /* AUTO-GENERATED-END */
//      with the freshly generated array
//   5. Saves index.html back to disk
//   6. Also prints the same console analysis as before, so you still
//      get readable output when you just want to check the numbers.
//
// Compile:   g++ -std=c++17 -o books_analysis books_analysis.cpp
// Run:       ./books_analysis
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>

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
    fields.push_back(field);
    return fields;
}

// Escapes characters that would break a JS string literal (e.g. quotes)
string escapeForJS(const string& s) {
    string out;
    for (char c : s) {
        if (c == '"') out += "\\\"";
        else out += c;
    }
    return out;
}

vector<Book> loadBooks(const string& filename) {
    vector<Book> books;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "ERROR: Could not open '" << filename << "'." << endl;
        cerr << "Make sure books.csv is in the same folder as the executable." << endl;
        exit(1);
    }

    string line;
    bool isHeader = true;

    while (getline(file, line)) {
        if (line.empty()) continue;
        if (isHeader) { isHeader = false; continue; }

        vector<string> f = splitCSVLine(line);
        if (f.size() < 7) continue;

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

// Builds the full "const books = [...]" JS block as a string
string buildBooksJsBlock(const vector<Book>& books) {
    ostringstream out;
    out << "const books = [\n";
    for (size_t i = 0; i < books.size(); ++i) {
        const Book& b = books[i];
        out << "  [\"" << escapeForJS(b.name) << "\",\""
            << escapeForJS(b.author) << "\","
            << b.rating << ","
            << b.reviews << ","
            << (int)b.price << ","
            << b.year << ",\""
            << escapeForJS(b.genre) << "\"]";
        if (i != books.size() - 1) out << ",";
        out << "\n";
    }
    out << "];";
    return out.str();
}

// Reads index.html, replaces the auto-generated block, writes it back
void updateHtmlFile(const string& htmlPath, const string& newBooksBlock) {
    ifstream inFile(htmlPath);
    if (!inFile.is_open()) {
        cerr << "ERROR: Could not open '" << htmlPath << "' to update it." << endl;
        cerr << "Make sure index.html is in the same folder as the executable." << endl;
        exit(1);
    }

    string startMarker = "/* AUTO-GENERATED-START";
    string endMarker   = "/* AUTO-GENERATED-END */";

    ostringstream html;
    string line;
    bool insideBlock = false;
    bool foundBlock = false;

    while (getline(inFile, line)) {
        if (!insideBlock && line.find(startMarker) != string::npos) {
            insideBlock = true;
            foundBlock = true;
            html << line << "\n";          // keep the start-marker comment line
            html << newBooksBlock << "\n";  // insert the freshly generated array
            continue;
        }
        if (insideBlock) {
            if (line.find(endMarker) != string::npos) {
                insideBlock = false;
                html << line << "\n";       // keep the end-marker comment line
            }
            continue; // skip old array lines while inside the block
        }
        html << line << "\n";
    }
    inFile.close();

    if (!foundBlock) {
        cerr << "WARNING: Could not find the AUTO-GENERATED markers in "
             << htmlPath << ". index.html was NOT modified." << endl;
        cerr << "Make sure index.html still has the "
             << "'/* AUTO-GENERATED-START ... AUTO-GENERATED-END */' comment block." << endl;
        exit(1);
    }

    ofstream outFile(htmlPath, ios::trunc);
    outFile << html.str();
    outFile.close();

    cout << "index.html updated successfully with " << "fresh book data." << endl;
}

int main() {
    vector<Book> books = loadBooks("books.csv");

    if (books.empty()) {
        cerr << "No data loaded. Exiting." << endl;
        return 1;
    }

    // ---------------------------------------------------------
    // CONSOLE ANALYSIS (same as before, useful for quick checks)
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

    vector<Book> byRating = books;
    sort(byRating.begin(), byRating.end(), [](const Book& a, const Book& b) {
        return a.rating > b.rating;
    });

    cout << "\nTOP 5 HIGHEST RATED:" << endl;
    for (int i = 0; i < 5 && i < (int)byRating.size(); ++i) {
        cout << "  " << byRating[i].name << " (" << byRating[i].rating << ")" << endl;
    }

    vector<Book> byReviews = books;
    sort(byReviews.begin(), byReviews.end(), [](const Book& a, const Book& b) {
        return a.reviews > b.reviews;
    });

    cout << "\nTOP 5 MOST REVIEWED:" << endl;
    for (int i = 0; i < 5 && i < (int)byReviews.size(); ++i) {
        cout << "  " << byReviews[i].name << " (" << byReviews[i].reviews << " reviews)" << endl;
    }

    map<string, double> genreRatingSum;
    map<string, int> genreCount;
    for (const auto& b : books) {
        genreRatingSum[b.genre] += b.rating;
        genreCount[b.genre] += 1;
    }

    cout << "\nAVERAGE RATING BY GENRE:" << endl;
    for (const auto& entry : genreRatingSum) {
        cout << "  " << entry.first << ": "
             << (entry.second / genreCount[entry.first]) << endl;
    }

    // ---------------------------------------------------------
    // UPDATE index.html WITH FRESH DATA
    // ---------------------------------------------------------
    string booksBlock = buildBooksJsBlock(books);
    updateHtmlFile("index.html", booksBlock);

    cout << "\nDONE! Open index.html in your browser to see the updated website." << endl;

    return 0;
}
