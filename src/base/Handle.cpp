
#include "logi/base/Handle.h"

namespace logi {

Handle::Handle(const bool valid) : data_(std::make_shared<Data>(valid, id_generator_++)) {}

size_t Handle::id() const {
  return data_->id;
}

bool Handle::operator==(const Handle& other) const {
  return data_->id == other.id();
}

Handle::operator bool() const {
  return data_->valid;
}

bool Handle::operator!() const {
  return !data_->valid;
}

bool Handle::valid() const {
  return data_->valid;
}

void Handle::free() {
  data_->valid.store(false, std::memory_order_release);
}

std::atomic<size_t> Handle::id_generator_ = 0;

Handle::Data::Data(bool valid, size_t id) : valid(valid), id(id) {}

void DestroyableHandle::destroy() {
  free();
}

} // namespace logi
