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

/* clang-format off */
/*
 * Document-class: CapNG::Capability
 *
 * Check Linux capabilities and define its constants.
 *
 * @example
 *  require 'capng'
 *
 *  @cap = CapNG::Capability.new
 *
 *  @cap.from_name(:dac_read_search) #=> 2
 *  @cap.to_name(CapNG::Capability::DAC_READ_SEARCH) #=> "dac_read_search"
 */
/* clang-format on */

struct CapNGCapability
{};

static void
capng_capability_free(void* capng);

static const rb_data_type_t rb_capng_capability_type = { "capng_capability/c_runtime",
                                                         {
                                                           0,
                                                           capng_capability_free,
                                                           0,
                                                         },
                                                         NULL,
                                                         NULL,
                                                         RUBY_TYPED_FREE_IMMEDIATELY };

static void
capng_capability_free(void* ptr)
{
  xfree(ptr);
}

static VALUE
rb_capng_capability_alloc(VALUE klass)
{
  VALUE obj;
  struct CapNGCapability* capng_capability;
  obj = TypedData_Make_Struct(
    klass, struct CapNGCapability, &rb_capng_capability_type, capng_capability);
  return obj;
}

/*
 * Initalize Capability class.
 *
 * @return [nil]
 *
 */
static VALUE
rb_capng_capability_initialize(VALUE self)
{
  return Qnil;
}

/*
 * Obtain capability name from capability value.
 *
 * @param rb_capability [Integer] Capability constant value.
 * @return [String]
 *
 */
static VALUE
rb_capng_capability_to_name(VALUE self, VALUE rb_capability)
{
  const char* name = capng_capability_to_name(NUM2UINT(rb_capability));

  if (name)
    return rb_str_new2(name);
  else
    return rb_str_new2("unknown");
}

/*
 * Obtain capability value from capability name.
 *
 * @param rb_capability_name_or_symbol [String or Symbol] Capability constant value.
 * @return [Integer]
 *
 */
static VALUE
rb_capng_capability_from_name(VALUE self, VALUE rb_capability_name_or_symbol)
{
  unsigned int capability;

  switch (TYPE(rb_capability_name_or_symbol)) {
    case T_SYMBOL:
      capability =
        capng_name_to_capability(RSTRING_PTR(rb_sym2str(rb_capability_name_or_symbol)));
      break;
    case T_STRING:
      capability = capng_name_to_capability(StringValuePtr(rb_capability_name_or_symbol));
      break;
    default:
      rb_raise(rb_eArgError, "Expected a String or a Symbol instance");
  }
  return INT2NUM(capability);
}

/*
 * Obtain capability code and name pairs with enumerable.
 *
 * @yield [Integer, String]
 * @return [nil]
 *
 */
static VALUE
rb_capng_capability_each(VALUE self)
{
  RETURN_ENUMERATOR(self, 0, 0);

  for (int i = 0; capabilityInfoTable[i].name != NULL; i++) {
    rb_yield_values(2,
                    INT2NUM(capabilityInfoTable[i].code),
                    rb_str_new2(capabilityInfoTable[i].name));
  }

  return Qnil;
}

void
Init_capng_capability(VALUE rb_cCapNG)
{
  VALUE rb_cCapability = rb_define_class_under(rb_cCapNG, "Capability", rb_cObject);

  rb_define_alloc_func(rb_cCapability, rb_capng_capability_alloc);

  rb_define_method(rb_cCapability, "initialize", rb_capng_capability_initialize, 0);
  rb_define_method(rb_cCapability, "to_name", rb_capng_capability_to_name, 1);
  rb_define_method(rb_cCapability, "from_name", rb_capng_capability_from_name, 1);
  rb_define_method(rb_cCapability, "each", rb_capng_capability_each, 0);

  // Capability constants.

  /* Make arbitrary changes to file UIDs and GIDs (see chown(2)). */
  rb_define_const(rb_cCapability, "CHOWN", INT2NUM(CAP_CHOWN));
  /*
   * Bypass file read, write, and execute permission checks.  (DAC
   * is an abbreviation of "discretionary access control".) */
  rb_define_const(rb_cCapability, "DAC_OVERRIDE", INT2NUM(CAP_DAC_OVERRIDE));
  /*
   * * Bypass file read permission checks and directory read and execute permission
   *   checks;
   * * invoke open_by_handle_at(2);
   * * use the linkat(2) AT_EMPTY_PATH flag to create a link to a file referred to by a
   *   file descriptor.
   */
  rb_define_const(rb_cCapability, "DAC_READ_SEARCH", INT2NUM(CAP_DAC_READ_SEARCH));
  /*
   * * Bypass permission checks on operations that normally require
   *   the filesystem UID of the process to match the UID of the
   *   file (e.g., chmod(2), utime(2)), excluding those operations
   *   covered by CAP_DAC_OVERRIDE and CAP_DAC_READ_SEARCH;
   * * set inode flags (see ioctl_iflags(2)) on arbitrary files;
   * * set Access Control Lists (ACLs) on arbitrary files;
   * * ignore directory sticky bit on file deletion;
   * * modify user extended attributes on sticky directory owned by
   *   any user;
   * * specify O_NOATIME for arbitrary files in open(2) and
   *   fcntl(2).
   */
  rb_define_const(rb_cCapability, "FOWNER", INT2NUM(CAP_FOWNER));
  /*
   * * Don't clear set-user-ID and set-group-ID mode bits when a
   *   file is modified;
   * * set the set-group-ID bit for a file whose GID does not match
   *   the filesystem or any of the supplementary GIDs of the
   *   calling process.
   */
  rb_define_const(rb_cCapability, "FSETID", INT2NUM(CAP_FSETID));
  /* Bypass permission checks for sending signals (see kill(2)).
   * This includes use of the ioctl(2) KDSIGACCEPT operation. */
  rb_define_const(rb_cCapability, "KILL", INT2NUM(CAP_KILL));
  /*
   * * Make arbitrary manipulations of process GIDs and
   *   supplementary GID list;
   * * forge GID when passing socket credentials via UNIX domain
   *   sockets;
   * * write a group ID mapping in a user namespace (see
   *   user_namespaces(7)).
   */
  rb_define_const(rb_cCapability, "SETGID", INT2NUM(CAP_SETGID));
  /*
   * * Make arbitrary manipulations of process UIDs (setuid(2),
   *   setreuid(2), setresuid(2), setfsuid(2));
   * * forge UID when passing socket credentials via UNIX domain
   *   sockets;
   * * write a user ID mapping in a user namespace (see
   *   user_namespaces(7)).
   */
  rb_define_const(rb_cCapability, "SETUID", INT2NUM(CAP_SETUID));
  /*
   * If file capabilities are supported (i.e., since Linux 2.6.24):
   * add any capability from the calling thread's bounding set to
   * its inheritable set; drop capabilities from the bounding set
   * (via prctl(2) PR_CAPBSET_DROP); make changes to the securebits
   * flags.
   *
   * If file capabilities are not supported (i.e., kernels before
   * Linux 2.6.24): grant or remove any capability in the caller's
   * permitted capability set to or from any other process.  (This
   * property of CAP_SETPCAP is not available when the kernel is
   * configured to support file capabilities, since CAP_SETPCAP has
   * entirely different semantics for such kernels.)
   */
  rb_define_const(rb_cCapability, "SETPCAP", INT2NUM(CAP_SETPCAP));
  /* Set the FS_APPEND_FL and FS_IMMUTABLE_FL inode flags (see ioctl_iflags(2)). */
  rb_define_const(rb_cCapability, "LINUX_IMMUTABLE", INT2NUM(CAP_LINUX_IMMUTABLE));
  /* Bind a socket to Internet domain privileged ports (port numbers less than 1024).*/
  rb_define_const(rb_cCapability, "NET_BIND_SERIVCE", INT2NUM(CAP_NET_BIND_SERVICE));
  /* (Unused)  Make socket broadcasts, and listen to multicasts. */
  rb_define_const(rb_cCapability, "NET_BROATCAST", INT2NUM(CAP_NET_BROADCAST));
  /* Perform various network-related operations:
   *
   * * interface configuration;
   * * administration of IP firewall, masquerading, and accounting;
   * * modify routing tables;
   * * bind to any address for transparent proxying;
   * * set type-of-service (TOS);
   * * clear driver statistics;
   * * set promiscuous mode;
   * * enabling multicasting;
   * * use setsockopt(2) to set the following socket options:
   *   * SO_DEBUG, SO_MARK, SO_PRIORITY (for a priority outside the
   *   * range 0 to 6), SO_RCVBUFFORCE, and SO_SNDBUFFORCE.
   */
  rb_define_const(rb_cCapability, "NET_ADMIN", INT2NUM(CAP_NET_ADMIN));
  /*
   * * Use RAW and PACKET sockets;
   * * bind to any address for transparent proxying.
   */
  rb_define_const(rb_cCapability, "NET_RAW", INT2NUM(CAP_NET_RAW));
  /* Lock memory (mlock(2), mlockall(2), mmap(2), shmctl(2)). */
  rb_define_const(rb_cCapability, "IPC_LOCK", INT2NUM(CAP_IPC_LOCK));
  /* Bypass permission checks for operations on System V IPC
   * objects.
   */
  rb_define_const(rb_cCapability, "IPC_OWNER", INT2NUM(CAP_IPC_OWNER));
  /*
   * * Load and unload kernel modules (see init_module(2) and
   *   delete_module(2)) in kernels before 2.6.25
   * * drop capabilities from the system-wide capability bounding set.
   */
  rb_define_const(rb_cCapability, "SYS_MODULE", INT2NUM(CAP_SYS_MODULE));
  /*
   * * Perform I/O port operations (iopl(2) and ioperm(2));
   * * access /proc/kcore;
   * * employ the FIBMAP ioctl(2) operation;
   * * open devices for accessing x86 model-specific registers
   *   (MSRs, see msr(4));
   * * update /proc/sys/vm/mmap_min_addr;
   * * create memory mappings at addresses below the value
   *   specified by /proc/sys/vm/mmap_min_addr;
   * * map files in /proc/bus/pci;
   * * open /dev/mem and /dev/kmem;
   * * perform various SCSI device commands;
   * * perform certain operations on hpsa(4) and cciss(4) devices;
   * * perform a range of device-specific operations on other
   *   devices.
   */
  rb_define_const(rb_cCapability, "SYS_RAWIO", INT2NUM(CAP_SYS_RAWIO));
  /*
   * * Use chroot(2);
   * * change mount namespaces using setns(2).
   */
  rb_define_const(rb_cCapability, "SYS_CHROOT", INT2NUM(CAP_SYS_CHROOT));
  /*
   * * Trace arbitrary processes using ptrace(2);
   * * apply get_robust_list(2) to arbitrary processes;
   * * transfer data to or from the memory of arbitrary processes
   *   using process_vm_readv(2) and process_vm_writev(2);
   * * inspect processes using kcmp(2).
   */
  rb_define_const(rb_cCapability, "SYS_PTRACE", INT2NUM(CAP_SYS_PTRACE));
  /* Use acct(2). */
  rb_define_const(rb_cCapability, "SYS_PACCT", INT2NUM(CAP_SYS_PACCT));
  /*
   * Note:
   *  this capability is overloaded; see Notes to kernel developers, below.
   *
   * * Perform a range of system administration operations
   *   including: quotactl(2), mount(2), umount(2), pivot_root(2),
   *   swapon(2), swapoff(2), sethostname(2), and setdomainname(2);
   * * perform privileged syslog(2) operations (since Linux 2.6.37,
   *   CAP_SYSLOG should be used to permit such operations);
   * * perform VM86_REQUEST_IRQ vm86(2) command;
   * * access the same checkpoint/restore functionality that is
   *   governed by CAP_CHECKPOINT_RESTORE (but the latter, weaker
   *   capability is preferred for accessing that functionality).
   * * perform the same BPF operations as are governed by CAP_BPF
   *   (but the latter, weaker capability is preferred for
   *   accessing that functionality).
   * * employ the same performance monitoring mechanisms as are
   *   governed by CAP_PERFMON (but the latter, weaker capability
   *   is preferred for accessing that functionality).
   * * perform IPC_SET and IPC_RMID operations on arbitrary System
   *   V IPC objects;
   * * override RLIMIT_NPROC resource limit;
   * * perform operations on trusted and security extended
   *   attributes (see xattr(7));
   * * use lookup_dcookie(2);
   * * use ioprio_set(2) to assign IOPRIO_CLASS_RT and (before
   *   Linux 2.6.25) IOPRIO_CLASS_IDLE I/O scheduling classes;
   * * forge PID when passing socket credentials via UNIX domain
   *   sockets;
   * * exceed /proc/sys/fs/file-max, the system-wide limit on the
   *   number of open files, in system calls that open files (e.g.,
   *   accept(2), execve(2), open(2), pipe(2));
   * * employ CLONE_* flags that create new namespaces with
   *   clone(2) and unshare(2) (but, since Linux 3.8, creating user
   *   namespaces does not require any capability);
   * * access privileged perf event information;
   * * call setns(2) (requires CAP_SYS_ADMIN in the target
   *   namespace);
   * * call fanotify_init(2);
   * * perform privileged KEYCTL_CHOWN and KEYCTL_SETPERM keyctl(2)
   *   operations;
   * * perform madvise(2) MADV_HWPOISON operation;
   * * employ the TIOCSTI ioctl(2) to insert characters into the
   *   input queue of a terminal other than the caller's
   *   controlling terminal;
   * * employ the obsolete nfsservctl(2) system call;
   * * employ the obsolete bdflush(2) system call;
   * * perform various privileged block-device ioctl(2) operations;
   * * perform various privileged filesystem ioctl(2) operations;
   * * perform privileged ioctl(2) operations on the /dev/random
   *   device (see random(4));
   * * install a seccomp(2) filter without first having to set the
   *   no_new_privs thread attribute;
   * * modify allow/deny rules for device control groups;
   * * employ the ptrace(2) PTRACE_SECCOMP_GET_FILTER operation to
   *   dump tracee's seccomp filters;
   * * employ the ptrace(2) PTRACE_SETOPTIONS operation to suspend
   *   the tracee's seccomp protections (i.e., the
   *   PTRACE_O_SUSPEND_SECCOMP flag);
   * * perform administrative operations on many device drivers;
   * * modify autogroup nice values by writing to
   *   /proc/[pid]/autogroup (see sched(7)).
   */
  rb_define_const(rb_cCapability, "SYS_ADMIN", INT2NUM(CAP_SYS_ADMIN));
  /* Use reboot(2) and kexec_load(2). */
  rb_define_const(rb_cCapability, "SYS_BOOT", INT2NUM(CAP_SYS_BOOT));
  /*
   * * Lower the process nice value (nice(2), setpriority(2)) and
   *   change the nice value for arbitrary processes;
   * * set real-time scheduling policies for calling process, and
   *   set scheduling policies and priorities for arbitrary
   *   processes (sched_setscheduler(2), sched_setparam(2),
   *   sched_setattr(2));
   * * set CPU affinity for arbitrary processes
   *   (sched_setaffinity(2));
   * * set I/O scheduling class and priority for arbitrary
   *   processes (ioprio_set(2));
   * * apply migrate_pages(2) to arbitrary processes and allow
   *   processes to be migrated to arbitrary nodes;
   * * apply move_pages(2) to arbitrary processes;
   * * use the MPOL_MF_MOVE_ALL flag with mbind(2) and
   *   move_pages(2).
   */
  rb_define_const(rb_cCapability, "SYS_NICE", INT2NUM(CAP_SYS_NICE));
  /*
   * * Use reserved space on ext2 filesystems;
   * * make ioctl(2) calls controlling ext3 journaling;
   * * override disk quota limits;
   * * increase resource limits (see setrlimit(2));
   * * override RLIMIT_NPROC resource limit;
   * * override maximum number of consoles on console allocation;
   * * override maximum number of keymaps;
   * * allow more than 64hz interrupts from the real-time clock;
   * * raise msg_qbytes limit for a System V message queue above
   *   the limit in /proc/sys/kernel/msgmnb (see msgop(2) and
   *   msgctl(2));
   * * allow the RLIMIT_NOFILE resource limit on the number of "in-
   *   flight" file descriptors to be bypassed when passing file
   *   descriptors to another process via a UNIX domain socket (see
   *   unix(7));
   * * override the /proc/sys/fs/pipe-size-max limit when setting
   *   the capacity of a pipe using the F_SETPIPE_SZ fcntl(2)
   *   command;
   * * use F_SETPIPE_SZ to increase the capacity of a pipe above
   *   the limit specified by /proc/sys/fs/pipe-max-size;
   * * override /proc/sys/fs/mqueue/queues_max,
   *   /proc/sys/fs/mqueue/msg_max, and
   *   /proc/sys/fs/mqueue/msgsize_max limits when creating POSIX
   *   message queues (see mq_overview(7));
   * * employ the prctl(2) PR_SET_MM operation;
   * * set /proc/[pid]/oom_score_adj to a value lower than the
   *   value last set by a process with CAP_SYS_RESOURCE.
   */
  rb_define_const(rb_cCapability, "SYS_RESOURCE", INT2NUM(CAP_SYS_RESOURCE));
  /* Set system clock (settimeofday(2), stime(2), adjtimex(2)); set
   * real-time (hardware) clock.*/
  rb_define_const(rb_cCapability, "SYS_TIME", INT2NUM(CAP_SYS_TIME));
  /* Use vhangup(2); employ various privileged ioctl(2) operations
   * on virtual terminals.
   */
  rb_define_const(rb_cCapability, "TTY_CONFIG", INT2NUM(CAP_SYS_TTY_CONFIG));
  /* Create special files using mknod(2). (since Linux 2.4) */
  rb_define_const(rb_cCapability, "MKNOD", INT2NUM(CAP_MKNOD));
  /* Establish leases on arbitrary files (see fcntl(2)). (since Linux 2.4) */
  rb_define_const(rb_cCapability, "LEASE", INT2NUM(CAP_LEASE));
  /* Write records to kernel auditing log.  (since Linux 2.6.11) */
  rb_define_const(rb_cCapability, "AUDIT_WRITE", INT2NUM(CAP_AUDIT_WRITE));
  /* Enable and disable kernel auditing; change auditing filter
   * rules; retrieve auditing status and filtering rules.  (since Linux 2.6.11)*/
  rb_define_const(rb_cCapability, "AUDIT_CONTROL", INT2NUM(CAP_AUDIT_CONTROL));
#ifdef CAP_SETFCAP
  /* Set arbitrary capabilities on a file. since Linux 2.6.24) */
  rb_define_const(rb_cCapability, "SETFCAP", INT2NUM(CAP_SETFCAP));
#endif
#ifdef CAP_MAC_OVERRIDE
  rb_define_const(rb_cCapability, "MAC_OVERRIDE", INT2NUM(CAP_MAC_OVERRIDE));
#endif
#ifdef CAP_MAC_ADMIN
  /* Allow MAC configuration or state changes.  Implemented for the
   * Smack Linux Security Module (LSM). (since Linux 2.6.25)
   */
  rb_define_const(rb_cCapability, "MAC_ADMIN", INT2NUM(CAP_MAC_ADMIN));
#endif
#ifdef CAP_SYSLOG
  /*
   * * Perform privileged syslog(2) operations.  See syslog(2) for
   *   information on which operations require privilege.
   * * View kernel addresses exposed via /proc and other interfaces
   *   when /proc/sys/kernel/kptr_restrict has the value 1.  (See
   *   the discussion of the kptr_restrict in proc(5).)
   */
  rb_define_const(rb_cCapability, "SYSLOG", INT2NUM(CAP_SYSLOG));
#endif
#if defined(CAP_EPOLLWAKEUP) && defined(CAP_BLOCK_SUSPEND)
#error "Both CAP_EPOLLWAKEUP and CAP_BLOCK_SUSPEND are defined"
#endif
#ifdef CAP_EPOLLWAKEUP
  rb_define_const(rb_cCapability, "EPOLLWAKEUP", INT2NUM(CAP_EPOLLWAKEUP));
#endif
#ifdef CAP_WAKE_ALARM
  /* Trigger something that will wake up the system (set
   * CLOCK_REALTIME_ALARM and CLOCK_BOOTTIME_ALARM timers).
   */
  rb_define_const(rb_cCapability, "WAKE_ALARM", INT2NUM(CAP_WAKE_ALARM));
#endif
#ifdef CAP_BLOCK_SUSPEND
  /*
    Employ features that can block system suspend (epoll(7)
    EPOLLWAKEUP, /proc/sys/wake_lock). (since Linux 3.5)
  */
  rb_define_const(rb_cCapability, "BLOCK_SUSPEND", INT2NUM(CAP_BLOCK_SUSPEND));
#endif
#ifdef CAP_AUDIT_READ
  /* Allow reading the audit log via a multicast netlink socket. (since Linux 3.16) */
  rb_define_const(rb_cCapability, "AUDIT_READ", INT2NUM(CAP_AUDIT_READ));
#endif
#ifdef CAP_PERFMON
  /*
   * Employ various performance-monitoring mechanisms, including:
   *
   * * call perf_event_open(2)
   * * employ various BPF operations that have performance
   *   implications.
   *
   * This capability was added in Linux 5.8 to separate out
   * performance monitoring functionality from the overloaded
   * CAP_SYS_ADMIN capability.  See also the kernel source file
   * Documentation/admin-guide/perf-security.rst.
   */
  rb_define_const(rb_cCapability, "PERFMON", INT2NUM(CAP_PERFMON));
#endif
#ifdef CAP_BPF
  /*
   * Employ privileged BPF operations; see bpf(2) and
   * bpf-helpers(7).
   *
   * This capability was added in Linux 5.8 to separate out BPF
   * functionality from the overloaded CAP_SYS_ADMIN capability.
   * (since Linux 5.8)
   */
  rb_define_const(rb_cCapability, "BPF", INT2NUM(CAP_BPF));
#endif
#ifdef CAP_CHECKPOINT_RESTORE
  /*
   * * employ the set_tid feature of clone3(2);
   * * read the contents of the symbolic links in
   *   /proc/[pid]/map_files for other processes.
   *
   * This capability was added in Linux 5.9 to separate out
   * checkpoint/restore functionality from the overloaded
   * CAP_SYS_ADMIN capability.
   * (since Linux 5.9)
   */
  rb_define_const(rb_cCapability, "CHECKPOINT_RESTORE", INT2NUM(CAP_CHECKPOINT_RESTORE));
#endif
}
