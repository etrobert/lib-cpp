#pragma once

#include <streambuf>
#include <ostream>

class nullbuff : public std::streambuf {
  public:
    int overflow(int c) { return c; }
};

class nullstream : public std::ostream {
  public:
    static nullstream& instance() {
      static nullstream ns;
      return ns;
    }
  private:
    nullstream() : std::ostream(&nb) {}
    nullbuff nb;
};
