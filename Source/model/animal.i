/* animal.i file */
%module AnimalModule
%include <std_string.i>
%include <typemaps.i>
%apply const std::string & {std::string &};

%{
  #include "Animal.hpp"
%}

%include "Animal.hpp"
