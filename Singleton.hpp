#ifndef DEF_SINGLETON_HPP
#define DEF_SINGLETON_HPP

template <typename T> 
class Singleton {
  protected: 
    Singleton() = default;
    virtual ~Singleton() = default;
    Singleton(const Singleton& r) = default;
    Singleton& operator=(const Singleton& r) = default;

  public: 
    static T* getIns() {
      static T inst;
      return &inst;
    };
};

#endif