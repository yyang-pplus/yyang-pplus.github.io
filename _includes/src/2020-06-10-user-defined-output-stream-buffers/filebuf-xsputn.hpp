class filebuf : public std::streambuf {
    virtual std::streamsize xsputn(const char *s, std::streamsize n) override {
        return write(fd, s, n);
    }
};
