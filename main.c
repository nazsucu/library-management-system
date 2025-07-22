#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define Book struct to hold book information
typedef struct {
    char title[100];
    char author[100];
    int isBorrowed; // 0 = available, 1 = borrowed
} Book;

#define MAX_BOOKS 100  // Maximum number of books allowed

// Load books from "books.txt" file into the books array
int loadBooks(Book books[], int maxBooks) {
    FILE *fp = fopen("books.txt", "r");  // Open file for reading
    if (!fp) return 0;                   // Return 0 if file can't be opened
    int count = 0;
    // Read books line by line: title;author;isBorrowed
    while (count < maxBooks && fscanf(fp, "%99[^;];%99[^;];%d\n",
           books[count].title, books[count].author, &books[count].isBorrowed) == 3) {
        count++;
    }
    fclose(fp);  // Close the file after reading
    return count; // Return number of books loaded
}

// Save the books array back to "books.txt" file
void saveBooks(Book books[], int count) {
    FILE *fp = fopen("books.txt", "w");  // Open file for writing (overwrite)
    if (!fp) {
        printf("Cannot save books!\n");
        return;
    }
    // Write each book's data to the file in the format: title;author;isBorrowed
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s;%s;%d\n", books[i].title, books[i].author, books[i].isBorrowed);
    }
    fclose(fp);  // Close the file after writing
}

// Display the list of books with their status
void listBooks(Book books[], int count) {
    printf("\n--- Library Books ---\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s by %s [%s]\n", i + 1, books[i].title, books[i].author,
               books[i].isBorrowed ? "Borrowed" : "Available");
    }
    printf("---------------------\n");
}

// Add a new book to the collection
void addBook(Book books[], int *count) {
    if (*count >= MAX_BOOKS) {
        printf("Library is full!\n");
        return;
    }
    Book newBook;
    printf("Enter book title: ");
    fgets(newBook.title, sizeof(newBook.title), stdin);
    newBook.title[strcspn(newBook.title, "\n")] = 0; // Remove trailing newline

    printf("Enter author name: ");
    fgets(newBook.author, sizeof(newBook.author), stdin);
    newBook.author[strcspn(newBook.author, "\n")] = 0; // Remove trailing newline

    newBook.isBorrowed = 0;  // Set book as available

    books[*count] = newBook; // Add book to array
    (*count)++;              // Increase book count
    printf("Book added successfully.\n");
}

// Search books by title or author keyword (case insensitive)
void searchBooks(Book books[], int count) {
    char searchTerm[100];
    int found = 0;
    printf("Enter search keyword (title or author): ");
    fgets(searchTerm, sizeof(searchTerm), stdin);
    searchTerm[strcspn(searchTerm, "\n")] = 0;  // Remove trailing newline

    printf("\n--- Search Results ---\n");
    for (int i = 0; i < count; i++) {
        // Check if searchTerm is in title or author
        if (strcasestr(books[i].title, searchTerm) != NULL || strcasestr(books[i].author, searchTerm) != NULL) {
            printf("%d. %s by %s [%s]\n", i + 1, books[i].title, books[i].author,
                   books[i].isBorrowed ? "Borrowed" : "Available");
            found = 1;
        }
    }
    if (!found) {
        printf("No matching books found.\n");
    }
    printf("---------------------\n");
}

// Borrow a book if it's available
void borrowBook(Book books[], int count) {
    int choice;
    listBooks(books, count);  // Show all books
    printf("Enter book number to borrow: ");
    scanf("%d", &choice);
    getchar(); // Clear newline from input buffer
    if (choice < 1 || choice > count) {
        printf("Invalid book number.\n");
        return;
    }
    if (books[choice - 1].isBorrowed) {
        printf("Book is already borrowed.\n");
    } else {
        books[choice - 1].isBorrowed = 1;  // Mark as borrowed
        printf("You borrowed \"%s\".\n", books[choice - 1].title);
    }
}

// Return a borrowed book
void returnBook(Book books[], int count) {
    int choice;
    listBooks(books, count);  // Show all books
    printf("Enter book number to return: ");
    scanf("%d", &choice);
    getchar(); // Clear newline from input buffer
    if (choice < 1 || choice > count) {
        printf("Invalid book number.\n");
        return;
    }
    if (!books[choice - 1].isBorrowed) {
        printf("This book was not borrowed.\n");
    } else {
        books[choice - 1].isBorrowed = 0;  // Mark as available
        printf("You returned \"%s\".\n", books[choice - 1].title);
    }
}

// Delete a book from the collection
void deleteBook(Book books[], int *count) {
    int choice;
    listBooks(books, *count);  // Show all books
    printf("Enter book number to delete: ");
    scanf("%d", &choice);
    getchar(); // Clear newline from input buffer
    if (choice < 1 || choice > *count) {
        printf("Invalid book number.\n");
        return;
    }
    // Shift all books after the deleted one to the left
    for (int i = choice - 1; i < *count - 1; i++) {
        books[i] = books[i + 1];
    }
    (*count)--;  // Decrease book count
    printf("Book deleted successfully.\n");
}

// Update the title and/or author of a book
void updateBook(Book books[], int count) {
    int choice;
    printf("Enter book number to update: ");
    scanf("%d", &choice);
    getchar(); // Clear newline from input buffer
    if (choice < 1 || choice > count) {
        printf("Invalid book number.\n");
        return;
    }
    Book *book = &books[choice - 1];

    printf("Enter new title (leave empty to keep \"%s\"): ", book->title);
    char newTitle[100];
    fgets(newTitle, sizeof(newTitle), stdin);
    if (newTitle[0] != '\n') {
        newTitle[strcspn(newTitle, "\n")] = 0;  // Remove trailing newline
        strcpy(book->title, newTitle);          // Update title
    }

    printf("Enter new author (leave empty to keep \"%s\"): ", book->author);
    char newAuthor[100];
    fgets(newAuthor, sizeof(newAuthor), stdin);
    if (newAuthor[0] != '\n') {
        newAuthor[strcspn(newAuthor, "\n")] = 0;  // Remove trailing newline
        strcpy(book->author, newAuthor);          // Update author
    }
    printf("Book updated successfully.\n");
}

int main() {
    Book books[MAX_BOOKS];
    int bookCount = loadBooks(books, MAX_BOOKS);  // Load books from file

    int choice;
    do {
        // Display menu options
        printf("\n--- Library Menu ---\n");
        printf("1. Add Book\n");
        printf("2. List Books\n");
        printf("3. Search Books\n");
        printf("4. Borrow Book\n");
        printf("5. Return Book\n");
        printf("6. Delete Book\n");
        printf("7. Update Book\n");
        printf("8. Exit\n");
        printf("Your choice: ");
        scanf("%d", &choice);
        getchar();  // Clear newline from input buffer

        // Process user choice
        switch (choice) {
            case 1:
                addBook(books, &bookCount);
                saveBooks(books, bookCount);  // Save changes to file
                break;
            case 2:
                listBooks(books, bookCount);
                break;
            case 3:
                searchBooks(books, bookCount);
                break;
            case 4:
                borrowBook(books, bookCount);
                saveBooks(books, bookCount);
                break;
            case 5:
                returnBook(books, bookCount);
                saveBooks(books, bookCount);
                break;
            case 6:
                deleteBook(books, &bookCount);
                saveBooks(books, bookCount);
                break;
            case 7:
                updateBook(books, bookCount);
                saveBooks(books, bookCount);
                break;
            case 8:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 8);

    return 0;
}
