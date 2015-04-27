/**************************************************************
* A document for storing the nGram information of a file
* Adds methods for dealing with Good-Turing language model
*
* Created By: Nick DelBen
* Created On: March 14, 2015
*
* Last Edited: March 14, 2015
*   - Created initially
**************************************************************/

#ifndef _H_GT_DOCUMENT
#define _H_GT_DOCUMENT

#include <string>        //  std::string
#include <vector>        //  std::vector
#include <unordered_map> //  std::unordered_map
#include <cmath>         //  log()

#include "document.t.h"
#include "languageModel.i.h"

namespace nlp {
  
  template <typename Type> class GTDocument : public Document<Type>, public LanguageModel<Type> {
  private:
    /* The frequency distrubutions for this document */
    std::vector<std::unordered_map<int, int>> frequencies;

    /* The probability distrubutions for this document */
    std::vector<std::unordered_map<int, double>> probabilities;

    /* The size of the vocabulary for this language model */
    int vocabulary;

    /* The threshold for when to stop using Good Turing model */
    int threshold;

    /*******************
    * Sets the necessary parameters for the document
    * @param gramLenth_in longest nGrams to search for
    *******************/
    int setValues(int gramLenth_in);

  public:
    /*******************
    * Creates a new instance of GTDocument finding all the ngrams 
    * from size 1 to the specified number
    * @param tokens_in     tokens to create the document from
    * @param gramLenth_in  longest nGrams to search for
    * @param threshold_in  threshold for the good turing model
    * @param vocabulary_in size of the vocabulary
    *******************/
    GTDocument(std::vector<Type> * tokens_in, int gramLength_in, int threshold_in, int vocabulary_in);
    /*******************
    * Creates a new instance of ADDocument finding all the ngrams 
    * from a specified size to another specified size
    * @param tokens_in    tokens to create the document from
    * @param gramLenthLow_in  shortest nGrams to search for
    * @param gramLenthHigh_in longest nGrams to search for
    * @param threshold_in  threshold for the good turing model
    * @param vocabulary_in size of the vocabulary
    *******************/
    GTDocument(std::vector<Type> * tokens_in, int gramLengthLow_in, int gramLengthHigh_in, int threshold_in, int vocabulary_in);
    /*******************
    * Creates a new instance of ADDocument finding all the ngrams 
    * of the sizes specified in the inout vector
    * @param tokens_in    tokens to create the document from
    * @param gramLenghts_in the lengths to construct the dictionaries from
    * @param threshold_in  threshold for the good turing model
    * @param vocabulary_in size of the vocabulary
    *******************/
    GTDocument(std::vector<Type> * tokens_in, std::vector<int> * gramLengths_in, int threshold_in, int vocabulary_in);

    //Default constructor and destructor
    GTDocument();
    ~GTDocument();

    /******************
    * Creates the distrubutions for each frequency frequency
    * @param  length_in length of ngrams to create distrubutions for
    * @return 0  success
    * @return -1 threshold too high
    ******************/
    int createFrequencyDistrubution(int length_in);

    /******************
    * Sets the documents threshold value
    * @param threshold_in new threshold value
    * @return 0 success
    ******************/
    int setThreshold(int threshold_in);

    /******************
    * Sets the size of the vocabulary
    * @param vocabulary_in new vocabulary size
    * @return 0 success
    ******************/
    int setVocabulary(int vocabulary_in);

    /******************
    * Computes the prabability of an ngram occuring in the document
    * @param  ngram_in ngram to check probability of
    * @return probability of specified ngram occuring
    ******************/
    double ngramProbability(std::vector<Type> * ngram_in);

    /******************
    * Computes the prabability of an ngram occuring in the document given that some portion has already occured
    * @param  given_in the portion iof the ngram given to have already occured
    * @param  new_in   the new portion of the ngram to find probability of
    * @return probability of specified ngram occuring
    ******************/
    double ngramProbabilityGiven(std::vector<Type> * given_in, std::vector<Type> * new_in);

    /******************
    * Computes the probability of a sentence occuring based on the implementing language model
    * @param  length_in   length of ngrams to check
    * @param  sentence_in sentence to find the probability of
    * @return prabability of sentence occurance
    ******************/
    double sentenceProbability(int length_in, std::vector<Type> * sentence_in);

    /******************
    * Computes the probability of a sentence occuring based on the good-turing language model
    * @param  length_in     length of ngrams to check
    * @param  sentence_in   sentence to find the probability of
    * @param  vocabulary_in true size of vocabulary to consider
    * @return prabability of sentence occurance
    ******************/
    double logSentenceProbability(int length_in, std::vector<Type> * sentence_in);
  };

};





#endif