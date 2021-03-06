/* <orly/synth/random_int.h>

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

#include <base/no_copy_semantics.h>
#include <orly/orly.package.cst.h>
#include <orly/synth/expr.h>

namespace Orly {

  namespace Synth {

    class TRandomInt : public TExpr {
      NO_COPY_SEMANTICS(TRandomInt);
      public:

      TRandomInt(const Package::Syntax::TBuiltInRandom *random_int);

      virtual Expr::TExpr::TPtr Build() const;

      private:
      const Package::Syntax::TBuiltInRandom *RandomInt;
    };

  } // Synth

} // Orly
