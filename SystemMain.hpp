#ifndef DEF_SYSTEMMAIN_HPP
#define DEF_SYSTEMMAIN_HPP

class SystemMain {
  public:
    SystemMain() = default;
    ~SystemMain() = default;
    bool initialize();
    void finalize() const;
    void main() const;
};

#endif