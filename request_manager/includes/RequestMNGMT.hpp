
#ifndef REQUESTMNGMT_HPP
# define REQUESTMNGMT_HPP

#include 

# include <string>
# include <cstring>
# include <sstream>
# include <vector>
# include <iostream>
# include <map>
# include <exception>
# include "../includes/RequestLine.hpp"
#include "Request.hpp"

class RequestMNGMT {

    public:
        static int      requestTypeSwitch(Request request);
};


#endif