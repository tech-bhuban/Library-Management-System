

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <ctime>
#include <iomanip>
#include <algorithm>

class Book {
private:
    std::string ISBN;
    std::string title;
    std::string author;
    std::string genre;
    int publicationYear;
    int totalCopies;
    int availableCopies;
    double rating;
    int timesBorrowed;
    
public:
    Book(const std::string& isbn, const std::string& t, const std::string& a,
         const std::string& g, int year, int copies)
        : ISBN(isbn), title(t), author(a), genre(g), 
          publicationYear(year), totalCopies(copies), 
          availableCopies(copies), rating(0.0), timesBorrowed(0) {}
    
    // Getters
    std::string getISBN() const { return ISBN; }
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    std::string getGenre() const { return genre; }
    int getYear() const { return publicationYear; }
    int getAvailable() const { return availableCopies; }
    int getTotal() const { return totalCopies; }
    double getRating() const { return rating; }
    int getTimesBorrowed() const { return timesBorrowed; }
    
    // Operations
    bool borrowBook() {
        if (availableCopies > 0) {
            availableCopies--;
            timesBorrowed++;
            return true;
        }
        return false;
    }
    
    bool returnBook() {
        if (availableCopies < totalCopies) {
            availableCopies++;
            return true;
        }
        return false;
    }
    
    void addRating(double newRating) {
        // Simple average calculation
        rating = (rating * timesBorrowed + newRating) / (timesBorrowed + 1);
    }
    
    void display() const {
        std::cout << "\n" << std::string(60, '=') << "\n";
        std::cout << "📚 " << title << "\n";
        std::cout << std::string(60, '-') << "\n";
        std::cout << std::left << std::setw(15) << "ISBN:" << ISBN << "\n";
        std::cout << std::setw(15) << "Author:" << author << "\n";
        std::cout << std::setw(15) << "Genre:" << genre << "\n";
        std::cout << std::setw(15) << "Year:" << publicationYear << "\n";
        std::cout << std::setw(15) << "Copies:" << availableCopies << "/" << totalCopies << "\n";
        std::cout << std::setw(15) << "Rating:" << std::fixed << std::setprecision(1) 
                 << rating << "/5.0\n";
        std::cout << std::setw(15) << "Times Borrowed:" << timesBorrowed << "\n";
        
        std::cout << "Status: ";
        if (availableCopies == 0) {
            std::cout << "❌ Currently Unavailable\n";
        } else if (availableCopies < totalCopies / 2) {
            std::cout << "⚠️  Limited Copies\n";
        } else {
            std::cout << "✅ Available\n";
        }
        std::cout << std::string(60, '=') << "\n";
    }
};

class Borrower {
private:
    std::string borrowerID;
    std::string name;
    std::string email;
    int maxBooks;
    int booksBorrowed;
    std::vector<std::string> borrowedBooks; // ISBNs
    time_t membershipDate;
    
public:
    Borrower(const std::string& id, const std::string& n, const std::string& e)
        : borrowerID(id), name(n), email(e), maxBooks(5), 
          booksBorrowed(0), membershipDate(time(0)) {}
    
    // Getters
    std::string getID() const { return borrowerID; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    int getBooksBorrowed() const { return booksBorrowed; }
    int getMaxBooks() const { return maxBooks; }
    
    // Operations
    bool canBorrow() const {
        return booksBorrowed < maxBooks;
    }
    
    bool borrowBook(const std::string& ISBN) {
        if (canBorrow()) {
            borrowedBooks.push_back(ISBN);
            booksBorrowed++;
            return true;
        }
        return false;
    }
    
    bool returnBook(const std::string& ISBN) {
        auto it = std::find(borrowedBooks.begin(), borrowedBooks.end(), ISBN);
        if (it != borrowedBooks.end()) {
            borrowedBooks.erase(it);
            booksBorrowed--;
            return true;
        }
        return false;
    }
    
    void display() const {
        std::cout << "\n" << std::string(50, '=') << "\n";
        std::cout << "👤 " << name << "\n";
        std::cout << std::string(50, '-') << "\n";
        std::cout << std::left << std::setw(15) << "Borrower ID:" << borrowerID << "\n";
        std::cout << std::setw(15) << "Email:" << email << "\n";
        std::cout << std::setw(15) << "Books Borrowed:" << booksBorrowed << "/" << maxBooks << "\n";
        
        // Calculate membership duration
        time_t now = time(0);
        double days = difftime(now, membershipDate) / (60 * 60 * 24);
        
        std::cout << std::setw(15) << "Member for:" << int(days) << " days\n";
        
        std::cout << "Status: ";
        if (booksBorrowed >= maxBooks) {
            std::cout << "❌ Cannot borrow more books\n";
        } else if (booksBorrowed >= maxBooks - 1) {
            std::cout << "⚠️  Only " << (maxBooks - booksBorrowed) << " book(s) left\n";
        } else {
            std::cout << "✅ Can borrow " << (maxBooks - booksBorrowed) << " more book(s)\n";
        }
        std::cout << std::string(50, '=') << "\n";
    }
};

class Library {
private:
    std::map<std::string, Book> books;
    std::map<std::string, Borrower> borrowers;
    int totalTransactions;
    
public:
    Library() : totalTransactions(0) {}
    
    void addBook(const Book& book) {
        books[book.getISBN()] = book;
    }
    
    void addBorrower(const Borrower& borrower) {
        borrowers[borrower.getID()] = borrower;
    }
    
    bool borrowBook(const std::string& borrowerID, const std::string& ISBN) {
        if (borrowers.find(borrowerID) == borrowers.end()) {
            std::cout << "Borrower not found!\n";
            return false;
        }
        
        if (books.find(ISBN) == books.end()) {
            std::cout << "Book not found!\n";
            return false;
        }
        
        Borrower& borrower = borrowers[borrowerID];
        Book& book = books[ISBN];
        
        if (!borrower.canBorrow()) {
            std::cout << "Borrower has reached maximum book limit!\n";
            return false;
        }
        
        if (!book.borrowBook()) {
            std::cout << "Book is not available!\n";
            return false;
        }
        
        if (borrower.borrowBook(ISBN)) {
            totalTransactions++;
            std::cout << "✅ Book borrowed successfully!\n";
            return true;
        }
        
        return false;
    }
    
    bool returnBook(const std::string& borrowerID, const std::string& ISBN) {
        if (borrowers.find(borrowerID) == borrowers.end()) {
            std::cout << "Borrower not found!\n";
            return false;
        }
        
        if (books.find(ISBN) == books.end()) {
            std::cout << "Book not found!\n";
            return false;
        }
        
        Borrower& borrower = borrowers[borrowerID];
        Book& book = books[ISBN];
        
        if (borrower.returnBook(ISBN) && book.returnBook()) {
            totalTransactions++;
            
            // Ask for rating
            double rating;
            std::cout << "Rate this book (1.0-5.0): ";
            std::cin >> rating;
            if (rating >= 1.0 && rating <= 5.0) {
                book.addRating(rating);
            }
            
            std::cout << "✅ Book returned successfully!\n";
            return true;
        }
        
        std::cout << "This borrower didn't borrow this book!\n";
        return false;
    }
    
    void searchBook(const std::string& query) {
        std::cout << "\n=== SEARCH RESULTS ===\n";
        bool found = false;
        
        for (const auto& pair : books) {
            const Book& book = pair.second;
            if (book.getTitle().find(query) != std::string::npos ||
                book.getAuthor().find(query) != std::string::npos ||
                book.getISBN().find(query) != std::string::npos ||
                book.getGenre().find(query) != std::string::npos) {
                book.display();
                found = true;
            }
        }
        
        if (!found) {
            std::cout << "No books found matching: " << query << "\n";
        }
    }
    
    void displayAllBooks() {
        std::cout << "\n=== LIBRARY CATALOG (" << books.size() << " books) ===\n";
        
        std::cout << std::left << std::setw(15) << "ISBN"
                  << std::setw(30) << "Title"
                  << std::setw(20) << "Author"
                  << std::setw(10) << "Available"
                  << std::setw(8) << "Rating\n";
        std::cout << std::string(93, '-') << "\n";
        
        for (const auto& pair : books) {
            const Book& book = pair.second;
            std::cout << std::left << std::setw(15) << book.getISBN().substr(0, 14)
                      << std::setw(30) << (book.getTitle().length() > 28 ? 
                          book.getTitle().substr(0, 27) + "." : book.getTitle())
                      << std::setw(20) << (book.getAuthor().length() > 18 ? 
                          book.getAuthor().substr(0, 17) + "." : book.getAuthor())
                      << std::right << std::setw(3) << book.getAvailable() << "/" 
                      << std::left << std::setw(4) << book.getTotal()
                      << std::right << std::setw(7) << std::fixed << std::setprecision(1) 
                      << book.getRating() << "\n";
        }
    }
    
    void displayStatistics() {
        std::cout << "\n=== LIBRARY STATISTICS ===\n";
        std::cout << "Total Books: " << books.size() << "\n";
        std::cout << "Total Borrowers: " << borrowers.size() << "\n";
        std::cout << "Total Transactions: " << totalTransactions << "\n";
        
        if (!books.empty()) {
            // Find most popular book
            const Book* mostPopular = &books.begin()->second;
            int maxBorrows = 0;
            
            // Genre distribution
            std::map<std::string, int> genreCount;
            
            for (const auto& pair : books) {
                const Book& book = pair.second;
                genreCount[book.getGenre()]++;
                
                if (book.getTimesBorrowed() > maxBorrows) {
                    mostPopular = &book;
                    maxBorrows = book.getTimesBorrowed();
                }
            }
            
            std::cout << "\nMost Popular Book: " << mostPopular->getTitle() 
                     << " (" << maxBorrows << " borrows)\n";
            
            std::cout << "\nGenre Distribution:\n";
            for (const auto& pair : genreCount) {
                double percentage = (static_cast<double>(pair.second) / books.size()) * 100;
                std::cout << "- " << pair.first << ": " << pair.second 
                         << " books (" << std::fixed << std::setprecision(1) 
                         << percentage << "%)\n";
            }
            
            // Availability statistics
            int availableBooks = 0;
            for (const auto& pair : books) {
                if (pair.second.getAvailable() > 0) {
                    availableBooks++;
                }
            }
            
            double availabilityRate = (static_cast<double>(availableBooks) / books.size()) * 100;
            std::cout << "\nAvailability Rate: " << std::fixed << std::setprecision(1) 
                     << availabilityRate << "%\n";
        }
    }
    
    void displayOverdueBooks() {
        // Simple overdue simulation
        std::cout << "\n=== OVERDUE BOOKS SIMULATION ===\n";
        std::cout << "Note: This is a simulation for demonstration.\n";
        std::cout << "In a real system, this would track actual due dates.\n\n";
        
        int overdueCount = 0;
        for (const auto& borrowerPair : borrowers) {
            const Borrower& borrower = borrowerPair.second;
            if (borrower.getBooksBorrowed() > 3) {
                std::cout << borrower.getName() << " has " 
                         << borrower.getBooksBorrowed() 
                         << " books (might have overdue)\n";
                overdueCount++;
            }
        }
        
        if (overdueCount == 0) {
            std::cout << "✅ No potential overdue books detected.\n";
        }
    }
};

// Sample data
void initializeLibrary(Library& lib) {
    // Add sample books
    lib.addBook(Book("978-3-16-148410-0", "The Great Gatsby", "F. Scott Fitzgerald", 
                    "Classic", 1925, 5));
    lib.addBook(Book("978-0-7432-7356-5", "To Kill a Mockingbird", "Harper Lee", 
                    "Fiction", 1960, 3));
    lib.addBook(Book("978-0-14-028333-4", "1984", "George Orwell", 
                    "Dystopian", 1949, 4));
    lib.addBook(Book("978-0-679-74539-9", "Harry Potter and the Philosopher's Stone", 
                    "J.K. Rowling", "Fantasy", 1997, 8));
    lib.addBook(Book("978-1-101-14853-0", "The Hobbit", "J.R.R. Tolkien", 
                    "Fantasy", 1937, 3));
    lib.addBook(Book("978-0-307-47443-8", "The Da Vinci Code", "Dan Brown", 
                    "Mystery", 2003, 6));
    
    // Add sample borrowers
    lib.addBorrower(Borrower("B001", "Alice Johnson", "alice@email.com"));
    lib.addBorrower(Borrower("B002", "Bob Smith", "bob@email.com"));
    lib.addBorrower(Borrower("B003", "Charlie Brown", "charlie@email.com"));
}

int main() {
    Library library;
    initializeLibrary(library);
    
    int choice;
    do {
        std::cout << "\n=== LIBRARY MANAGEMENT SYSTEM ===\n";
        std::cout << "1. View All Books\n";
        std::cout << "2. Search Book\n";
        std::cout << "3. Borrow Book\n";
        std::cout << "4. Return Book\n";
        std::cout << "5. View Library Statistics\n";
        std::cout << "6. Check Overdue Books\n";
        std::cout << "7. Exit\n";
        std::cout << "Choice: ";
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice == 1) {
            library.displayAllBooks();
        } else if (choice == 2) {
            std::string query;
            std::cout << "Enter search query (title/author/ISBN/genre): ";
            std::getline(std::cin, query);
            library.searchBook(query);
        } else if (choice == 3) {
            std::string borrowerID, ISBN;
            std::cout << "Enter Borrower ID: ";
            std::getline(std::cin, borrowerID);
            std::cout << "Enter Book ISBN: ";
            std::getline(std::cin, ISBN);
            library.borrowBook(borrowerID, ISBN);
        } else if (choice == 4) {
            std::string borrowerID, ISBN;
            std::cout << "Enter Borrower ID: ";
            std::getline(std::cin, borrowerID);
            std::cout << "Enter Book ISBN: ";
            std::getline(std::cin, ISBN);
            library.returnBook(borrowerID, ISBN);
        } else if (choice == 5) {
            library.displayStatistics();
        } else if (choice == 6) {
            library.displayOverdueBooks();
        }
    } while (choice != 7);
    
    return 0;
}

