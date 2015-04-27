/**************************************************************
* A document for storing the nGram information of a file
* Adds methods for dealing with add delta probabilities
*
* Created By: Nick DelBen
* Created On: March 7, 2015
*
* Last Edited: March 7, 2015
*   - Created initially
**************************************************************/

#ifndef _H_AD_DOCUMENT
#define _H_AD_DOCUMENT

#include <string>   //  std::string
#include <cmath>    //  pow()    log()

#include "document.t.h"
#include "languageModel.i.h"

namespace nlp {

  template <typename Type> class ADDocument : public Document<Type>, public LanguageModel<Type> {
  private:
    /* The delta value to be used for this model */
    double delta;
  public:
    /*******************
    * Creates a new instance of ADDocument finding all the ngrams 
    * from size 1 to the specified number
    * @param tokens_in    tokens to create the document from
    * @param gramLenth_in longest nGrams to search for
    *******************/
    ADDocument(std::vector<Type> * tokens_in, int gramLength_in, double delta_in);
    /*******************
    * Creates a new instance of ADDocument finding all the ngrams 
    * from a specified size to another specified size
    * @param tokens_in    tokens to create the document from
    * @param gramLenthLow_in  shortest nGrams to search for
    * @param gramLenthHigh_in longest nGrams to search for
    *******************/
    ADDocument(std::vector<Type> * tokens_in, int gramLengthLow_in, int gramLengthHigh_in, double delta_in);
    /*******************
    * Creates a new instance of ADDocument finding all the ngrams 
    * of the sizes specified in the inout vector
    * @param tokens_in    tokens to create the document from
    * @param gramLenghts_in the lengths to construct the dictionaries from
    *******************/
    ADDocument(std::vector<Type> * tokens_in, std::vector<int> * gramLengths_in, double delta_in);

    //Default constructor and destructor
    ADDocument();
    ~ADDocument();

    /******************
    * Computes the prabability of an ngram occuring in the document
    * @param  ngram_in      ngram to check probability of
    * @return probability of specified ngram occuring
    ******************/
    double ngramProbability(std::vector<Type> * ngram_in);
    /******************
    * Computes the prabability of an ngram occuring in the document
    * @param  ngram_in      ngram to check probability of
    * @param  vocabulary_in true size of vocabulary to consider
    * @return probability of specified ngram occuring
    ******************/
    double ngramProbability(std::vector<Type> * ngram_in, int vocabulary_in);

    /******************
    * Computes the probability of a sentence occuring based on the add-delta language model
    * @param  length_in   length of ngrams to check
    * @param  sentence_in sentence to find the probability of
    * @return prabability of sentence occurance
    ******************/
    double sentenceProbability(int length_in, std::vector<Type> * sentence_in);

    /******************
    * Computes the probability of a sentence occuring based on the add-delta language model
    * @param  length_in     length of ngrams to check
    * @param  sentence_in   sentence to find the probability of
    * @param  vocabulary_in true size of vocabulary to consider
    * @return prabability of sentence occurance
    ******************/
    double logSentenceProbability(int length_in, std::vector<Type> * sentence_in);
    /******************
    * Computes the probability of a sentence occuring based on the add-delta language model
    * @param  length_in     length of ngrams to check
    * @param  sentence_in   sentence to find the probability of
    * @param  vocabulary_in true size of vocabulary to consider
    * @return prabability of sentence occurance
    ******************/
    double logSentenceProbability(int length_in, std::vector<Type> * sentence_in, int vocabulary_in);

    /******************
    * Sets the documents delta value
    * @param delta_in new delta value
    * @return 0 success
    ******************/
    int setDelta(double delta_in);

  };

};

#endif