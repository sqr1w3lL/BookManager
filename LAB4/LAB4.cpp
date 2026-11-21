#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

istream& insetup(istream& in) {
    in >> ws;
    return in;
}

ostream& outsetup(ostream& out) {
    out << fixed << uppercase << right;
    return out;
}

struct Author {
    string name;
    int books_count;
    int birth_year;

    Author() : name(""), books_count(0), birth_year(0) {}
};


class Book {
private:
    string title;
    string publisher;
    int year;
    Author* authors;
    int authors_count;

public:
    Book() : title(""), publisher(""), year(0), authors(nullptr), authors_count(0) {}

    Book(const Book& other) : title(other.title), publisher(other.publisher), year(other.year), authors_count(other.authors_count) {
        if (authors_count > 0) {
            authors = new Author[authors_count];
            for (int i = 0; i < authors_count; i++) {
                authors[i] = other.authors[i];
            }
        }
        else {
            authors = nullptr;
        }
    }

    ~Book() {
        delete[] authors;
    }

    void input(istream& in) {
        cout << "Enter title: ";
        getline(insetup(in), title);
        cout << "Enter publisher: ";
        getline(insetup(in), publisher);
        cout << "Enter year: ";
        in >> year;
        in.ignore();
    }

    void display(ostream& out) const {
        outsetup(out);
        out << "Title: " << title << "\nPublisher: " << publisher << "\nYear: " << year << "\nAuthors:\n";
        if (authors_count == 0) {
            out << "  No authors added.\n";
            return;
        }
        for (int i = 0; i < authors_count; i++) {
            out << "  "
                << setw(20) << authors[i].name << " | "
                << "Books: " << setw(3) << authors[i].books_count << " | "
                << "Born: " << setw(4) << authors[i].birth_year << "\n";
        }
    }

    void add_author(const string& name, int books, int birth) {
        Author* new_authors = new Author[authors_count + 1];
        for (int i = 0; i < authors_count; i++) {
            new_authors[i] = authors[i];
        }
        new_authors[authors_count] = { name, books, birth };
        delete[] authors;
        authors = new_authors;
        authors_count++;
    }

    void del_author(const string& name) {
        if (authors_count == 0) return;

        int index = -1;
        for (int i = 0; i < authors_count; i++) {
            if (authors[i].name == name) {
                index = i;
                break;
            }
        }
        if (index == -1) return;

        Author* new_authors = new Author[authors_count - 1];
        for (int i = 0, j = 0; i < authors_count; i++) {
            if (i != index) {
                new_authors[j++] = authors[i];
            }
        }
        delete[] authors;
        authors = new_authors;
        authors_count--;
    }

    int count_authors() const {
        return authors_count;
    }
};

int main() {
    Book book;
    book.input(cin);

    ofstream outFile("book_data.txt");
    if (!outFile) {
        cerr << "Error opening file for writing.\n";
        return 1;
    }

    int choice;
    do {
        cout << "\nMenu:\n";
        cout << "1 - Add author\n";
        cout << "2 - Delete author\n";
        cout << "3 - Count authors\n";
        cout << "4 - Show book details\n";
        cout << "5 - Save to file\n";
        cout << "6 - Exit\n";
        cout << "- Enter your choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string name;
            int books, birth;
            cout << "Enter author name: ";
            getline(insetup(cin), name);
            cout << "Enter number of books: ";
            cin >> books;
            cout << "Enter birth year: ";
            cin >> birth;
            cin.ignore();
            book.add_author(name, books, birth);
        }
        else if (choice == 2) {
            string name;
            cout << "Enter author name to delete: ";
            getline(insetup(cin), name);
            book.del_author(name);
        }
        else if (choice == 3) {
            cout << "Number of authors: " << book.count_authors() << "\n";
        }
        else if (choice == 4) {
            book.display(cout);
        }
        else if (choice == 5) {
            book.display(outFile);
            cout << "Data saved to book_data.txt\n";
        }
    } while (choice != 6);

    outFile.close();
    return 0;
}