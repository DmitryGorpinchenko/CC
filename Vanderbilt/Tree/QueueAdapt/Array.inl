template <typename T> INLINE size_t Array<T>::size() const { return cur_size_; }
template <typename T> INLINE bool Array<T>::in_range(size_t index) const { return index < cur_size_; }

template <typename T> INLINE T &Array<T>::operator[](size_t index) { return array_[index]; }
template <typename T> INLINE const T &Array<T>::operator[](size_t index) const { return array_[index]; }

template <typename T> INLINE typename Array<T>::iterator Array<T>::begin() { return iterator(*this); }
template <typename T> INLINE typename Array<T>::iterator Array<T>::end() { return iterator(*this, size()); }
template <typename T> INLINE typename Array<T>::const_iterator Array<T>::begin() const { return const_iterator(*this); }
template <typename T> INLINE typename Array<T>::const_iterator Array<T>::end() const { return const_iterator(*this, size()); }

template <typename T> INLINE T &Array_Iterator<T>::operator*() { return array_[pos_]; }
template <typename T> INLINE T &Array_Iterator<T>::operator*() const { return array_[pos_]; }

template <typename T> INLINE Array_Iterator<T> &Array_Iterator<T>::operator++() { ++pos_; return *this; }
template <typename T> INLINE Array_Iterator<T> Array_Iterator<T>::operator++(int) { auto tmp = *this; ++pos_; return tmp; }
template <typename T> INLINE Array_Iterator<T> &Array_Iterator<T>::operator--() { --pos_; return *this; }
template <typename T> INLINE Array_Iterator<T> Array_Iterator<T>::operator--(int) { auto tmp = *this; --pos_; return tmp; }

template <typename T> INLINE bool Array_Iterator<T>::operator==(const Array_Iterator &rhs) const { return &array_ == &rhs.array_ && pos_ == rhs.pos_; }
template <typename T> INLINE bool Array_Iterator<T>::operator!=(const Array_Iterator &rhs) const { return !(*this == rhs); }

template <typename T> INLINE const T &Const_Array_Iterator<T>::operator*() const { return array_[pos_]; }

template <typename T> INLINE Const_Array_Iterator<T> &Const_Array_Iterator<T>::operator++() { ++pos_; return *this; }
template <typename T> INLINE Const_Array_Iterator<T> Const_Array_Iterator<T>::operator++(int) { auto tmp = *this; ++pos_; return tmp; }
template <typename T> INLINE Const_Array_Iterator<T> &Const_Array_Iterator<T>::operator--() { --pos_; return *this; }
template <typename T> INLINE Const_Array_Iterator<T> Const_Array_Iterator<T>::operator--(int) { auto tmp = *this; --pos_; return tmp; }

template <typename T> INLINE bool Const_Array_Iterator<T>::operator==(const Const_Array_Iterator &rhs) const { return &array_ == &rhs.array_ && pos_ == rhs.pos_; }
template <typename T> INLINE bool Const_Array_Iterator<T>::operator!=(const Const_Array_Iterator &rhs) const { return !(*this == rhs); }

