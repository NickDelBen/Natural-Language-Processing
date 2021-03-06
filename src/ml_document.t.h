//A document for storing the nGram information of a file
//Adds methods for dealing with Maximum Likelyhood probabilities

#ifndef _T_MLDOCUMENT
#define _T_MLDOCUMENT

#include "ml_document.h"

namespace nlp {

  //Computes the prabability of an ngram occuring in the document
  template <typename Type> double MLDocument<Type>::ngramProbability(std::vector<Type> * ngram_in) {
    //return (double) (((double) countNgram(ngram_in)) / ((double) this->numNgrams(ngram_in->size())));
    return (double) (((double) this->countNgram(ngram_in)) / ((double) this->numNgrams(1)));
  }

  //Computes the probability of an ngram occuring given another ngram
  template <typename Type> double MLDocument<Type>::probabilityGiven(std::vector<Type> * given_in, std::vector<Type> * ngram_in) {
    std::vector<Type> fullSentence;
    int gramIterator;

    //Initially the full sentence begins with the given text
    fullSentence = *given_in;
    //Add the remaining tokens to the sentence
    for (gramIterator = 0; gramIterator < (int) ngram_in->size(); ++gramIterator) {
      fullSentence.push_back((*ngram_in)[gramIterator]);
    }

    //If the sentence does not exist probability is 0
    if (! this->hasNgram(&fullSentence)) {
      return 0;
    }

    return ((double) ((double) this->countNgram(&fullSentence)) / ((double) this->countNgram(given_in)));
  }

  //Computes the probability of a token occuring given an ngram that precedes it
  template <typename Type> double MLDocument<Type>::probabilityGiven(std::vector<Type> * given_in, Type * ngram_in) {
    std::vector<Type> fullSentence;

    fullSentence = *given_in;
    fullSentence.push_back(*ngram_in);

    //If the sentence does not exist probability is 0
    if (! this->hasNgram(&fullSentence)) {
      return 0;
    }

    return ((double) ((double) this->countNgram(&fullSentence)) / ((double) this->countNgram(given_in)));
  }

  //Computes the probability of a sentence occuring based
  template <typename Type> double MLDocument<Type>::sentenceProbability(int length_in, std::vector<Type> * sentence_in) {
    std::vector<Type> currentNgram;
    int tokenIterator;
    double result;

    //Initially probability is simply probability of first word
    currentNgram.push_back((*sentence_in)[0]);
    result = ngramProbability(&currentNgram);

    for (tokenIterator = 1; tokenIterator < (int) sentence_in->size(); ++tokenIterator) {
      //Trim any excess tokens from the current given nGram
      while ((int) currentNgram.size() >= length_in) {
        currentNgram.erase(currentNgram.begin());
      }
      result = result * probabilityGiven(&currentNgram, &((*sentence_in)[tokenIterator]));
      //Add the new token to the end of the nGram
      currentNgram.push_back((*sentence_in)[tokenIterator]);
    }

    return result;
  }

  //Computes the probability of a sentence occuring based on the maximum-likliehood language model
  template <typename Type> double MLDocument<Type>::logSentenceProbability(int length_in, std::vector<Type> * sentence_in) {
    std::vector<Type> currentNgram;
    int tokenIterator;
    double result;
    
    //Initially probability is simply probability of first word
    currentNgram.push_back((*sentence_in)[0]);
    result = log(ngramProbability(&currentNgram));

    for (tokenIterator = 1; tokenIterator < (int) sentence_in->size(); ++tokenIterator) {
      //Trim any excess tokens from the current given nGram
      while ((int) currentNgram.size() >= length_in) {
        currentNgram.erase(currentNgram.begin());
      }
      result = result + log(probabilityGiven(&currentNgram, &((*sentence_in)[tokenIterator])));
      //Add the new token to the end of the nGram
      currentNgram.push_back((*sentence_in)[tokenIterator]);
    }

    return result;
  }

  //Creates a probability distrubution for each ngram of the specified length in the dictionary.
  template <typename Type> int MLDocument<Type>::makeDistrubution(int length_in, std::vector<std::vector<Type>> * ngramList, std::vector<double> * probabilityList) {
    int index;

    index = this->getIndex(length_in);

    for (auto iterator = this->dictionary[index].begin(); iterator != this->dictionary[index].end(); ++iterator) {
      //Extract the current ngram we are iteratin over
      std::vector<Type> currentNgram = iterator->first;
      //Push the ngram to the list of ngrams
      ngramList->push_back(currentNgram);
      //Push the probability to the list of probabilities
      probabilityList->push_back(ngramProbability(&currentNgram));
    }
    
    return 0;
  }

  template <typename Type> int MLDocument<Type>::makeDistrubution(std::vector<std::vector<Type>> * wordList, std::vector<Type> * given_in, std::vector<Type> * choices, std::vector<double> * probabilityList) {
    int wordIterator;
    std::vector<Type> fullNgram;
    Type currentOption;

    for (wordIterator = 0; wordIterator < (int) wordList->size(); wordIterator++) {
      currentOption = (*wordList)[wordIterator][0];
      //Create the trian ngram by adding the test value to the given
      fullNgram = *given_in;
      fullNgram.push_back(currentOption);
      //Check if the current nGram exists in the dictionary
      if (this->hasNgram(&fullNgram)) {
        //Add the current word to the list of choices
        choices->push_back(currentOption);
        //Add the probability of the word occuring to the probability list
        probabilityList->push_back(probabilityGiven(given_in, &currentOption));
      }
    }

    return 0;
  }

  //Generates a random ngram from the document and stores it in the specified location
  template <typename Type> int MLDocument<Type>::generateSentence(int length_in, std::vector<Type> * location_in, int sentencePrefix) {
    std::vector<Type> result;
    std::vector<std::vector<Type>> words;
    std::vector<double> probabilities;
    std::vector<Type> currentNgram;

    //Reseed the random generator
    srand(time(NULL));

    //Make a distrubution of all the words in the dictionary
    makeDistrubution(1, &words, &probabilities);

    //Sentence generation with specified context size requires ngrams of said size
    this->readTokens(length_in);

    //Check if a proper sentence prefix is to be generated
    if (sentencePrefix) {
      //Prefix requires ngrams of size two to be in the dictionary
      this->readTokens(SENTENCE_PREFIX_NGRAM_LENGTH);

      //Create the structures to hold the options to hold from
      std::vector<Type> startOptions;
      std::vector<double> startProbabilities;

      currentNgram.push_back(EOS);

      //Create a distrubution across the current words
      makeDistrubution(&words, &currentNgram, &startOptions, &startProbabilities);
      result.push_back(startOptions[getRandomIndex(&startProbabilities)]);
    } else {
      //If we do not want a proper setence prefix just randomly select a first word
      result.push_back(words[getRandomIndex(&probabilities)][0]);
    }

    //Length of 1 is a special case as it requires no context
    if (length_in == 1) {
      while (result[result.size() - 1] != EOS) {
        result.push_back(words[getRandomIndex(&probabilities)][0]);
      }
      *location_in = result;
      return 0;
    }

    currentNgram.push_back(result[0]);

    //Continue to add words to the sentence until an EOS is found
    while (result[result.size() - 1] != EOS) {
      //Trim any excess tokens from the current nGram
      while ((int) currentNgram.size() >= length_in) {
        currentNgram.erase(currentNgram.begin());
      }
      
      //Create the structures to hold the options to hold from
      std::vector<Type> wordOptions;
      std::vector<double> probabilityOptions;

      //Create a distrubution across the current words
      makeDistrubution(&words, &currentNgram, &wordOptions, &probabilityOptions);
      result.push_back(wordOptions[getRandomIndex(&probabilityOptions)]);

      //Append the new word to the current ngram
      currentNgram.push_back(result[result.size() - 1]);
    }

    //Store the created sentence in the specified location
    *location_in = result;
    return 0;
  }

  //Inherited constructors
  template <typename Type> MLDocument<Type>::MLDocument(std::vector<Type> * tokens_in, int gramLength_in)
    :Document<Type>(tokens_in, gramLength_in) {}
  template <typename Type> MLDocument<Type>::MLDocument(std::vector<Type> * tokens_in, int gramLengthLow_in, int gramLengthHigh_in)
    :Document<Type>(tokens_in, gramLengthLow_in, gramLengthHigh_in) {}
  template <typename Type> MLDocument<Type>::MLDocument(std::vector<Type> * tokens_in, std::vector<int> * gramLengths_in)
    :Document<Type>(tokens_in, gramLengths_in) {}

  //Default constructor and destructor
  template <typename Type> MLDocument<Type>::MLDocument() {}
  template <typename Type> MLDocument<Type>::~MLDocument() {}

};

//Randomly selects an index from a weighted probability set
int getRandomIndex(std::vector<double> * probabilities_in) {
  double accumulator;
  int probabilityIterator;
  int numProbabilities;
  double randomVal;

  //Store the amount of probabilities to select from
  numProbabilities = probabilities_in->size();
  randomVal = ((double) (rand() % numProbabilities) ) / (double) numProbabilities;

  accumulator = 0;
  for (probabilityIterator = 0; probabilityIterator < numProbabilities; probabilityIterator++) {
    accumulator += (*probabilities_in)[probabilityIterator];
    if (accumulator > randomVal) {
      return probabilityIterator;
    }
  }

  return numProbabilities - 1;
}

#endif