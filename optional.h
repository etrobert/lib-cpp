#ifndef OPTIONAL_H
#define OPTIONAL_H

template<typename T>
class optional {
  public:
    optional(void) :
      _initialized(false) {}
    optional(const T& val) :
      _initialized(true) {
      ::new (_data.data()) T(val);
    }
    ~optional(void) {
      if (_initialized)
        data()->~T();
    }
    T& operator*(void) {
      return *data();
    }
    T* operator->(void) {
      return data();
    }
  private:
    T* data(void) { return reinterpret_cast<T*>(_data.data()); }
    std::array<std::uint8_t, sizeof(T)> _data;
    bool                                _initialized;
};

#endif // OPTIONAL_H
