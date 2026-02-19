/* Ref class function definitions
 * Computer Science, MVNU
 * CSC-3004 Introduction to Software Development
 *
 * NOTE: You may add code to this file, but do not
 * delete any code or delete any comments.
 *
 * STUDENT NAME: Elijah Litman
 */

#include "Ref.h"
#include <iostream>
#include <string>
#include <fstream>

#include <cstring>
#include <cerrno>

using namespace std;

/* GetNextToken - general routine for getting next token from a string
 * Parameters
 *    str - string to search. search is destructive, i.e., token is removed.
 *    delimiters - string, a list of characters to use a delimiters between tokens
 * Returns: string token (removed from str)
 */
string GetNextToken(string& str, const string& delimiters = " ") {
   // Skip delimiters at beginning
   string::size_type startPos = str.find_first_not_of(delimiters, 0);

   // Find position of delimiter at end of token
   string::size_type endPos = str.find_first_of(delimiters, startPos);

   // Found a token, remove it from string, and return it
   string next = str.substr(startPos, endPos - startPos);
   string rest = str.substr(endPos - startPos + 1, string::npos);
   str = rest;
   return(next);
}

// Ref member functions

// Initialize vector to hold Bible book names
vector<string> InitializeBooks(vector<string>& books) {
   books = {"Genesis", "Exodus", "Leviticus", "Numbers", "Deuteronomy",
            "Joshua", "Judges", "Ruth", "1 Samuel", "2 Samuel", "1 Kings",
            "2 Kings", "1 Chronicles", "2 Chronicles", "Ezra", "Nehemiah",
            "Esther", "Job", "Psalms", "Proverbs", "Ecclesiastes", "Song of Songs",
            "Isaiah", "Jeremiah", "Lamentations", "Ezekiel", "Daniel", "Hosea",
            "Joel", "Amos", "Obadiah", "Jonah", "Micah", "Nahum", "Habakkuk",
            "Zephaniah", "Haggai", "Zechariah", "Malachi", "Matthew", "Mark",
            "Luke", "John", "Acts", "Romans", "1 Corinthians", "2 Corinthians",
            "Galatians", "Ephesians", "Philippians", "Colossians", "1 Thessalonians",
            "2 Thessalonians", "1 Timothy", "2 Timothy", "Titus", "Philemon",
            "Hebrews", "James", "1 Peter", "2 Peter", "1 John", "2 John",
            "3 John", "Jude", "Revelation"};

   return books;
}

// Default constructor
Ref::Ref() {
   book = 0;
   chapter = 0;
   verse = 0;

   // Initialize the books
   InitializeBooks(booksOfTheBible);
}

// Parse constructor - receives a line "34:5:7 text"
Ref::Ref(const string s) {
   string rtext = s; // make local copy of string to avoid modifying parameter
   // parse the reference - notice, currently there is no error checking!

   // Get book number
   string strbook = GetNextToken(rtext,":");
   book = atoi(strbook.c_str());

   // Get the chapter number
   string strchap = GetNextToken(rtext,":");
   chapter = atoi(strchap.c_str());

   // Get the verse number
   string strverse = GetNextToken(rtext," ");
   verse = atoi(strverse.c_str());

   // Initialize the books
   noBooks = false;
   InitializeBooks(booksOfTheBible);
}

// Construct Ref from three integers
Ref::Ref(const int b, const int c, const int v) {
   book = b;
   chapter = c;
   verse = v;

   // Initialize the books
   noBooks = false;
   InitializeBooks(booksOfTheBible);
}

// Accessors
int Ref::getBook() {return book;}	 // Access book number
int Ref::getChapter() {return chapter;}	 // Access chapter number
int Ref::getVerse() {return verse;};     // Access verse number

// Access book name
string Ref::getBookName() {
   // Make sure book name exists
   if (noBooks || book > 66) {
      return to_string(book);
   } else {
      return booksOfTheBible[book - 1];
   }
}

// REQUIRED: == comparison
bool Ref::operator==(Ref & ref) {
   // TODO: implement comparison functions

   if ((book == ref.getBook()) && (chapter == ref.getChapter()) &&
       (verse == ref.getVerse())) {
      return true;
   }

   return false;
}

// OPTIONAL: define < and > comparisons
// Unused
bool Ref::operator<(Ref& ref) {
   if (book < ref.getBook()) {
      return true;
   }

   if (book == ref.getBook()) {
      if (chapter < ref.getChapter()) {
         return true;
      }

      if (chapter == ref.getChapter()) {
         if (verse < ref.getVerse()) {
            return true;
         }
      }
   }

   return false;
}

// Unused
bool Ref::operator>(Ref& ref) {
   if (book > ref.getBook()) {
      return true;
   }

   if (book == ref.getBook()) {
      if (chapter > ref.getChapter()) {
         return true;
      }

      if (chapter == ref.getChapter()) {
         if (verse > ref.getVerse()) {
            return true;
         }
      }
   }

   return false;
}

// Display Reference
void Ref::display() {
   // TODO: modify display function to show book name instead of book number
   if (noBooks) {
      cout << book << ":" << chapter << endl << verse;
   } else {
      cout << booksOfTheBible[book - 1] << " " << chapter << "<br>" <<  verse << endl;
   }
}

// Display reference with no book or chapter numbers/names
void Ref::displayNoBookOrChapter() {
   cout << verse;
}
