/* <tools/nycr/symbol/member_with_kind.h>

   A member with no specific name.

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

#include <base/no_copy_semantics.h>
#include <tools/nycr/symbol/compound.h>

namespace Tools {

  namespace Nycr {

    namespace Symbol {

      /* TODO */
      class TMemberWithKind
          : public TCompound::TMember {
        NO_COPY_SEMANTICS(TMemberWithKind);
        public:

        /* TODO */
        TKind *GetKind() const {
          assert(this);
          return Kind;
        }

        /* TODO */
        virtual const TKind *TryGetKind() const;

        protected:

        /* TODO */
        TMemberWithKind(TKind *kind)
            : Kind(kind) {
          assert(kind);
        }

        private:

        /* TODO */
        TKind *Kind;

      };  // TMemberWithKind

    }  // Symbol

  }  // Nycr

}  // Tools
