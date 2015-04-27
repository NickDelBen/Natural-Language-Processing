//A document for storing the nGram information of a file
//Adds methods for dealing with Good-Turing language model

#ifndef _T_GTDOCUMENT
#define _T_GTDOCUMENT

#include "gt_document.h"

namespace nlp {
  //Sets the necessary parameters for the document
  template <typename Type> int GTDocument<Type>::setValues(int gramLenth_in) {
    int lengthIterator;

    for (lengthIterator = 0; lengthIterator < gramLenth_in; lengthIterator++) {
      std::unordered_map<int, int> newMap1;
      std::unordered_map<int, double> newMap2;
      frequencies.push_back(newMap1);
      probabilities.push_back(newMap2);
    }

    return 0;
  }
  
  //Creates the distrubutions for each frequency frequency
  template <typename Type> int GTDocument<Type>::createFrequencyDistrubution(int length_in) {
    int lengthIterator;
    int frequencyIterator;
    int index;
    int occurances;
    double normalizationConstant;
    double numNgrams;

    //Create a distrubution for each ngram length
    for (lengthIterator = 0; lengthIterator < length_in; lengthIterator++) {
      //Find the location of the current ngram length
      index = this->getIndex(lengthIterator + 1);
      //Iterate over the database and increase counts for each ngram length
      for (auto iterator = this->dictionary[index].begin(); iterator != this->dictionary[index].end(); ++iterator) {
        //Extract the number of times this ngram occurs
        occurances = iterator->second;
        //Increase the counter for ngrams that occur this many times
        if (frequencies[lengthIterator].count(occurances) == 0) {
          //If nothing of this count has occured before, set occurances to 1
          frequencies[lengthIterator][occurances] = 1;
        } else {
          //If count has occured before increment frequency
          frequencies[lengthIterator][occurances] = frequencies[lengthIterator][occurances] + 1;
        }             
      }
    }

    //If the threshold is 0 we do not need any probabilities
    if (threshold == 0) {
      return 0;
    }

    //Find the number of ngrams we are normalizing for
    numNgrams = (double) this->numNgrams(1);

    //Find the probabilities for the required good turing values
    for (lengthIterator = 0; lengthIterator < length_in; lengthIterator++) {
      //Check to make sure probability of length 1 is non-zero
      if (frequencies[lengthIterator].count(1) == 0) {
        return -1;
      }
      //Find the probability of an unseen ngram occuring
      probabilities[lengthIterator][0] = frequencies[lengthIterator][1] / numNgrams;
      //Calculate the normalization constant for this
      normalizationConstant = 1 - probabilities[lengthIterator][0];     
      //Only iterate over the values required for good turing
      for (frequencyIterator = 1; frequencyIterator < threshold; frequencyIterator++) {
        //Check to make sure frequency is nonzero
        if (frequencies[lengthIterator].count(frequencyIterator + 1) == 0) {
          return -1;
        }
        //Find the occurances of the word occuring
        occurances = frequencies[lengthIterator][frequencyIterator];
        //Extract the number of occurances for this number
        probabilities[lengthIterator][frequencyIterator] = ((frequencyIterator + 1) * frequencies[lengthIterator][frequencyIterator + 1]) / (numNgrams * frequencies[lengthIterator][frequencyIterator]);
        //Normalize the current probability
        probabilities[lengthIterator][frequencyIterator] = probabilities[lengthIterator][frequencyIterator] * normalizationConstant;
      }
    }

    return 0;
  }

  //Sets the documents threshold value
  template <typename Type> int GTDocument<Type>::setThreshold(int threshold_in) {
    threshold = threshold_in;
    return 0;
  }

  //Sets the size of the vocabulary
  template <typename Type> int GTDocument<Type>::setVocabulary(int vocabulary_in) {
    vocabulary = vocabulary_in;
    return 0;
  }

  //Computes the prabability of an ngram occuring in the document
  template <typename Type> double GTDocument<Type>::ngramProbability(std::vector<Type> * ngram_in) {
    int ngramCount;

    //Count the number of times the ngram occurs
    ngramCount = this->countNgram(ngram_in);

    //If the count is below our threshold use good-turing model
    if (ngramCount < threshold) {
       return probabilities[ngram_in->size() - 1][ngramCount];  
    }

    //If count is greater than or equal to threshold use Maximum Likliehood estimation
    return (double) (((double) this->countNgram(ngram_in)) / ((double) this->numNgrams(1)));
  }

  //Computes the prabability of an ngram occuring in the document given that some portion has already occured
  template <typename Type> double GTDocument<Type>::ngramProbabilityGiven(std::vector<Type> * given_in, std::vector<Type> * new_in) {
    std::vector<Type> fullNgram;
    int ngramIterator;
    int ngramCount;

    //Create the full ngram to find the probability of
    fullNgram = *given_in;
    for (ngramIterator = 0; ngramIterator < (int) new_in->size(); ngramIterator++) {
      fullNgram.push_back((*new_in)[ngramIterator]);
    }

    //Check the count of the full sentence
    ngramCount = this->countNgram(&fullNgram);

    //If the count is below threshold use good-turing model
    if (ngramCount < threshold) {
      return ngramProbability(&fullNgram) / ngramProbability(given_in);
    }

    return ((double) ((double) this->countNgram(&fullNgram)) / ((double) this->countNgram(given_in)));
  }

  //Computes the probability of a sentence occuring based on the implementing language model
  template <typename Type> double GTDocument<Type>::sentenceProbability(int length_in, std::vector<Type> * sentence_in) {
    std::vector<Type> currentNgram;
    int tokenIterator;
    double result;

    //Initially probability is simply probability of first word
    currentNgram.push_back((*sentence_in)[0]);
    result = ngramProbability(&currentNgram);

    for (tokenIterator = 1; tokenIterator < (int) sentence_in->size(); ++tokenIterator) {
      std::vector<Type> currentNew;
      currentNew.push_back((*sentence_in)[tokenIterator]);
      //Trim any excess tokens from the current given nGram
      while ((int) currentNgram.size() >= length_in) {
        currentNgram.erase(currentNgram.begin());
      }
      result = result * ngramProbabilityGiven(&currentNgram, &currentNew);
      //Add the new token to the end of the nGram
      currentNgram.push_back((*sentence_in)[tokenIterator]);
    }

    return result;
  }

  //Computes the probability of a sentence occuring based on the good-turing language model
  template <typename Type> double GTDocument<Type>::logSentenceProbability(int length_in, std::vector<Type> * sentence_in) {
    double result;
    int sentenceIterator;
    std::vector<Type> currentNgram;

    currentNgram.push_back((*sentence_in)[0]);
    result = log(ngramProbability(&currentNgram)); 

    if (length_in == 1) {
      for (sentenceIterator = 1; sentenceIterator < (int) sentence_in->size(); sentenceIterator++) {
        std::vector<Type> currentNew;
        currentNew.push_back((*sentence_in)[sentenceIterator]);
        result = result + log(ngramProbability(&currentNew));
      }
      return result;
    }
    
    for(sentenceIterator = 1; sentenceIterator < (int) sentence_in->size(); sentenceIterator++) {
      std::vector<Type> currentNew;
      currentNew.push_back((*sentence_in)[sentenceIterator]);
      //Trim any excess tokens from the current nGram
      while ((int) currentNgram.size() >= length_in) {
        currentNgram.erase(currentNgram.begin());
      }
      result = result + log(ngramProbabilityGiven(&currentNgram, &currentNew));
      //Add the current token to the back of the current ngram
      currentNgram.push_back((*sentence_in)[sentenceIterator]);
    }

    return result;  
  }

  //Creates a new instance of GTDocument finding all the ngrams
  template <typename Type> GTDocument<Type>::GTDocument(std::vector<Type> * tokens_in, int gramLength_in, int threshold_in, int vocabulary_in)
    :Document<Type>(tokens_in, gramLength_in) {
    setThreshold(threshold_in);
    setVocabulary(vocabulary_in);
    setValues(gramLength_in);
  }

  //Creates a new instance of ADDocument finding all the ngrams
  template <typename Type> GTDocument<Type>::GTDocument(std::vector<Type> * tokens_in, int gramLengthLow_in, int gramLengthHigh_in, int threshold_in, int vocabulary_in)
    :Document<Type>(tokens_in, gramLengthLow_in, gramLengthHigh_in) {
    setThreshold(threshold_in);
    setVocabulary(vocabulary_in);
    setValues(gramLengthHigh_in);
  }

  //Creates a new instance of ADDocument finding all the ngrams
  template <typename Type> GTDocument<Type>::GTDocument(std::vector<Type> * tokens_in, std::vector<int> * gramLengths_in, int threshold_in, int vocabulary_in)
    :Document<Type>(tokens_in, gramLengths_in) {
    setThreshold(threshold_in);
    setVocabulary(vocabulary_in);
  }

  //Default constructor and destructor
  template <typename Type> GTDocument<Type>::GTDocument() {}
  template <typename Type> GTDocument<Type>::~GTDocument() {} 
};



#endif