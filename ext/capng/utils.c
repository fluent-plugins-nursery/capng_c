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

capng_select_t
select_name_to_select_type(char* select_name)
{
  if (strcmp(select_name, "caps") == 0) {
    return CAPNG_SELECT_CAPS;
  } else if (strcmp(select_name, "bounds") == 0) {
    return CAPNG_SELECT_BOUNDS;
  } else if (strcmp(select_name, "both") == 0) {
    return CAPNG_SELECT_BOTH;
#if defined(HAVE_CONST_CAPNG_SELECT_AMBIENT)
  } else if (strcmp(select_name, "ambient") == 0) {
    return CAPNG_SELECT_AMBIENT;
#endif
#if defined(HAVE_CONST_CAPNG_SELECT_ALL)
  } else if (strcmp(select_name, "all") == 0) {
    return CAPNG_SELECT_ALL;
#endif
  } else {
    rb_raise(rb_eArgError, "unknown select name %s", select_name);
  }
}

capng_act_t
action_name_to_action_type(char* action_name)
{
  if (strcmp(action_name, "drop") == 0) {
    return CAPNG_DROP;
  } else if (strcmp(action_name, "add") == 0) {
    return CAPNG_ADD;
  } else {
    rb_raise(rb_eArgError, "unknown action name %s", action_name);
  }
}

capng_print_t
print_name_to_print_type(char* print_name)
{
  if (strcmp(print_name, "stdout") == 0) {
    return CAPNG_PRINT_STDOUT;
  } else if (strcmp(print_name, "buffer") == 0) {
    return CAPNG_PRINT_BUFFER;
  } else {
    rb_raise(rb_eArgError, "unknown print name %s", print_name);
  }
}

capng_type_t
capability_type_name_to_capability_type(char* capability_name)
{
  if (strcmp(capability_name, "effective") == 0) {
    return CAPNG_EFFECTIVE;
  } else if (strcmp(capability_name, "permitted") == 0) {
    return CAPNG_PERMITTED;
  } else if (strcmp(capability_name, "inheritable") == 0) {
    return CAPNG_INHERITABLE;
  } else if (strcmp(capability_name, "bounding_set") == 0) {
    return CAPNG_BOUNDING_SET;
#if defined(HAVE_CONST_CAPNG_AMBIENT)
  } else if (strcmp(capability_name, "ambient") == 0) {
    return CAPNG_AMBIENT;
#endif
  } else {
    rb_raise(rb_eArgError, "unknown capability name: %s", capability_name);
  }
}
