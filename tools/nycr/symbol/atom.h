/* <tools/nycr/symbol/atom.h>

   A leaf token.

   Copyright 2010-2014 OrlyAtomics, Inc.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */

#pragma once

#include <cassert>
#include <string>

#include <base/no_copy_semantics.h>
#include <base/opt.h>
#include <tools/nycr/symbol/final.h>

namespace Tools {

  namespace Nycr {

    namespace Symbol {

      /* TODO */
      class TAtom
          : public TFinal {
        public:

        /* TODO */
        const std::string &GetPattern() const {
          assert(this);
          return Pattern;
        }

        /* TODO */
        const Base::TOpt<int> &GetPri() const {
          assert(this);
          return Pri;
        }

        /* TODO */
        int GetPriAsInt() const {
          assert(this);
          return Pri ? *Pri : 0;
        }

        protected:

        /* TODO */
        TAtom(const TName &name, TAnyBase *base, const std::string &pattern, const Base::TOpt<int> &pri)
            : TFinal(name, base), Pattern(pattern), Pri(pri) {}

        private:

        /* TODO */
        std::string Pattern;

        /* TODO */
        Base::TOpt<int> Pri;

      };  // TAtom

    }  // Symbol

  }  // Nycr

}  // Tools
