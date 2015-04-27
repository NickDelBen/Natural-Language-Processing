/**************************************************************
* Interface defines functions for statistical language modeling
*
* Created By: Nick DelBen
* Created On: March 8, 2015
*
* Last Edited: March 8, 2015
*   - Created initially
**************************************************************/

#ifndef _I_LANGUAGEMODEL
#define _I_LANGUAGEMODEL

namespace nlp {

  template <typename Type> class LanguageModel {
  public:
    /******************
    * Computes the prabability of an ngram occuring in the document
    * @param  ngram_in  ngram to check probability of
    * @return probability of specified ngram occuring
    ******************/
    virtual double ngramProbability(std::vector<Type> * ngram_in) = 0;

    /******************
    * Computes the probability of a sentence occuring based on the implementing language model
    * @param  length_in   length of ngrams to check
    * @param  sentence_in sentence to find the probability of
    * @return prabability of sentence occurance
    ******************/
    virtual double sentenceProbability(int length_in, std::vector<Type> * sentence_in) = 0;

    /******************
    * Computes the probability of a sentence occuring based on the implementing language model
    * @param  length_in   length of ngrams to check
    * @param  sentence_in sentence to find the probability of
    * @return prabability of sentence occurance
    ******************/
    virtual double logSentenceProbability(int length_in, std::vector<Type> * sentence_in) = 0;
  };

};

#endif