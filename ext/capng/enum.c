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

void
Init_capng_enum(VALUE rb_cCapNG)
{
  Init_capng_enum_action(rb_cCapNG);
  Init_capng_enum_flags(rb_cCapNG);
  Init_capng_enum_result(rb_cCapNG);
  Init_capng_enum_select(rb_cCapNG);
  Init_capng_enum_type(rb_cCapNG);
}
