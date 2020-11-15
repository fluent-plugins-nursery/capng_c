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

CapabilityInfo capabilityInfoTable[] = {
  {CAP_CHOWN,              "chown"},
  {CAP_DAC_OVERRIDE,       "dac_override"},
  {CAP_DAC_READ_SEARCH,    "dac_read_search"},
  {CAP_FOWNER,             "fowner"},
  {CAP_FSETID,             "fsetid"},
  {CAP_KILL,               "kill"},
  {CAP_SETGID,             "setgid"},
  {CAP_SETUID,             "setuid"},
  {CAP_SETPCAP,            "setpcap"},
  {CAP_LINUX_IMMUTABLE,    "linux_immutable"},
  {CAP_NET_BIND_SERVICE,   "net_bind_service"},
  {CAP_NET_BROADCAST,      "net_broadcast"},
  {CAP_NET_ADMIN,          "net_admin"},
  {CAP_NET_RAW,            "net_raw"},
  {CAP_IPC_LOCK,           "ipc_lock"},
  {CAP_IPC_OWNER,          "ipc_owner"},
  {CAP_SYS_MODULE,         "sys_module"},
  {CAP_SYS_RAWIO,          "sys_rawio"},
  {CAP_SYS_CHROOT,         "sys_chroot"},
  {CAP_SYS_PTRACE,         "sys_ptrace"},
  {CAP_SYS_PACCT,          "sys_pacct"},
  {CAP_SYS_ADMIN,          "sys_admin"},
  {CAP_SYS_BOOT,           "sys_boot"},
  {CAP_SYS_NICE,           "sys_nice"},
  {CAP_SYS_RESOURCE,       "sys_resource"},
  {CAP_SYS_TIME,           "sys_time"},
  {CAP_SYS_TTY_CONFIG,     "sys_tty_config"},
  {CAP_MKNOD,              "mknod"},
  {CAP_LEASE,              "lease"},
  {CAP_AUDIT_WRITE,        "audit_write"},
  {CAP_AUDIT_CONTROL,      "audit_control"},
#ifdef CAP_SETFCAP
  {CAP_SETFCAP,            "setfcap"},
#endif
#ifdef CAP_MAC_OVERRIDE
  {CAP_MAC_OVERRIDE,       "mac_override"},
#endif
#ifdef CAP_MAC_ADMIN
  {CAP_MAC_ADMIN,          "mac_admin"},
#endif
#ifdef CAP_SYSLOG
  {CAP_SYSLOG,             "syslog"},
#endif
#ifdef CAP_EPOLLWAKEUP
  {CAP_EPOLLWAKEUP,        "epollwakeup"},
#endif
#ifdef CAP_WAKE_ALARM
  {CAP_WAKE_ALARM,         "wake_alarm"},
#endif
#ifdef CAP_BLOCK_SUSPEND
  {CAP_BLOCK_SUSPEND,      "block_suspend"},
#endif
#ifdef CAP_AUDIT_READ
  {CAP_AUDIT_READ,         "audit_read"},
#endif
#ifdef CAP_PERFMON
  {CAP_PERFMON,            "perfmon"},
#endif
#ifdef CAP_BPF
  {CAP_BPF,                "bpf"},
#endif
#ifdef CAP_CHECKPOINT_RESTORE
  {CAP_CHECKPOINT_RESTORE, "checkpoint_restore"},
#endif
  {-1,                     NULL},
};
