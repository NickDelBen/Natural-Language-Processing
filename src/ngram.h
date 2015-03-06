//Question 1

#ifndef _H_NGRAM
#define _H_NGRAM

#include <vector>  //std::vector
#include <string>  //std::string
#include <sstream> //stream

#ifndef TYPE_NGRAM
#define TYPE_NGRAM
typedef std::string Type;
#endif

int toString(std::vector<Type> * ngram_in, std::string * location_in) {
  std::stringstream stream;
  int elementIterator;
  std::string delimeter;
  
  delimeter = sizeof(Type) == 1 ? "" : " ";
  //Insert the first ngram
  stream << (*ngram_in)[0];
  //Insert the remaining ngrams, delimited by a space
  for (elementIterator = 1; elementIterator < (int) ngram_in->size(); ++elementIterator) {
    stream << delimeter << (*ngram_in)[elementIterator];
  }
  //Store the result stream in the specified location
  *location_in = stream.str();

  return 0;
}

int toString(const std::vector<Type> * ngram_in, std::string * location_in) {
  std::stringstream stream;
  int elementIterator;
  std::string delimeter;
  
  delimeter = sizeof(Type) == 1 ? "" : " ";
  //Insert the first ngram
  stream << (*ngram_in)[0];
  //Insert the remaining ngrams, delimited by a space
  for (elementIterator = 1; elementIterator < (int) ngram_in->size(); ++elementIterator) {
    stream << delimeter << (*ngram_in)[elementIterator];
  }
  //Store the result stream in the specified location
  *location_in = stream.str();

  return 0;
}

//Overloads output stream operator for this object
std::ostream & operator<<(std::ostream & output, std::vector<Type> & ngram) {
  std::string representation;

  //Get string representation of the object
  toString(&ngram, &representation);
  //Push the new information to the stream
  output << representation;
  //Add string to output stream and return new stream
  return output;
}

//Overloads output stream operator for this object
std::ostream & operator<<(std::ostream & output, const std::vector<Type> & ngram) {
  std::string representation;

  //Get string representation of the object
  toString(&ngram, &representation);
  //Push the new information to the stream
  output << representation;
  //Add string to output stream and return new stream
  return output;
}

#endif