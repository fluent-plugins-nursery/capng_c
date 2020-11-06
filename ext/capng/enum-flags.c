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

/* clang-format off */
/*
 * Document-module: CapNG::Flags
 *
 * Define NO_FLAGS, DROP_SUPP_GRP, CLEAR_BOUNDING, and INIT_SUPP_GRP (if available) constants.
 */
/* clang-format on */

#include <capng.h>

void Init_capng_enum_flags(VALUE rb_cCapNG)
{
  VALUE rb_mFlags = rb_define_module_under(rb_cCapNG, "Flags");

  // capng_flags_t enum constants
  /* Simply change uid and retain specified capabilities and that's
   * all. */
  rb_define_const(rb_mFlags, "NO_FLAG", LONG2NUM(CAPNG_NO_FLAG));
  /* it will have no effect. */
  rb_define_const(rb_mFlags, "DROP_SUPP_GRP", LONG2NUM(CAPNG_DROP_SUPP_GRP));
  /* Clear the bounding set regardless to the internal representation
   * already setup prior to changing the uid/gid.*/
  rb_define_const(rb_mFlags, "CLEAR_BOUNDING", LONG2NUM(CAPNG_CLEAR_BOUNDING));
#if defined(HAVE_CONST_CAPNG_INIT_SUPP_GRP)
  /* After changing id, initialize any supplement groups that may come with the new account. If given with
   *  Note: Ubuntu Trusty's libcap-ng-dev doesn't have CAPNG_INIT_SUPP_GRP
   * constant. */
  rb_define_const(rb_mFlags, "INIT_SUPP_GRP", LONG2NUM(CAPNG_INIT_SUPP_GRP));
#endif
}
