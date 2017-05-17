/**
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "core/repository/VolatileRepository.h"
#include <memory>
#include <string>
#include <vector>
#include "FlowFileRecord.h"

namespace org {
namespace apache {
namespace nifi {
namespace minifi {
namespace core {
namespace repository {

const char *VolatileRepository::volatile_repo_max_count =
    "max.count";

void VolatileRepository::run() {
  repo_full_ = false;
}

/**
 * Purge
 */
void VolatileRepository::purge() {
  while (current_size_ > max_size_) {
    for (auto ent : value_vector_) {
      // let the destructor do the cleanup
      RepoValue value;
      if (ent->getValue(value)) {
        current_size_ -= value.size();
        logger_->log_info("VolatileRepository -- purge %s %d %d %d",
                          value.getKey(), current_size_.load(), max_size_,
                          current_index_.load());
      }
      if (current_size_ < max_size_)
        break;
    }
  }
}

void VolatileRepository::loadComponent() {
}

} /* namespace repository */
} /* namespace core */
} /* namespace minifi */
} /* namespace nifi */
} /* namespace apache */
} /* namespace org */