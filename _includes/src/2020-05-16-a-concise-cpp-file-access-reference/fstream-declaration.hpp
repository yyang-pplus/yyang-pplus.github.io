template<class CharT, class Traits = std::char_traits<CharT>>
class basic_ifstream : public std::basic_istream<CharT, Traits>;

template<class CharT, class Traits = std::char_traits<CharT>>
class basic_ofstream : public std::basic_ostream<CharT, Traits>;

template<class CharT, class Traits = std::char_traits<CharT>>
class basic_fstream : public std::basic_iostream<CharT, Traits>;

template<class CharT, class Traits = std::char_traits<CharT>>
class basic_filebuf : public std::basic_streambuf<CharT, Traits>;
