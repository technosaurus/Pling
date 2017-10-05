//TODO add includes and make this actually work, I just wrote it in 1 pass

/*** Config Options ***/
#define SINGLEUSER _Y
#define DEBUG      _N
#define MOUNTALL   _Y


#if 1 //Macros - ifdef'ed for folding
#define LEN(x) (sizeof(x)/sizeof(x[0]))
#define PASTE_(x,y) x##y
#define PASTE(x,y) PASTE_(x,y)
#ifndef _Y
#define _Y(...) __VA_ARGS__
#define _N(...)
#define NOT_N Y
#define NOT_Y N
#define IF(x) x //alternate method
#define IF_NOT(x) PASTE(NOT_,x)
#define NOT(x) PASTE(NOT_,x)
#endif
IF(SINGLEUSER) (
#define USER "root"
#define HOME "/root"
)
IF_NOT(SINGLEUSER) (
#ifndef USER
#define USER "puppy"
#endif
#define HOME "/home/" USER
)
#endif //Macros

struct mount {
  const char *source;
  const char *target;
  const char *fstype,
  unsigned long flags,
  const void *data
};

struct qmount {
  const char *source;
  const char *target;
  const char *fstype,
};
  
static const struct qmount default_mounts[] = {
  { "/proc",    "/proc",    "proc" },
  { "/sys",     "/sys",     "sys"  },
  { "/dev",     "/dev",     "devtmpfs" },
  { "/dev/pts", "/dev/pts", "devpts" },
  { HOME,       HOME,       "tmpfs" },
}

char *environ[] = {
  "PATH=/bin",
  "HOME=/",
  "TERM=rxvt",
  "PS1=# ",
  "SHELL=/bin/sh",
  "DISPLAY=:0"
};

static char **kernel_args;
static char **kernel_env;


void do_default_mounts(void);
void do_extra_mounts(void);
void handle_kernel_args(void);
void quick_mount(struct mount m);

#define XMACRO_THREADS(OP) \
  /* Function name,      Prerequisites... */ \
  OP(do_default_mounts) \
  OP(handle_kernel_args) \
  OP(do_extra_mounts, do_default_mounts_FINISHED|handle_kernel_args_FINISHED)

//mount the default mounts ... 
static void do_default_mounts(void){
  for ( size_t i = 0; i < LEN(default_mounts); ++i ){
    quick_mount(default_mounts[i]);
  }
}

//mount stuff in /proc/partitions + user files within
static void do_extra_mounts(void){
  MOUNTALL(
  /* see my blkid implementation at:
   * https://github.com/landley/toybox/blob/master/toys/other/blkid.c
   */
  )
  //mount user specified mounts
    //TODO
}

void quick_mount(struct mount *m){
  long ret = syscall(__NR_mount, m->source, m->target, m->fstype, MS_MGC_VAL, NULL);
  DEBUG(if (ret<0) printf("failed to mount %s\n",m.target);)
}

/* The linux kernel puts boot parameters containing an '=' in envp,
 * and the remaining parameters go in argv ... in no particular order
 */
int main(int argc, char **argv, char **envp){
  //make argv and envp globally accessible
  kernel_args=argv;
  kernel_env=envp;

  //TODO -> do these in thread with clone syscall. see:
  //https://github.com/technosaurus/BQC/blob/master/wait4-clone-nanosleep-example.c
  do_default_mounts();
  handle_kernel_args();
  do_extra_mounts();
  
  while(1) sleep(1);
}

static inline void *getsp(){
  //TODO: I forgot exactly what I did here before
  register void *sp asm("sp") = sp;
  //I think clang may need some kind of inline asm statement here
  return sp;
}

/* We use our own _start to avoid clobbering environ and to minimize overhead
 * Linux passes parameters on the stack regardless of the platform (binfmt_elf.c)
 * Unfortunately we can't just use a cdecl function attribute,
 * so we need to do some platform specific garbage here: thus `getsp()`
 * Most c libraries use assembly for this part, we may have to also.
 */
void _start(){
  //This is probably off, added here just for the basic idea
  long *argcp = (long *) getsp(), argc = *argcp;
  char **argv = (const char**) argcp;
  char **envp = argv + argc;
  exit(main(argc,argv,envp));
}
