/* Verse class function definitions
 * Computer Science, MVNU
 * CSC-3004 Introduction to Software Development
 *
 * NOTE: You may add code to this file, but do not
 * delete any code or delete any comments.
 *
 * STUDENT NAME: Elijah Litman
 */

#include "Verse.h"
#include <iostream>

using namespace std;

// Default constructor
Verse::Verse() {
   verseText = "Uninitialized Verse!";
   verseRef = Ref();
}

// REQUIRED: Parse constructor - pass verse string from file
Verse::Verse(const string s) {
   // TODO: use Ref constructor to create verseRef
   // store the remainder of the string in verseText
   verseRef = Ref(s);

   int startingPos = s.find(" ");

   verseText = s.substr(startingPos + 1);
}

// REQUIRED: Accessors
string Verse::getVerse() {
   return verseText;
}

Ref Verse::getRef() {
   return verseRef;
}

// display reference and verse
void Verse::display() {
   verseRef.display();
   cout << " " << verseText;
}

// Display verse number and verse text
void Verse::displayNoBC() {
   verseRef.displayNoBC();
   cout << " " << verseText;
}
