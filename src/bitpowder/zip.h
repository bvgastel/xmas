#ifndef LIB_ZIP_H
#define LIB_ZIP_H

template <class S, class T>
struct zip_iterator : std::iterator<std::forward_iterator_tag,
                                    std::pair<const typename S::value_type &,
                                              const typename T::value_type &>> {
  friend bool operator!=(const zip_iterator<S, T> &a,
                         const zip_iterator<S, T> &b) {
    return a.s_ != b.s_ && a.t_ != b.t_;
  }

  zip_iterator(S s, T t) : s_(s), t_(t) {}

  zip_iterator& operator++() {
    ++s_;
    ++t_;
    return *this;
  }

  std::pair<const typename S::value_type&, const typename T::value_type&>
  operator*() {
    return {*s_, *t_};
  }

 private:
  S s_;
  T t_;
};

template <class S, class T>
struct zip_impl {
  zip_impl(S &s, T &t) : s_(s), t_(t) {}

  zip_iterator<typename S::const_iterator, typename T::const_iterator> begin() {
    return zip_iterator<decltype(s_.begin()), decltype(t_.begin())>(s_.begin(), t_.begin());
  }

  zip_iterator<typename S::const_iterator, typename T::const_iterator> end() {
    return zip_iterator<typename S::const_iterator, typename T::const_iterator>(s_.end(), t_.end());
  }

private:
  S& s_;
  T& t_;
};

template <class S, class T>
zip_impl<S, T> zip(S &s, T &t) {
  return zip_impl<S, T>(s, t);
}

#endif