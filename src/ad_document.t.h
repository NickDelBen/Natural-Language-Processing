//A document for storing the nGram information of a file
//Adds methods for dealing with add delta probabilities

#ifndef _T_ADDOCUMENT
#define _T_ADDOCUMENT

#include "ad_document.h"

namespace nlp {

  //Sets a new delta value
  template <typename Type> int ADDocument<Type>::setDelta(double delta_in) {
    delta = delta_in;
    return 0;
  }

  //Computes the prabability of an ngram occuring in the document
  template <typename Type> double ADDocument<Type>::ngramProbability(std::vector<Type> * ngram_in) {
    return ngramProbability(ngram_in, this->numDistinctNgrams(ngram_in->size()));
  }

  //Computes the prabability of an ngram occuring in the document
  template <typename Type> double ADDocument<Type>::ngramProbability(std::vector<Type> * ngram_in, int vocabulary_in) {
    double numerator;
    double denominator;
    std::vector<Type> given = *ngram_in;
    given.pop_back();

    numerator = (double) this->countNgram(ngram_in) + delta;
    denominator = (double) (ngram_in->size() > 1 ? this->countNgram(&given) : this->numNgrams(1)) + delta * pow((double) vocabulary_in, (double) ngram_in->size());

    return numerator / denominator;
  }

  //Computes the probability of a sentence occuring based
  template <typename Type> double ADDocument<Type>::sentenceProbability(int length_in, std::vector<Type> * sentence_in) {
    double result;
    int sentenceIterator;
    std::vector<Type> currentNgram;

    result = 1.0;
    for(sentenceIterator = 0; sentenceIterator < (int) sentence_in->size(); ++sentenceIterator) {
      //Trim any excess tokens from the current nGram
      while ((int) currentNgram.size() >= length_in) {
        currentNgram.erase(currentNgram.begin());
      }
      //Add the current token to the back of the current ngram
      currentNgram.push_back((*sentence_in)[sentenceIterator]);
      result = result * ngramProbability(&currentNgram);
    }

    return result;
  }

  template <typename Type> double ADDocument<Type>::logSentenceProbability(int length_in, std::vector<Type> * sentence_in) {
    return logSentenceProbability(length_in, sentence_in, this->numDistinctNgrams(length_in));
  }

  template <typename Type> double ADDocument<Type>::logSentenceProbability(int length_in, std::vector<Type> * sentence_in, int vocabulary_in) {
    double result;
    int sentenceIterator;
    std::vector<Type> currentNgram;

    result = 0.0; 
    for(sentenceIterator = 0; sentenceIterator < (int) sentence_in->size(); ++sentenceIterator) {
      //Trim any excess tokens from the current nGram
      while ((int) currentNgram.size() >= length_in) {
        currentNgram.erase(currentNgram.begin());
      }
      //Add the current token to the back of the current ngram
      currentNgram.push_back((*sentence_in)[sentenceIterator]);
      result = result + log(ngramProbability(&currentNgram, vocabulary_in));
    }    

    return result;
  }

  //Creates a new instance of ADDocument finding all the ngrams from size 1 to the specified number
  template <typename Type> ADDocument<Type>::ADDocument(std::vector<Type> * tokens_in, int gramLength_in, double delta_in) 
    :Document<Type>(tokens_in, gramLength_in) {
      setDelta(delta_in);
    }

  //Creates a new instance of ADDocument finding all the ngrams from a specified size to another specified size
  template <typename Type> ADDocument<Type>::ADDocument(std::vector<Type> * tokens_in, int gramLengthLow_in, int gramLengthHigh_in, double delta_in)
    :Document<Type>(tokens_in, gramLengthLow_in, gramLengthHigh_in) {
      setDelta(delta_in);
    }

  //Creates a new instance of ADDocument finding all the ngrams of the sizes specified in the inout vector
  template <typename Type> ADDocument<Type>::ADDocument(std::vector<Type> * tokens_in, std::vector<int> * gramLengths_in, double delta_in)
    :Document<Type>(tokens_in, gramLengths_in) {
      setDelta(delta_in);
    }

  //Default constructor and destructor
  template <typename Type> ADDocument<Type>::ADDocument() {}
  template <typename Type> ADDocument<Type>::~ADDocument() {}
};

#endif