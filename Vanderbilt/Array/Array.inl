INLINE bool Array::in_range(size_t index) const { return index < cur_size_; }
INLINE size_t Array::size() const { return cur_size_; }

INLINE const T &Array::operator[](size_t index) const { return array_[index]; }
INLINE T &Array::operator[](size_t index) { return array_[index]; }
