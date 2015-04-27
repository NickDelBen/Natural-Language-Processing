/**************************************************************
* A document for storing the nGram information of a file
* Adds methods for dealing with Maximum Likelyhood probabilities
*
* Created By: Nick DelBen
* Created On: March 4, 2015
*
* Last Edited: March 6, 2015
*   - Modified generateSentence to allow proper sentence prefixes
**************************************************************/

#ifndef _H_ML_DOCUMENT
#define _H_ML_DOCUMENT

#include <time.h>  //time()
#include <cmath>   //log()

#include "document.t.h"
#include "languageModel.i.h"

#define SENTENCE_PREFIX_NGRAM_LENGTH 2

namespace nlp {

  template <typename Type> class MLDocument : public Document<Type>, public LanguageModel<Type> {
  public:
    /*******************
    * Creates a new instance of MLDocument finding all the ngrams 
    * from size 1 to the specified number
    * @param tokens_in    tokens to create the document from
    * @param gramLenth_in longest nGrams to search for
    *******************/
    MLDocument(std::vector<Type> * tokens_in, int gramLength_in);
    /*******************
    * Creates a new instance of MLDocument finding all the ngrams 
    * from a specified size to another specified size
    * @param tokens_in    tokens to create the document from
    * @param gramLenthLow_in  shortest nGrams to search for
    * @param gramLenthHigh_in longest nGrams to search for
    *******************/
    MLDocument(std::vector<Type> * tokens_in, int gramLengthLow_in, int gramLengthHigh_in);
    /*******************
    * Creates a new instance of MLDocument finding all the ngrams 
    * of the sizes specified in the inout vector
    * @param tokens_in    tokens to create the document from
    * @param gramLenghts_in the lengths to construct the dictionaries from
    *******************/
    MLDocument(std::vector<Type> * tokens_in, std::vector<int> * gramLengths_in);

    //Default constructor and destructor
    MLDocument();
    ~MLDocument();

    /******************
    * Computes the prabability of an ngram occuring in the document
    * @param  ngram_in  ngram to check probability of
    * @return probability of specified ngram occuring
    ******************/
    double ngramProbability(std::vector<Type> * ngram_in);

    /******************
    * Computes the probability of an ngram occuring given another ngram
    * @param  given_in ngram give to have occured already
    * @param  ngram_in sentence to find the probability of
    * @return prabability of sentence occurance
    ******************/
    double probabilityGiven(std::vector<Type> * given_in, std::vector<Type> * ngram_in);

    /******************
    * Computes the probability of a token occuring given an ngram that precedes it
    * @param  given_in ngram give to have occured already
    * @param  token_in token to find the probability of
    * @return prabability of ngram occurance
    ******************/
    double probabilityGiven(std::vector<Type> * given_in, Type * ngram_in);

    /******************
    * Computes the probability of a sentence occuring based
    * @param  length_in   length of ngrams to check
    * @param  sentence_in sentence to find the probability of
    * @return prabability of sentence occurance
    ******************/
    double sentenceProbability(int length_in, std::vector<Type> * sentence_in);

    /******************
    * Computes the probability of a sentence occuring based on the maximum-likliehood language model
    * @param  length_in   length of ngrams to check
    * @param  sentence_in sentence to find the probability of
    * @return prabability of sentence occurance
    ******************/
    double logSentenceProbability(int length_in, std::vector<Type> * sentence_in);

    /******************
    * Creates a probability distrubution for each ngram of the specified length in the dictionary. 
    * Stores the results in the specified location. (wordList[i] has probability of probabilityList[i])
    * @param  length_in       length of ngrams to get distrubution for
    * @param  wordList        location to store the vector of unique tokens
    * @param  probabilityList location to store the probabilities
    * @return 0 success
    ******************/
    int makeDistrubution(int length_in, std::vector<std::vector<Type>> * wordList, std::vector<double> * probabilityList);

    /******************
    * Creates a probability distrubution of an ngram given already occuring ngrams. 
    * Stores the results in the specified location. (wordList[i] has probability of probabilityList[i])
    * @param  wordList        location to store the vector of unique tokens
    * @param  given_in        parts of ngram already given to have occured
    * @param  choices         location to store the list of words that can follow
    * @param  probabilityList location to store the probabilities
    * @return 0 success
    ******************/
    int makeDistrubution(std::vector<std::vector<Type>> * wordList, std::vector<Type> * given_in, std::vector<Type> * choices, std::vector<double> * probabilityList);
    
    /******************
    * Generates a random ngram from the document and stores it in the specified location
    * @param  length_in      length of ngrams to use for context of word generation
    * @param  location_in    location to store the resulting ngram
    * @param  sentencePrefix if this is >0 than the first word of the sentence will be a sentence 
    *                        starter from the dictionary otherwise it will be random
    * @return 0 successful
    ******************/
    int generateSentence(int length_in, std::vector<Type> * location_in, int sentencePrefix);
  };

};

/*******************
* Randomly selects an index from a weighted probability set
* @param  probabilities_in list of probabilities to selecte from (sums to 1)
* @return the index of the selected probability
*******************/
int getRandomIndex(std::vector<double> * probabilities_in);

#endif