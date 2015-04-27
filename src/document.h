/**************************************************************
* A document for storing the nGram information of a file
*
* Created By: Nick DelBen
* Created On: Feb 28, 2015
*
* Last Edited: March 6, 2015
*   - Added readTokens() and refactored accordingly
**************************************************************/

#ifndef _H_DOCUMENT
#define _H_DOCUMENT

#include <string>        //std::string
#include <vector>        //std::vector
#include <unordered_map> //std::unordered_map
#include <algorithm>     //std::sort()   std::find()

#include "VectorHash.h"

#define EOS "<END>"

namespace nlp {

  template <typename Type> class Document {
  protected:
    /* The amount of elements in each ngram for the document. */
    std::vector<int> ngramLengths;

    /* Dictionary to keep track of nGram occurances or varying lengths */
    std::vector<std::unordered_map<std::vector<Type>, int>> dictionary;

    /* Amount of tokens in the document */
    int numTokens;

    /* The tokens that were used to generate the document */
    std::vector<Type> tokens;

    /*******************
     * Finds the index of a ngram length in the dictionary
     * @param  length_in length to find index of
     * @return index of the specified length
     ******************/
    int getIndex(int length_in);

    /*******************
    * Initilizes the values of the object
    * @param tokens_in    tokens to create the document from
    * @param gramLenghts_in the lengths to construct the dictionaries from
    *******************/
    int init_object(std::vector<Type> * tokens_in, std::vector<int> * gramLengths_in);

  public:
    /*******************
    * Creates a new instance of Document finding all the ngrams 
    * from size 1 to the specified number
    * @param tokens_in    tokens to create the document from
    * @param gramLenth_in longest nGrams to search for
    *******************/
    Document(std::vector<Type> * tokens_in, int gramLength_in);
    /*******************
    * Creates a new instance of Document finding all the ngrams 
    * from a specified size to another specified size
    * @param tokens_in    tokens to create the document from
    * @param gramLenthLow_in  shortest nGrams to search for
    * @param gramLenthHigh_in longest nGrams to search for
    *******************/
    Document(std::vector<Type> * tokens_in, int gramLengthLow_in, int gramLengthHigh_in);
    /*******************
    * Creates a new instance of Document finding all the ngrams 
    * of the sizes specified in the inout vector
    * @param tokens_in    tokens to create the document from
    * @param gramLenghts_in the lengths to construct the dictionaries from
    *******************/
    Document(std::vector<Type> * tokens_in, std::vector<int> * gramLengths_in);

    //Default constructor and destructor
    Document();
    ~Document();

    /*******************
    * Reads tokens of the specified lengths from the document's tokens
    * @param  lengths_in ngram lengths to be created from the tokens
    * @return 0 success
    *******************/
    int readTokens(std::vector<int> * lengths_in);

    /*******************
    * Reads tokens from lengths 1 to the specified length from the document's tokens
    * @param  length_in ngram lengths to be created from the tokens
    * @return 0 success
    *******************/
    int readTokens(int length_in);

    /*******************
    * Checks if grams of the specified length have been added to the dictionary
    * @param  length_in the ngram length to check the dictionary for
    * @return -1 invalid ngram length
    * @return 0  ngrams of specified length have not been read
    * @return 1  ngrams of specified length have been read
    *******************/
    int hasNgrams(int length_in);

    /*******************
    * Returns the number of ngrams of a specified length in the document
    * @param  length_in  length of ngrams to get count for
    * @return number of ngrams in the document
    *******************/
    int numNgrams(int length_in);

    /*******************
    * Returns the number of disctict ngrams in the document
    * @param  length_in  length of ngrams to get count for
    * @return number of distinct ngrams in the document
    *******************/
    int numDistinctNgrams(int length_in);

    /*******************
    * Finds the occurances of an ngram in the document
    * @param  nGram_in ngram to check for existace
    * @return number of occurances of nGram in dictionary
    *******************/
    int countNgram(std::vector<Type> * nGram_in);

    /*******************
    * Checks if an nGram occurs in this document
    * @param  nGram_in ngram to check for existace
    * @return 0 nGram is not in database
    * @return 1 nGram is in database
    *******************/
    int hasNgram(std::vector<Type> * nGram_in);

    /*******************
    * Adds an nGram to the dictionary
    * @param nGram_in ngram to add to the database
    * @return 0 success
    *******************/
    int addNgram(std::vector<Type> * nGram_in);

    /*******************
    * Finds the number of ngrams in this document that are also in the specified document
    * @param  length_in   ngram length to compare
    * @param  document_in document to compare to
    * @return number of common ngrams to both documents
    ******************/
    int numCommon(int length_in, Document * document_in);

    /*******************
    * Shows the ngrams of a specified length in this document that are also in a specifeid document
    * @param  length_in   ngram length to compare
    * @param  document_in document to compare to
    * @param  result_in   location to store the common ngrams
    * @return number of common ngrams to both documents
    ******************/
    int findCommon(int length_in, Document * document_in, std::vector<const std::vector<Type>*> * result_in);

    /*******************
    * Finds the number of unique ngram lengths stored in this document
    * @return number of ngram lengths on th document
    ******************/
    int numLengths();

    /******************
    * Checks if the document contains the specified sentence
    * @param  ngramLength_in length of ngrams to check
    * @param  sentence_in    sentence to check the document for
    * @return 0 sentence is not in document
    * @return 1 sentence is in document
    ******************/
    int hasSentence(int ngramLength_in, std::vector<Type> * sentence_in);
  };

};

#endif