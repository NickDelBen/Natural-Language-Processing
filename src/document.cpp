//A document for storing the nGram information of a file

#include "document.h"

//Creates a new instance of Document
Document::Document(std::vector<Type> * tokens_in, int gramLength_in) {
  int lengthIterator;
  std::vector<int> lengths;

  //Create the vector of lengts to be considered
  for (lengthIterator = 1; lengthIterator <= gramLength_in; ++lengthIterator) {
    lengths.push_back(lengthIterator);
  }

  init_object(tokens_in, &lengths);
}

//Creates a new instance of Document finding all the ngrams from a specified size to another specified size
Document::Document(std::vector<Type> * tokens_in, int gramLengthLow_in, int gramLengthHigh_in) {
  int lengthIterator;
  std::vector<int> lengths;

  //Create the vector of lengts to be considered
  for (lengthIterator = gramLengthLow_in; lengthIterator <= gramLengthHigh_in; ++lengthIterator) {
    lengths.push_back(lengthIterator);
  }

  init_object(tokens_in, &lengths);
}

//Creates a new instance of Document finding all the ngrams of the sizes specified in the input vector
Document::Document(std::vector<Type> * tokens_in, std::vector<int> * gramLengths_in) {
  init_object(tokens_in, gramLengths_in);
}

//Initilizes the values of the object
int Document::init_object(std::vector<Type> * tokens_in, std::vector<int> * gramLengths_in) {
  syslog(LOG_DEBUG, "Document::init_object: Creating new instance of Document from %d tokens", (int) tokens_in->size());

  int tokenIterator;
  int hashmapIterator;
  int numGramSizes;
  int currentToken;

  //Sort the ngram sizes
  std::sort(gramLengths_in->begin(), gramLengths_in->end());
  //Save the number of tokens in this document
  numTokens = tokens_in->size();
  //Save the input tokens to the document
  tokens = *tokens_in;

  readTokens(gramLengths_in);
  
  return 0;
}

//Reads tokens from lengths 1 to the specified length from the document's tokens
int Document::readTokens(int length_in) {
  std::vector<int> lengths;
  int lengthIterator;

  for (lengthIterator = 1; lengthIterator <= length_in; ++lengthIterator) {
    lengths.push_back(lengthIterator);
  }

  readTokens(&lengths);

  return 0;
}

//Reads tokens of the specified lengths from the document's tokens
int Document::readTokens(std::vector<int> * lengths_in) {
  int lengthIterator;
  std::vector<int> lengths;
  int tokenIterator;
  int currentToken;
  int dictionaryIndex;
  int numGramSizes;

  syslog(LOG_DEBUG, "Document::readTokens: removing specified lengths that already exist");
  lengths = *lengths_in;
  //Check if each length is alread in the dictionary
  lengthIterator = 0;
  while (lengthIterator < lengths.size()) {
    if (hasNgrams(lengths[lengthIterator])) {
      lengths.erase(lengths.begin() + lengthIterator);
      continue;
    }
    ++lengthIterator;
  }

  syslog(LOG_DEBUG, "Document::readTokens: Adding hashmaps to dictionary");
  numGramSizes = lengths.size();
  for (lengthIterator = 0; lengthIterator < numGramSizes; ++lengthIterator) {
    syslog(LOG_DEBUG, "Document::readTokens: Creating hash table for ngrams of length '%d'", lengths[lengthIterator]);
    //Create a new nashmap and add it to the dictionary
    std::unordered_map<std::vector<Type>, int> newMap;
    dictionary.push_back(newMap);
    //Add the ngram length of this hashmap to the list
    ngramLengths.push_back(lengths[lengthIterator]);
  }

  syslog(LOG_DEBUG, "Document::readTokens: Processing tokens into ngrams");
  //Add ngrams to dictionary
  for (tokenIterator = 0; tokenIterator < numTokens; ++tokenIterator) {
    //Create a new nGram
    currentToken = 0;
    std::vector<Type> newGram;
    for (lengthIterator = 0; lengthIterator < numGramSizes; ++lengthIterator) {
      //Check to make sure we are not going out of bounds
      if (tokenIterator + lengths[lengthIterator] > numTokens) {
        break;
      }
      //Append tokens to the ngram until it matches the specified length
      while (currentToken < lengths[lengthIterator]) {
        newGram.push_back(tokens[tokenIterator + currentToken++]);
      }
      dictionaryIndex = getIndex(lengths[lengthIterator]);
      //Push the current ngram to the correct gram length matcher
      if (dictionary[dictionaryIndex].count(newGram) == 0) {
        //If nGram is not in dictionary add it as count 1
        dictionary[dictionaryIndex][newGram] = 1;
      } else {
        //If ngram is in dictionary increase count
        dictionary[dictionaryIndex][newGram] = dictionary[dictionaryIndex][newGram] + 1;
      }
    }
  }

  syslog(LOG_DEBUG, "Document::readTokens: Completed processing tokens to ngrams");
  return 0;
}

//Returns the number of ngrams of a specified length in the document
int Document::numNgrams(int length_in) {
  return numTokens + 1 - length_in;
}

//Returns the number of disctict ngrams in the document
int Document::numDistinctNgrams(int length_in) {
  syslog(LOG_DEBUG, "Document::numDistinctNgrams: Checking how many disctict ngrams of length %d are in dictionary", length_in);
  int index;

  index = getIndex(length_in);
  return dictionary[index].size();
}

//Finds the occurances of an ngram in the document
int Document::countNgram(std::vector<Type> * nGram_in) {
  int index;

  //If the ngram does not occur we can return 0 right away
  if (! hasNgram(nGram_in)) {
    return 0;
  }

  index = getIndex(nGram_in->size());
  return dictionary[index][*nGram_in];
}

//Checks if an nGram occurs in this document
int Document::hasNgram(std::vector<Type> * nGram_in) {
  syslog(LOG_DEBUG, "Document::hasNgram checking if document has specified ngram");
  int index;

  index = getIndex(nGram_in->size());
  syslog(LOG_DEBUG, "Document::hasNgram found index %d in dictionary of size %d", index, dictionary.size());
  return dictionary[index].count(*nGram_in) > 0 ? 1 : 0;
}

//Adds an nGram to the dictionary
int Document::addNgram(std::vector<Type> * nGram_in) {
  syslog(LOG_DEBUG, "Document::addNgram: Adding new ngram of length %d", (*nGram_in).size());
  int index;
  
  index = getIndex(nGram_in->size());
  //Add the nGram to the database
  if (dictionary[index].count(*nGram_in) == 0) {
    //If nGram is not in dictionary add it as count 1
    dictionary[index][*nGram_in] = 1;
  } else {
    //If ngram is in dictionary increase count
    dictionary[index][*nGram_in] = dictionary[index][*nGram_in] + 1;
  }
  return 0;
}

//Finds the index of a ngram length in the dictionary
int Document::getIndex(int length_in) {
  auto result = std::find(ngramLengths.begin(), ngramLengths.end(), length_in);
  return std::distance(ngramLengths.begin(), result);
}

//Finds the number of unique ngram lengths stored in this document
int Document::numLengths() {
  return dictionary.size();
}

//Finds the number of ngrams in this document that are also in the specified document
int Document::numCommon(int length_in, Document * document_in) {
  syslog(LOG_DEBUG, "Document::numCommon: Finding common grams between documents");
  int result;
  int index;

  result = 0;
  index = getIndex(length_in);

  syslog(LOG_DEBUG, "Document::numCommon: Searching for ngrams of length %d stored at index %d", length_in, index);

  for (auto iterator = dictionary[index].begin(); iterator != dictionary[index].end(); ++iterator) {
    std::vector<Type> currentNgram = iterator->first;
    if (document_in->hasNgram(&currentNgram)) {
      ++result;
    }
  }

  return result;
}

//Shows the ngrams of a specified length in this document that are also in a specifeid document
int Document::findCommon(int length_in, Document * document_in, std::vector<const std::vector<Type>*> * result_in) {
  syslog(LOG_DEBUG, "Document::findCommon: Finding common ngrams between two files");

  int index;
  int result;

  result = 0;
  index = getIndex(length_in);

  for (auto iterator = dictionary[index].begin(); iterator != dictionary[index].end(); ++iterator) {
    std::vector<Type> currentNgram = iterator->first;
    //Check if the ngram is in the specified document
    if (document_in->hasNgram(&currentNgram)) {
      //Add pointer to the ngram to the result list
      result_in->push_back(&iterator->first);
      //Increase the counter
      ++result;
    }
  }

  return result;
}

//Checks if the document contains the specified sentence
int Document::hasSentence(int ngramLength_in, std::vector<Type> * sentence_in) {
  std::vector<Type> current;
  int sentenceIterator;

  for (sentenceIterator = 0; sentenceIterator < sentence_in->size(); ++sentenceIterator) {
    //Push the next word in the sentence on to the ngram to be checked
    current.push_back((*sentence_in)[sentenceIterator]);
    //If the sentence is too long remove the first word
    if (current.size() > ngramLength_in) {
      current.erase(current.begin());
    }
    //Check if the ngram occurs in the dictionary
    if (! hasNgram(&current)) {
      return 0;
    }
  }

  return 1;
}

//Checks if grams of the specified length have been added to the dictionary
int Document::hasNgrams(int length_in) {
  syslog(LOG_DEBUG, "Document::hasNgrams: Checking if ngrams of length %d have been added to dictionary", length_in);
  //Check if the length is valid
  if (length_in <= 0) {
    syslog(LOG_DEBUG, "Document::hasNgrams: Specified length is invalid");
    return -1;
  }
  //Check if any ngrams have been read
  if (ngramLengths.empty()) {
    syslog (LOG_DEBUG, "Document::hasNgrams: Dictionary has no ngrams");
    return 0;
  }
  //Check if the length has been read
  if(std::find(ngramLengths.begin(), ngramLengths.end(), length_in) != ngramLengths.end()) {
    syslog (LOG_DEBUG, "Document::hasNgrams: Ngram length was found in dictionary");
    return 1;
  }
  syslog (LOG_DEBUG, "Document::hasNgrams: Ngram length not found in dictionary");
  //The length has not been read
  return 0;
}

//Default constructor and destructor
Document::Document() {}
Document::~Document() {}