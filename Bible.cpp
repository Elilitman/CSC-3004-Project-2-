/* Bible class function definitions
 * Computer Science, MVNU
 * CSC-3004 Introduction to Software Development
 *
 * NOTE: You may add code to this file, but do not
 * delete any code or delete any comments.
 *
 * STUDENT NAME: Elijah Litman
 */

#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

// Default constructor
Bible::Bible()
{
   infile = "/home/class/csc3004/Bibles/web-complete";
}

// Constructor – pass bible filename
Bible::Bible(const string s) { infile = s; }

// REQUIRED: lookup finds a given verse in this Bible
Verse Bible::lookup(Ref ref, LookupResult& status) {
   // TODO: scan the file to retrieve the line that holds ref ...

   // Open the Bible's file if is isn't already
   if (!instream.is_open()) {
      openBible();
   }

   // Declare verse to be returned
   Verse aVerse;

   // Declare string to hold the current line
   string verseLine;

   // Booleans used for checking
   bool found = false;
   bool fileOpened = true;
   bool noVerseOrBook = false;

   // Make sure the file actually opened
   if (!instream.is_open()) {
      fileOpened = false;
      status = OTHER;
   }

   // Check the book number's validity
   if (ref.getBook() < 1 || ref.getBook() > 66) {
      status = NO_BOOK;
   }

   // Check for verses after Revelation 22:21
   if (ref.getBook() == 66) {
      if (ref.getChapter() > 22) {
         status = NO_CHAPTER;
      }

      if (ref.getChapter() == 22 && ref.getVerse() > 21) {
         status = NO_VERSE;
      }
   }

   // Attempt to find the verse
   if ((status != NO_BOOK) || (status != NO_CHAPTER) ||
       (status != NO_VERSE) || (fileOpened == false)) {
      lineNum = 1;

      while(found == false && noVerseOrBook == false &&
            getline(instream, verseLine)) {
         Ref lineRef = Ref(verseLine);
         Verse nextV;

         // Make sure that the end of the Bible has not
         // been reached before trying to get the next verse
         if (!(lineRef.getBook() == 66 &&
               lineRef.getChapter() == 22 &&
               lineRef.getVerse() == 21)) {

            // Save the current position in the Bible
            streampos currPos = instream.tellg();

            // Get the next verse for error checking
            nextV = nextVerse(status);

            // Return to the saved position
            instream.seekg(currPos);
         }

         // Check to see if the current verse is the requested one
         if (ref == lineRef) {
            found = true;
         }

         /* Determine if an invalid chapter or verse was given

            This works by comparing the current verse to the next one.
            If current verse is not the requested verse, but is in the same book
            and chapter, and if the next verse's chapter or book number increments,
            an invalid refernce was given. */

         if (ref.getBook() == lineRef.getBook()) {
            if ((ref.getChapter() == lineRef.getChapter()) &&
                (nextV.getRef().getBook() > ref.getBook() ||
                 nextV.getRef().getChapter() > ref.getChapter())) {
               status = NO_VERSE;
               noVerseOrBook = true;
            } else if (nextV.getRef().getBook() > ref.getBook()) {
               status = NO_CHAPTER;
               noVerseOrBook = true;
            }
         }

         lineNum++;
      }
   }

   // update the status variable
   if (found) {
      // Verse was found
      status = SUCCESS;
      aVerse = Verse(verseLine);
   } else {
      // create and return the verse object
      // default verse, to be replaced by a Verse object
      // that is constructed from a line in the file.

      // Verse was not found
      aVerse = Verse();
   }

   return(aVerse);
}

// REQUIRED: Return the next verse from the Bible file stream if the file is open.
// If the file is not open, open the file and return the first verse.
Verse Bible::nextVerse(LookupResult& status) {
   // Open the file if is not is open
   if (!instream.is_open()) {
      openBible();
   }

   // Verse the next (or first) verse and return it
   string verseNext;
   getline(instream, verseNext);

   Verse verse = Verse(verseNext);
   return verse;
}

// REQUIRED: Return an error message string to describe status
string Bible::error(Ref ref, LookupResult status) {
   if (status == NO_BOOK) {
      return "<p>No such book " + to_string(ref.getBook()) + "</p>";
   } else if (status == NO_CHAPTER) {
      return "<p>No such chapter " + to_string(ref.getChapter())
              + " in " + ref.getBookName() + "</p>";
   } else if (status == NO_VERSE) {
      return "<p>No such verse " + to_string(ref.getVerse()) + " in "
              + ref.getBookName() + " " + to_string(ref.getChapter()) + "</p>";
   } else {
      return "<p>Error: An unexpected error has occurred</p>";
   }
}

void Bible::display() {
   cout << "Bible file: " << infile << endl;
}

// OPTIONAL access functions
// OPTIONAL: Return the reference after the given ref
Ref Bible::next(const Ref ref, LookupResult& status) {
   return ref;
}

// OPTIONAL: Return the reference before the given ref
Ref Bible::prev(const Ref ref, LookupResult& status)
{
   return ref;
}

// Open the file
void Bible::openBible() {
   instream.open(infile);
}

// Close the file
void Bible::closeBible() {
   instream.close();
}
