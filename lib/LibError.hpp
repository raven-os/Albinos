#pragma once

# include <exception>

class   LibError: public std::exception
{

public:

  LibError(int errorCode = -1);
  ~LibError(void) throw() {}

  int getCode(void) const throw();

private:

  int code;

};
