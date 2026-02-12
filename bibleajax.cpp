/* Demo server program for Bible lookup using AJAX/CGI interface
 * By James Skon, Febrary 10, 2011
 * updated by Bob Kasper, January 2020
 * updated by Deborah Wilson, February 2026
 * Mount Vernon Nazarene University
 *
 * This sample program works using the cgicc AJAX library to
 * allow live communication between a web page and a program running on the
 * same server that hosts the web server.
 *
 * This program is run by a request from the associated html web document.
 * A Javascript client function invokes an AJAX request,
 * passing the input form data as the standard input string.
 *
 * The cgi.getElement function parses the input string, searching for the matching
 * field name, and returing a "form_iterator" oject, which contains the actual
 * string the user entered into the corresponding field. The actual values can be
 * accessed by dereferencing the form iterator twice, e.g. **verse
 * refers to the actual string entered in the form's "verse" field.
 *
 * STUDENT NAME: Elijah Litman
 */

#include <iostream>
#include <stdio.h>
#include <string.h>

/* Required libraries for AJAX to function */
#include "/home/class/csc3004/cgicc/Cgicc.h"
#include "/home/class/csc3004/cgicc/HTTPHTMLHeader.h"
#include "/home/class/csc3004/cgicc/HTMLClasses.h"

// Project 1 files
#include "Bible.h"
#include "Verse.h"
#include "Ref.h"

using namespace std;
using namespace cgicc;

int main()
{
   /* A CGI program must send a response header with content type
    * back to the web client before any other output.
    * For an AJAX request, our response is not a complete HTML document,
    * so the response type is just plain text to insert into the web page.
    */
   cout << "Content-Type: text/plain\n\n";

   Cgicc cgi;  // create object used to access CGI request data

   // GET THE INPUT DATA
   // browser sends us a string of field name/value pairs from HTML form
   // retrieve the value for each appropriate field name
   form_iterator st = cgi.getElement("search_type");
   form_iterator translation = cgi.getElement("translation");
   form_iterator book = cgi.getElement("book");
   form_iterator chapter = cgi.getElement("chapter");
   form_iterator verse = cgi.getElement("verse");
   form_iterator nv = cgi.getElement("num_verse");

   // Convert book, chapter, and verse to ints
   int bookNum = book->getIntegerValue();
   int chapterNum = chapter->getIntegerValue();
   int verseNum = verse->getIntegerValue();
   int numVerses = nv->getIntegerValue();

   // Convert the translation to a string
   string BibleTranslation = translation->getValue();

   // Determine if a valid translation was given
   if (BibleTranslation != "web" && BibleTranslation != "kjv" &&
       BibleTranslation != "dby" && BibleTranslation != "ylt" &&
       BibleTranslation != "webster") {
      cout << "<p>Error: (" << BibleTranslation << ") is not a valid translation. " <<
              "Please select a valid translation from the dropdown menu.</p>" << endl;
      return 0;
   }

   // Check input data
   bool validInput = true;

   // Book number check
   if (book != cgi.getElements().end()) {
      if (bookNum > 66 || bookNum < 1) {
         cout << "<p>There is no book number (" << bookNum << ") in the Bible.</p>" << endl;
         validInput = false;
      }
   }

   // Chapter number check
   if (chapter != cgi.getElements().end()) {
      if (chapterNum > 150) {
         cout << "<p>The chapter number (" << chapterNum << ") is too high.</p>" << endl;
         validInput = false;
      } else if (chapterNum <= 0) {
         cout << "<p>The chapter must be a positive number.</p>" << endl;
         validInput = false;
      }
   }

   // Verse number check
   if (verse != cgi.getElements().end()) {
      if (verseNum > 176) {
         cout << "<p>The verse number (" << verseNum << ") is too high.</p>" << endl;
         validInput = false;
      } else if (verseNum <= 0) {
         cout << "<p>The verse must be a positive number.</p>" << endl;
         validInput = false;
      }
   }

   // TODO: OTHER INPUT VALUE CHECKS ARE NEEDED ... but that's up to you!

   /* TODO: PUT CODE HERE TO CALL YOUR BIBLE CLASS FUNCTIONS
    *        TO LOOK UP THE REQUESTED VERSES
    */

   // Used for verse retrieval issues
   LookupResult result;

   // Given reference
   Ref ref(bookNum, chapterNum, verseNum);

   // Create & open the correct Bible
   string BibleFileLocation = "/home/class/csc3004/Bibles/" + BibleTranslation + "-complete";
   Bible selectedBible(BibleFileLocation);
   selectedBible.openBible();

   // Declare & initialize the verse
   Verse requestedVerse;
   requestedVerse = selectedBible.lookup(ref, result);

   /* SEND BACK THE RESULTS
    * Finally we send the result back to the client on the standard output stream
    * in HTML text format.
    * This string will be inserted as is inside a container on the web page,
    * so we must include HTML formatting commands to make things look presentable!
    */
   if (validInput) {
      if (result == SUCCESS) {
         requestedVerse.display();
         cout << endl;

         for (int i = 0; i < numVerses - 1; i++) {

            // Do not allow any attempts to retrieve a verse beyond Rev 22:21
            if (requestedVerse.getRef().getBook() == 66 &&
                requestedVerse.getRef().getChapter() == 22 &&
                requestedVerse.getRef().getVerse() == 21) {
              cout << "<p>Revelation 22:21 is the last verse in the Bible.</p>" << endl;
              break;
            }

            // Get the next verse
            Verse nextVerse = selectedBible.nextVerse(result);

            // Determine if the book and chapter needs to be displayed or not
            if (nextVerse.getRef().getBook() > requestedVerse.getRef().getBook() ||
                nextVerse.getRef().getChapter() > requestedVerse.getRef().getChapter()) {
               cout << "<br><br>" << endl;;
               nextVerse.display();
            } else {
               cout << "<br>" << endl;
               nextVerse.displayNoBC();
            }

            cout << endl;

            /* Set verse to nextVerse
               This is to help determine if
               display or displayNoBC should be called */
            requestedVerse = nextVerse;
         }

         cout << "</p>" << endl;
      } else {
         cout << "<p>" << selectedBible.error(ref, result) << "</p>" << endl;
      }
   }

   selectedBible.closeBible();

   return 0;
}
