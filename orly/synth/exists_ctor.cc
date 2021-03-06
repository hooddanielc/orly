/* <orly/synth/exists_ctor.cc>

   Implements <orly/synth/exists_ctor.h>.

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

#include <orly/synth/exists_ctor.h>

#include <cassert>
#include <unordered_map>
#include <vector>

#include <base/assert_true.h>
#include <orly/expr/addr.h>
#include <orly/expr/exists.h>
#include <orly/pos_range.h>
#include <orly/synth/affix_expr.h>
#include <orly/synth/get_pos_range.h>
#include <orly/synth/new_type.h>
#include <orly/synth/new_type.h>
#include <orly/type/impl.h>
#include <orly/type/dict.h>
#include <orly/type/list.h>
#include <orly/type/set.h>
#include <tools/nycr/error.h>

using namespace Orly;
using namespace Orly::Synth;

TExistsCtor::TExistsCtor(const TExprFactory *expr_factory, const Package::Syntax::TPrefixExists *exists_ctor)
    : ExistsCtor(Base::AssertTrue(exists_ctor)) {
  assert(expr_factory);
  try {
    Expr = expr_factory->NewExpr(ExistsCtor->GetExpr());
    ValueType = NewType(ExistsCtor->GetType());
  } catch (...) {
    delete Expr;
    delete ValueType;
    throw;
  }
}

TExistsCtor::~TExistsCtor() {
  assert(this);
  delete ValueType;
}

Expr::TExpr::TPtr TExistsCtor::Build() const {
  assert(this);
  /*auto addr_expr_ptr = Expr->Build();
  Expr::TAddr *k = dynamic_cast<Expr::TAddr *>(addr_expr_ptr.get());*/
  /* If read is on a keys expression, check that the keys value type is the same as the as_type of the read expression */
  /*if (k) {
    Tools::Nycr::TError::TBuilder(GetPosRange(ExistsCtor)) << "exists expression is dereferencing something that is not an address (tuple)";
  }*/
  return Expr::TExists::New(Expr->Build(), ValueType->GetSymbolicType(), GetPosRange(ExistsCtor));
}

void TExistsCtor::ForEachRef(const std::function<void (TAnyRef &)> &cb) {
  assert(this);
  assert(&cb);
  assert(cb);
  assert(ValueType);
  Expr->ForEachRef(cb);
  ValueType->ForEachRef(cb);
}
