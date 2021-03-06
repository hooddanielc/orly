/* <orly/expr/match.h>

   Matches (all for now) occurrences of a regular expression in a string

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

#include <memory>

#include <base/no_copy_semantics.h>
#include <orly/expr/binary.h>

namespace Orly {

  namespace Expr {

    class TMatch
        : public TBinary {
      NO_COPY_SEMANTICS(TMatch);
      public:

      typedef std::shared_ptr<TMatch> TPtr;

      static TExpr::TPtr New(const TExpr::TPtr &lhs, const TExpr::TPtr &rhs, const TPosRange &pos_range);

      virtual void Accept(const TVisitor &visitor) const;

      virtual Type::TType GetType() const;

      private:

      TMatch(const TExpr::TPtr &lhs, const TExpr::TPtr &rhs, const TPosRange &pos_range);

    };  // TMatch

  }  // Expr

}  // Orly
