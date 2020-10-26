/* capng_c */
/* Copyright 2020- Hiroshi Hatake*/
/* */
/* Licensed under the Apache License, Version 2.0 (the "License"); */
/* you may not use this file except in compliance with the License. */
/* You may obtain a copy of the License at */
/*     http://www.apache.org/licenses/LICENSE-2.0 */
/* Unless required by applicable law or agreed to in writing, software */
/* distributed under the License is distributed on an "AS IS" BASIS, */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. */
/* See the License for the specific language governing permissions and */
/* limitations under the License. */

#include <capng.h>

void Init_capng_enum(VALUE rb_cCapNG)
{
  rb_mAction = rb_define_module_under(rb_cCapNG, "Action");
  rb_mSelect = rb_define_module_under(rb_cCapNG, "Select");
  rb_mType = rb_define_module_under(rb_cCapNG, "Type");
  rb_mResult = rb_define_module_under(rb_cCapNG, "Result");
  rb_mFlags = rb_define_module_under(rb_cCapNG, "Flags");

  // capng_cat_t enum constants
  rb_define_const(rb_mAction, "DROP", INT2NUM(CAPNG_DROP));
  rb_define_const(rb_mAction, "ADD", INT2NUM(CAPNG_ADD));

  // capng_select_t enum constants
  rb_define_const(rb_mSelect, "CAPS", INT2NUM(CAPNG_SELECT_CAPS));
  rb_define_const(rb_mSelect, "BOUNDS", INT2NUM(CAPNG_SELECT_BOUNDS));
  rb_define_const(rb_mSelect, "BOTH", INT2NUM(CAPNG_SELECT_BOTH));
#if defined(CAPNG_SELECT_AMBIENT)
  rb_define_const(rb_mSelect, "AMBIENT", INT2NUM(CAPNG_SELECT_AMBIENT));
#endif
#if defined(CAPNG_SELECT_ALL)
  rb_define_const(rb_mSelect, "ALL", INT2NUM(CAPNG_SELECT_ALL));
#endif

  // capng_type_t enum constants
  rb_define_const(rb_mType, "EFFECTIVE", INT2NUM(CAPNG_EFFECTIVE));
  rb_define_const(rb_mType, "PERMITTED", INT2NUM(CAPNG_PERMITTED));
  rb_define_const(rb_mType, "INHERITABLE", INT2NUM(CAPNG_INHERITABLE));
  rb_define_const(rb_mType, "BOUNDING_SET", INT2NUM(CAPNG_BOUNDING_SET));
#if defined(CAPNG_AMBIENT)
  rb_define_const(rb_mType, "AMBIENT", INT2NUM(CAPNG_AMBIENT));
#endif

  // capng_result_t enum constants
  rb_define_const(rb_mResult, "FAIL", LONG2NUM(CAPNG_FAIL));
  rb_define_const(rb_mResult, "NONE", LONG2NUM(CAPNG_NONE));
  rb_define_const(rb_mResult, "PARTIAL", LONG2NUM(CAPNG_PARTIAL));
  rb_define_const(rb_mResult, "FULL", LONG2NUM(CAPNG_FULL));

  // capng_flags_t enum constants
  rb_define_const(rb_mFlags, "NO_FLAG", LONG2NUM(CAPNG_NO_FLAG));
  rb_define_const(rb_mFlags, "DROP_SUPP_GRP", LONG2NUM(CAPNG_DROP_SUPP_GRP));
  rb_define_const(rb_mFlags, "CLEAR_BOUNDING", LONG2NUM(CAPNG_CLEAR_BOUNDING));
  rb_define_const(rb_mFlags, "INIT_SUPP_GRP", LONG2NUM(CAPNG_INIT_SUPP_GRP));
}
