/* <orly/symbol/param_def.h>

   TODO

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

#include <string>

#include <base/no_copy_semantics.h>
#include <orly/pos_range.h>
#include <orly/symbol/function.h>
#include <orly/type/impl.h>

namespace Orly {

  namespace Symbol {

    class TParamDef
        : public TFunction::TParamDef {
      NO_COPY_SEMANTICS(TParamDef);
      public:

      TFunction::TPtr GetFunction() const;

      Type::TType GetType() const;

      TFunction::TPtr TryGetFunction() const;

      protected:

      TParamDef(
          const TFunction::TPtr &function,
          const std::string &name,
          const Type::TType &type,
          const TPosRange &pos_range);

      private:

      std::weak_ptr<TFunction> Function;

      Type::TType Type;

    };  // ParamDef

  }  // Symbol

}  // Orly
