Linux Cross Reference
Free Electrons
Embedded Linux Experts

 • source navigation  • diff markup  • identifier search  • freetext search  • 

Version:  2.0.40 2.2.26 2.4.37 3.2 3.3 3.4 3.5 3.6 3.7 3.8 3.9 3.10 3.11 3.12 3.13 3.14 3.15 3.16 3.17 3.18
Linux/include/linux/kernel.h

  1 #ifndef _LINUX_KERNEL_H
  2 #define _LINUX_KERNEL_H
  3 
  4 
  5 #include <stdarg.h>
  6 #include <linux/linkage.h>
  7 #include <linux/stddef.h>
  8 #include <linux/types.h>
  9 #include <linux/compiler.h>
 10 #include <linux/bitops.h>
 11 #include <linux/log2.h>
 12 #include <linux/typecheck.h>
 13 #include <linux/printk.h>
 14 #include <linux/dynamic_debug.h>
 15 #include <asm/byteorder.h>
 16 #include <uapi/linux/kernel.h>
 17 
 18 #define USHRT_MAX       ((u16)(~0U))
 19 #define SHRT_MAX        ((s16)(USHRT_MAX>>1))
 20 #define SHRT_MIN        ((s16)(-SHRT_MAX - 1))
 21 #define INT_MAX         ((int)(~0U>>1))
 22 #define INT_MIN         (-INT_MAX - 1)
 23 #define UINT_MAX        (~0U)
 24 #define LONG_MAX        ((long)(~0UL>>1))
 25 #define LONG_MIN        (-LONG_MAX - 1)
 26 #define ULONG_MAX       (~0UL)
 27 #define LLONG_MAX       ((long long)(~0ULL>>1))
 28 #define LLONG_MIN       (-LLONG_MAX - 1)
 29 #define ULLONG_MAX      (~0ULL)
 30 #define SIZE_MAX        (~(size_t)0)
 31 
 32 #define U8_MAX          ((u8)~0U)
 33 #define S8_MAX          ((s8)(U8_MAX>>1))
 34 #define S8_MIN          ((s8)(-S8_MAX - 1))
 35 #define U16_MAX         ((u16)~0U)
 36 #define S16_MAX         ((s16)(U16_MAX>>1))
 37 #define S16_MIN         ((s16)(-S16_MAX - 1))
 38 #define U32_MAX         ((u32)~0U)
 39 #define S32_MAX         ((s32)(U32_MAX>>1))
 40 #define S32_MIN         ((s32)(-S32_MAX - 1))
 41 #define U64_MAX         ((u64)~0ULL)
 42 #define S64_MAX         ((s64)(U64_MAX>>1))
 43 #define S64_MIN         ((s64)(-S64_MAX - 1))
 44 
 45 #define STACK_MAGIC     0xdeadbeef
 46 
 47 #define REPEAT_BYTE(x)  ((~0ul / 0xff) * (x))
 48 
 49 #define ALIGN(x, a)             __ALIGN_KERNEL((x), (a))
 50 #define __ALIGN_MASK(x, mask)   __ALIGN_KERNEL_MASK((x), (mask))
 51 #define PTR_ALIGN(p, a)         ((typeof(p))ALIGN((unsigned long)(p), (a)))
 52 #define IS_ALIGNED(x, a)                (((x) & ((typeof(x))(a) - 1)) == 0)
 53 
 54 #define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))
 55 
 56 /*
 57  * This looks more complex than it should be. But we need to
 58  * get the type for the ~ right in round_down (it needs to be
 59  * as wide as the result!), and we want to evaluate the macro
 60  * arguments just once each.
 61  */
 62 #define __round_mask(x, y) ((__typeof__(x))((y)-1))
 63 #define round_up(x, y) ((((x)-1) | __round_mask(x, y))+1)
 64 #define round_down(x, y) ((x) & ~__round_mask(x, y))
 65 
 66 #define FIELD_SIZEOF(t, f) (sizeof(((t*)0)->f))
 67 #define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))
 68 #define DIV_ROUND_UP_ULL(ll,d) \
 69         ({ unsigned long long _tmp = (ll)+(d)-1; do_div(_tmp, d); _tmp; })
 70 
 71 #if BITS_PER_LONG == 32
 72 # define DIV_ROUND_UP_SECTOR_T(ll,d) DIV_ROUND_UP_ULL(ll, d)
 73 #else
 74 # define DIV_ROUND_UP_SECTOR_T(ll,d) DIV_ROUND_UP(ll,d)
 75 #endif
 76 
 77 /* The `const' in roundup() prevents gcc-3.3 from calling __divdi3 */
 78 #define roundup(x, y) (                                 \
 79 {                                                       \
 80         const typeof(y) __y = y;                        \
 81         (((x) + (__y - 1)) / __y) * __y;                \
 82 }                                                       \
 83 )
 84 #define rounddown(x, y) (                               \
 85 {                                                       \
 86         typeof(x) __x = (x);                            \
 87         __x - (__x % (y));                              \
 88 }                                                       \
 89 )
 90 
 91 /*
 92  * Divide positive or negative dividend by positive divisor and round
 93  * to closest integer. Result is undefined for negative divisors and
 94  * for negative dividends if the divisor variable type is unsigned.
 95  */
 96 #define DIV_ROUND_CLOSEST(x, divisor)(                  \
 97 {                                                       \
 98         typeof(x) __x = x;                              \
 99         typeof(divisor) __d = divisor;                  \
100         (((typeof(x))-1) > 0 ||                         \
101          ((typeof(divisor))-1) > 0 || (__x) > 0) ?      \
102                 (((__x) + ((__d) / 2)) / (__d)) :       \
103                 (((__x) - ((__d) / 2)) / (__d));        \
104 }                                                       \
105 )
106 
107 /*
108  * Multiplies an integer by a fraction, while avoiding unnecessary
109  * overflow or loss of precision.
110  */
111 #define mult_frac(x, numer, denom)(                     \
112 {                                                       \
113         typeof(x) quot = (x) / (denom);                 \
114         typeof(x) rem  = (x) % (denom);                 \
115         (quot * (numer)) + ((rem * (numer)) / (denom)); \
116 }                                                       \
117 )
118 
119 
120 #define _RET_IP_                (unsigned long)__builtin_return_address(0)
121 #define _THIS_IP_  ({ __label__ __here; __here: (unsigned long)&&__here; })
122 
123 #ifdef CONFIG_LBDAF
124 # include <asm/div64.h>
125 # define sector_div(a, b) do_div(a, b)
126 #else
127 # define sector_div(n, b)( \
128 { \
129         int _res; \
130         _res = (n) % (b); \
131         (n) /= (b); \
132         _res; \
133 } \
134 )
135 #endif
136 
137 /**
138  * upper_32_bits - return bits 32-63 of a number
139  * @n: the number we're accessing
140  *
141  * A basic shift-right of a 64- or 32-bit quantity.  Use this to suppress
142  * the "right shift count >= width of type" warning when that quantity is
143  * 32-bits.
144  */
145 #define upper_32_bits(n) ((u32)(((n) >> 16) >> 16))
146 
147 /**
148  * lower_32_bits - return bits 0-31 of a number
149  * @n: the number we're accessing
150  */
151 #define lower_32_bits(n) ((u32)(n))
152 
153 struct completion;
154 struct pt_regs;
155 struct user;
156 
157 #ifdef CONFIG_PREEMPT_VOLUNTARY
158 extern int _cond_resched(void);
159 # define might_resched() _cond_resched()
160 #else
161 # define might_resched() do { } while (0)
162 #endif
163 
164 #ifdef CONFIG_DEBUG_ATOMIC_SLEEP
165   void __might_sleep(const char *file, int line, int preempt_offset);
166 /**
167  * might_sleep - annotation for functions that can sleep
168  *
169  * this macro will print a stack trace if it is executed in an atomic
170  * context (spinlock, irq-handler, ...).
171  *
172  * This is a useful debugging help to be able to catch problems early and not
173  * be bitten later when the calling function happens to sleep when it is not
174  * supposed to.
175  */
176 # define might_sleep() \
177         do { __might_sleep(__FILE__, __LINE__, 0); might_resched(); } while (0)
178 #else
179   static inline void __might_sleep(const char *file, int line,
180                                    int preempt_offset) { }
181 # define might_sleep() do { might_resched(); } while (0)
182 #endif
183 
184 #define might_sleep_if(cond) do { if (cond) might_sleep(); } while (0)
185 
186 /*
187  * abs() handles unsigned and signed longs, ints, shorts and chars.  For all
188  * input types abs() returns a signed long.
189  * abs() should not be used for 64-bit types (s64, u64, long long) - use abs64()
190  * for those.
191  */
192 #define abs(x) ({                                               \
193                 long ret;                                       \
194                 if (sizeof(x) == sizeof(long)) {                \
195                         long __x = (x);                         \
196                         ret = (__x < 0) ? -__x : __x;           \
197                 } else {                                        \
198                         int __x = (x);                          \
199                         ret = (__x < 0) ? -__x : __x;           \
200                 }                                               \
201                 ret;                                            \
202         })
203 
204 #define abs64(x) ({                             \
205                 s64 __x = (x);                  \
206                 (__x < 0) ? -__x : __x;         \
207         })
208 
209 /**
210  * reciprocal_scale - "scale" a value into range [0, ep_ro)
211  * @val: value
212  * @ep_ro: right open interval endpoint
213  *
214  * Perform a "reciprocal multiplication" in order to "scale" a value into
215  * range [0, ep_ro), where the upper interval endpoint is right-open.
216  * This is useful, e.g. for accessing a index of an array containing
217  * ep_ro elements, for example. Think of it as sort of modulus, only that
218  * the result isn't that of modulo. ;) Note that if initial input is a
219  * small value, then result will return 0.
220  *
221  * Return: a result based on val in interval [0, ep_ro).
222  */
223 static inline u32 reciprocal_scale(u32 val, u32 ep_ro)
224 {
225         return (u32)(((u64) val * ep_ro) >> 32);
226 }
227 
228 #if defined(CONFIG_MMU) && \
229         (defined(CONFIG_PROVE_LOCKING) || defined(CONFIG_DEBUG_ATOMIC_SLEEP))
230 void might_fault(void);
231 #else
232 static inline void might_fault(void) { }
233 #endif
234 
235 extern struct atomic_notifier_head panic_notifier_list;
236 extern long (*panic_blink)(int state);
237 __printf(1, 2)
238 void panic(const char *fmt, ...)
239         __noreturn __cold;
240 extern void oops_enter(void);
241 extern void oops_exit(void);
242 void print_oops_end_marker(void);
243 extern int oops_may_print(void);
244 void do_exit(long error_code)
245         __noreturn;
246 void complete_and_exit(struct completion *, long)
247         __noreturn;
248 
249 /* Internal, do not use. */
250 int __must_check _kstrtoul(const char *s, unsigned int base, unsigned long *res);
251 int __must_check _kstrtol(const char *s, unsigned int base, long *res);
252 
253 int __must_check kstrtoull(const char *s, unsigned int base, unsigned long long *res);
254 int __must_check kstrtoll(const char *s, unsigned int base, long long *res);
255 
256 /**
257  * kstrtoul - convert a string to an unsigned long
258  * @s: The start of the string. The string must be null-terminated, and may also
259  *  include a single newline before its terminating null. The first character
260  *  may also be a plus sign, but not a minus sign.
261  * @base: The number base to use. The maximum supported base is 16. If base is
262  *  given as 0, then the base of the string is automatically detected with the
263  *  conventional semantics - If it begins with 0x the number will be parsed as a
264  *  hexadecimal (case insensitive), if it otherwise begins with 0, it will be
265  *  parsed as an octal number. Otherwise it will be parsed as a decimal.
266  * @res: Where to write the result of the conversion on success.
267  *
268  * Returns 0 on success, -ERANGE on overflow and -EINVAL on parsing error.
269  * Used as a replacement for the obsolete simple_strtoull. Return code must
270  * be checked.
271 */
272 static inline int __must_check kstrtoul(const char *s, unsigned int base, unsigned long *res)
273 {
274         /*
275          * We want to shortcut function call, but
276          * __builtin_types_compatible_p(unsigned long, unsigned long long) = 0.
277          */
278         if (sizeof(unsigned long) == sizeof(unsigned long long) &&
279             __alignof__(unsigned long) == __alignof__(unsigned long long))
280                 return kstrtoull(s, base, (unsigned long long *)res);
281         else
282                 return _kstrtoul(s, base, res);
283 }
284 
285 /**
286  * kstrtol - convert a string to a long
287  * @s: The start of the string. The string must be null-terminated, and may also
288  *  include a single newline before its terminating null. The first character
289  *  may also be a plus sign or a minus sign.
290  * @base: The number base to use. The maximum supported base is 16. If base is
291  *  given as 0, then the base of the string is automatically detected with the
292  *  conventional semantics - If it begins with 0x the number will be parsed as a
293  *  hexadecimal (case insensitive), if it otherwise begins with 0, it will be
294  *  parsed as an octal number. Otherwise it will be parsed as a decimal.
295  * @res: Where to write the result of the conversion on success.
296  *
297  * Returns 0 on success, -ERANGE on overflow and -EINVAL on parsing error.
298  * Used as a replacement for the obsolete simple_strtoull. Return code must
299  * be checked.
300  */
301 static inline int __must_check kstrtol(const char *s, unsigned int base, long *res)
302 {
303         /*
304          * We want to shortcut function call, but
305          * __builtin_types_compatible_p(long, long long) = 0.
306          */
307         if (sizeof(long) == sizeof(long long) &&
308             __alignof__(long) == __alignof__(long long))
309                 return kstrtoll(s, base, (long long *)res);
310         else
311                 return _kstrtol(s, base, res);
312 }
313 
314 int __must_check kstrtouint(const char *s, unsigned int base, unsigned int *res);
315 int __must_check kstrtoint(const char *s, unsigned int base, int *res);
316 
317 static inline int __must_check kstrtou64(const char *s, unsigned int base, u64 *res)
318 {
319         return kstrtoull(s, base, res);
320 }
321 
322 static inline int __must_check kstrtos64(const char *s, unsigned int base, s64 *res)
323 {
324         return kstrtoll(s, base, res);
325 }
326 
327 static inline int __must_check kstrtou32(const char *s, unsigned int base, u32 *res)
328 {
329         return kstrtouint(s, base, res);
330 }
331 
332 static inline int __must_check kstrtos32(const char *s, unsigned int base, s32 *res)
333 {
334         return kstrtoint(s, base, res);
335 }
336 
337 int __must_check kstrtou16(const char *s, unsigned int base, u16 *res);
338 int __must_check kstrtos16(const char *s, unsigned int base, s16 *res);
339 int __must_check kstrtou8(const char *s, unsigned int base, u8 *res);
340 int __must_check kstrtos8(const char *s, unsigned int base, s8 *res);
341 
342 int __must_check kstrtoull_from_user(const char __user *s, size_t count, unsigned int base, unsigned long long *res);
343 int __must_check kstrtoll_from_user(const char __user *s, size_t count, unsigned int base, long long *res);
344 int __must_check kstrtoul_from_user(const char __user *s, size_t count, unsigned int base, unsigned long *res);
345 int __must_check kstrtol_from_user(const char __user *s, size_t count, unsigned int base, long *res);
346 int __must_check kstrtouint_from_user(const char __user *s, size_t count, unsigned int base, unsigned int *res);
347 int __must_check kstrtoint_from_user(const char __user *s, size_t count, unsigned int base, int *res);
348 int __must_check kstrtou16_from_user(const char __user *s, size_t count, unsigned int base, u16 *res);
349 int __must_check kstrtos16_from_user(const char __user *s, size_t count, unsigned int base, s16 *res);
350 int __must_check kstrtou8_from_user(const char __user *s, size_t count, unsigned int base, u8 *res);
351 int __must_check kstrtos8_from_user(const char __user *s, size_t count, unsigned int base, s8 *res);
352 
353 static inline int __must_check kstrtou64_from_user(const char __user *s, size_t count, unsigned int base, u64 *res)
354 {
355         return kstrtoull_from_user(s, count, base, res);
356 }
357 
358 static inline int __must_check kstrtos64_from_user(const char __user *s, size_t count, unsigned int base, s64 *res)
359 {
360         return kstrtoll_from_user(s, count, base, res);
361 }
362 
363 static inline int __must_check kstrtou32_from_user(const char __user *s, size_t count, unsigned int base, u32 *res)
364 {
365         return kstrtouint_from_user(s, count, base, res);
366 }
367 
368 static inline int __must_check kstrtos32_from_user(const char __user *s, size_t count, unsigned int base, s32 *res)
369 {
370         return kstrtoint_from_user(s, count, base, res);
371 }
372 
373 /* Obsolete, do not use.  Use kstrto<foo> instead */
374 
375 extern unsigned long simple_strtoul(const char *,char **,unsigned int);
376 extern long simple_strtol(const char *,char **,unsigned int);
377 extern unsigned long long simple_strtoull(const char *,char **,unsigned int);
378 extern long long simple_strtoll(const char *,char **,unsigned int);
379 
380 extern int num_to_str(char *buf, int size, unsigned long long num);
381 
382 /* lib/printf utilities */
383 
384 extern __printf(2, 3) int sprintf(char *buf, const char * fmt, ...);
385 extern __printf(2, 0) int vsprintf(char *buf, const char *, va_list);
386 extern __printf(3, 4)
387 int snprintf(char *buf, size_t size, const char *fmt, ...);
388 extern __printf(3, 0)
389 int vsnprintf(char *buf, size_t size, const char *fmt, va_list args);
390 extern __printf(3, 4)
391 int scnprintf(char *buf, size_t size, const char *fmt, ...);
392 extern __printf(3, 0)
393 int vscnprintf(char *buf, size_t size, const char *fmt, va_list args);
394 extern __printf(2, 3)
395 char *kasprintf(gfp_t gfp, const char *fmt, ...);
396 extern char *kvasprintf(gfp_t gfp, const char *fmt, va_list args);
397 
398 extern __scanf(2, 3)
399 int sscanf(const char *, const char *, ...);
400 extern __scanf(2, 0)
401 int vsscanf(const char *, const char *, va_list);
402 
403 extern int get_option(char **str, int *pint);
404 extern char *get_options(const char *str, int nints, int *ints);
405 extern unsigned long long memparse(const char *ptr, char **retptr);
406 extern bool parse_option_str(const char *str, const char *option);
407 
408 extern int core_kernel_text(unsigned long addr);
409 extern int core_kernel_data(unsigned long addr);
410 extern int __kernel_text_address(unsigned long addr);
411 extern int kernel_text_address(unsigned long addr);
412 extern int func_ptr_is_kernel_text(void *ptr);
413 
414 struct pid;
415 extern struct pid *session_of_pgrp(struct pid *pgrp);
416 
417 unsigned long int_sqrt(unsigned long);
418 
419 extern void bust_spinlocks(int yes);
420 extern int oops_in_progress;            /* If set, an oops, panic(), BUG() or die() is in progress */
421 extern int panic_timeout;
422 extern int panic_on_oops;
423 extern int panic_on_unrecovered_nmi;
424 extern int panic_on_io_nmi;
425 extern int sysctl_panic_on_stackoverflow;
426 /*
427  * Only to be used by arch init code. If the user over-wrote the default
428  * CONFIG_PANIC_TIMEOUT, honor it.
429  */
430 static inline void set_arch_panic_timeout(int timeout, int arch_default_timeout)
431 {
432         if (panic_timeout == arch_default_timeout)
433                 panic_timeout = timeout;
434 }
435 extern const char *print_tainted(void);
436 enum lockdep_ok {
437         LOCKDEP_STILL_OK,
438         LOCKDEP_NOW_UNRELIABLE
439 };
440 extern void add_taint(unsigned flag, enum lockdep_ok);
441 extern int test_taint(unsigned flag);
442 extern unsigned long get_taint(void);
443 extern int root_mountflags;
444 
445 extern bool early_boot_irqs_disabled;
446 
447 /* Values used for system_state */
448 extern enum system_states {
449         SYSTEM_BOOTING,
450         SYSTEM_RUNNING,
451         SYSTEM_HALT,
452         SYSTEM_POWER_OFF,
453         SYSTEM_RESTART,
454 } system_state;
455 
456 #define TAINT_PROPRIETARY_MODULE        0
457 #define TAINT_FORCED_MODULE             1
458 #define TAINT_CPU_OUT_OF_SPEC           2
459 #define TAINT_FORCED_RMMOD              3
460 #define TAINT_MACHINE_CHECK             4
461 #define TAINT_BAD_PAGE                  5
462 #define TAINT_USER                      6
463 #define TAINT_DIE                       7
464 #define TAINT_OVERRIDDEN_ACPI_TABLE     8
465 #define TAINT_WARN                      9
466 #define TAINT_CRAP                      10
467 #define TAINT_FIRMWARE_WORKAROUND       11
468 #define TAINT_OOT_MODULE                12
469 #define TAINT_UNSIGNED_MODULE           13
470 #define TAINT_SOFTLOCKUP                14
471 
472 extern const char hex_asc[];
473 #define hex_asc_lo(x)   hex_asc[((x) & 0x0f)]
474 #define hex_asc_hi(x)   hex_asc[((x) & 0xf0) >> 4]
475 
476 static inline char *hex_byte_pack(char *buf, u8 byte)
477 {
478         *buf++ = hex_asc_hi(byte);
479         *buf++ = hex_asc_lo(byte);
480         return buf;
481 }
482 
483 extern const char hex_asc_upper[];
484 #define hex_asc_upper_lo(x)     hex_asc_upper[((x) & 0x0f)]
485 #define hex_asc_upper_hi(x)     hex_asc_upper[((x) & 0xf0) >> 4]
486 
487 static inline char *hex_byte_pack_upper(char *buf, u8 byte)
488 {
489         *buf++ = hex_asc_upper_hi(byte);
490         *buf++ = hex_asc_upper_lo(byte);
491         return buf;
492 }
493 
494 extern int hex_to_bin(char ch);
495 extern int __must_check hex2bin(u8 *dst, const char *src, size_t count);
496 extern char *bin2hex(char *dst, const void *src, size_t count);
497 
498 bool mac_pton(const char *s, u8 *mac);
499 
500 /*
501  * General tracing related utility functions - trace_printk(),
502  * tracing_on/tracing_off and tracing_start()/tracing_stop
503  *
504  * Use tracing_on/tracing_off when you want to quickly turn on or off
505  * tracing. It simply enables or disables the recording of the trace events.
506  * This also corresponds to the user space /sys/kernel/debug/tracing/tracing_on
507  * file, which gives a means for the kernel and userspace to interact.
508  * Place a tracing_off() in the kernel where you want tracing to end.
509  * From user space, examine the trace, and then echo 1 > tracing_on
510  * to continue tracing.
511  *
512  * tracing_stop/tracing_start has slightly more overhead. It is used
513  * by things like suspend to ram where disabling the recording of the
514  * trace is not enough, but tracing must actually stop because things
515  * like calling smp_processor_id() may crash the system.
516  *
517  * Most likely, you want to use tracing_on/tracing_off.
518  */
519 #ifdef CONFIG_RING_BUFFER
520 /* trace_off_permanent stops recording with no way to bring it back */
521 void tracing_off_permanent(void);
522 #else
523 static inline void tracing_off_permanent(void) { }
524 #endif
525 
526 enum ftrace_dump_mode {
527         DUMP_NONE,
528         DUMP_ALL,
529         DUMP_ORIG,
530 };
531 
532 #ifdef CONFIG_TRACING
533 void tracing_on(void);
534 void tracing_off(void);
535 int tracing_is_on(void);
536 void tracing_snapshot(void);
537 void tracing_snapshot_alloc(void);
538 
539 extern void tracing_start(void);
540 extern void tracing_stop(void);
541 
542 static inline __printf(1, 2)
543 void ____trace_printk_check_format(const char *fmt, ...)
544 {
545 }
546 #define __trace_printk_check_format(fmt, args...)                       \
547 do {                                                                    \
548         if (0)                                                          \
549                 ____trace_printk_check_format(fmt, ##args);             \
550 } while (0)
551 
552 /**
553  * trace_printk - printf formatting in the ftrace buffer
554  * @fmt: the printf format for printing
555  *
556  * Note: __trace_printk is an internal function for trace_printk and
557  *       the @ip is passed in via the trace_printk macro.
558  *
559  * This function allows a kernel developer to debug fast path sections
560  * that printk is not appropriate for. By scattering in various
561  * printk like tracing in the code, a developer can quickly see
562  * where problems are occurring.
563  *
564  * This is intended as a debugging tool for the developer only.
565  * Please refrain from leaving trace_printks scattered around in
566  * your code. (Extra memory is used for special buffers that are
567  * allocated when trace_printk() is used)
568  *
569  * A little optization trick is done here. If there's only one
570  * argument, there's no need to scan the string for printf formats.
571  * The trace_puts() will suffice. But how can we take advantage of
572  * using trace_puts() when trace_printk() has only one argument?
573  * By stringifying the args and checking the size we can tell
574  * whether or not there are args. __stringify((__VA_ARGS__)) will
575  * turn into "()\0" with a size of 3 when there are no args, anything
576  * else will be bigger. All we need to do is define a string to this,
577  * and then take its size and compare to 3. If it's bigger, use
578  * do_trace_printk() otherwise, optimize it to trace_puts(). Then just
579  * let gcc optimize the rest.
580  */
581 
582 #define trace_printk(fmt, ...)                          \
583 do {                                                    \
584         char _______STR[] = __stringify((__VA_ARGS__)); \
585         if (sizeof(_______STR) > 3)                     \
586                 do_trace_printk(fmt, ##__VA_ARGS__);    \
587         else                                            \
588                 trace_puts(fmt);                        \
589 } while (0)
590 
591 #define do_trace_printk(fmt, args...)                                   \
592 do {                                                                    \
593         static const char *trace_printk_fmt                             \
594                 __attribute__((section("__trace_printk_fmt"))) =        \
595                 __builtin_constant_p(fmt) ? fmt : NULL;                 \
596                                                                         \
597         __trace_printk_check_format(fmt, ##args);                       \
598                                                                         \
599         if (__builtin_constant_p(fmt))                                  \
600                 __trace_bprintk(_THIS_IP_, trace_printk_fmt, ##args);   \
601         else                                                            \
602                 __trace_printk(_THIS_IP_, fmt, ##args);                 \
603 } while (0)
604 
605 extern __printf(2, 3)
606 int __trace_bprintk(unsigned long ip, const char *fmt, ...);
607 
608 extern __printf(2, 3)
609 int __trace_printk(unsigned long ip, const char *fmt, ...);
610 
611 /**
612  * trace_puts - write a string into the ftrace buffer
613  * @str: the string to record
614  *
615  * Note: __trace_bputs is an internal function for trace_puts and
616  *       the @ip is passed in via the trace_puts macro.
617  *
618  * This is similar to trace_printk() but is made for those really fast
619  * paths that a developer wants the least amount of "Heisenbug" affects,
620  * where the processing of the print format is still too much.
621  *
622  * This function allows a kernel developer to debug fast path sections
623  * that printk is not appropriate for. By scattering in various
624  * printk like tracing in the code, a developer can quickly see
625  * where problems are occurring.
626  *
627  * This is intended as a debugging tool for the developer only.
628  * Please refrain from leaving trace_puts scattered around in
629  * your code. (Extra memory is used for special buffers that are
630  * allocated when trace_puts() is used)
631  *
632  * Returns: 0 if nothing was written, positive # if string was.
633  *  (1 when __trace_bputs is used, strlen(str) when __trace_puts is used)
634  */
635 
636 #define trace_puts(str) ({                                              \
637         static const char *trace_printk_fmt                             \
638                 __attribute__((section("__trace_printk_fmt"))) =        \
639                 __builtin_constant_p(str) ? str : NULL;                 \
640                                                                         \
641         if (__builtin_constant_p(str))                                  \
642                 __trace_bputs(_THIS_IP_, trace_printk_fmt);             \
643         else                                                            \
644                 __trace_puts(_THIS_IP_, str, strlen(str));              \
645 })
646 extern int __trace_bputs(unsigned long ip, const char *str);
647 extern int __trace_puts(unsigned long ip, const char *str, int size);
648 
649 extern void trace_dump_stack(int skip);
650 
651 /*
652  * The double __builtin_constant_p is because gcc will give us an error
653  * if we try to allocate the static variable to fmt if it is not a
654  * constant. Even with the outer if statement.
655  */
656 #define ftrace_vprintk(fmt, vargs)                                      \
657 do {                                                                    \
658         if (__builtin_constant_p(fmt)) {                                \
659                 static const char *trace_printk_fmt                     \
660                   __attribute__((section("__trace_printk_fmt"))) =      \
661                         __builtin_constant_p(fmt) ? fmt : NULL;         \
662                                                                         \
663                 __ftrace_vbprintk(_THIS_IP_, trace_printk_fmt, vargs);  \
664         } else                                                          \
665                 __ftrace_vprintk(_THIS_IP_, fmt, vargs);                \
666 } while (0)
667 
668 extern int
669 __ftrace_vbprintk(unsigned long ip, const char *fmt, va_list ap);
670 
671 extern int
672 __ftrace_vprintk(unsigned long ip, const char *fmt, va_list ap);
673 
674 extern void ftrace_dump(enum ftrace_dump_mode oops_dump_mode);
675 #else
676 static inline void tracing_start(void) { }
677 static inline void tracing_stop(void) { }
678 static inline void trace_dump_stack(int skip) { }
679 
680 static inline void tracing_on(void) { }
681 static inline void tracing_off(void) { }
682 static inline int tracing_is_on(void) { return 0; }
683 static inline void tracing_snapshot(void) { }
684 static inline void tracing_snapshot_alloc(void) { }
685 
686 static inline __printf(1, 2)
687 int trace_printk(const char *fmt, ...)
688 {
689         return 0;
690 }
691 static inline int
692 ftrace_vprintk(const char *fmt, va_list ap)
693 {
694         return 0;
695 }
696 static inline void ftrace_dump(enum ftrace_dump_mode oops_dump_mode) { }
697 #endif /* CONFIG_TRACING */
698 
699 /*
700  * min()/max()/clamp() macros that also do
701  * strict type-checking.. See the
702  * "unnecessary" pointer comparison.
703  */
704 #define min(x, y) ({                            \
705         typeof(x) _min1 = (x);                  \
706         typeof(y) _min2 = (y);                  \
707         (void) (&_min1 == &_min2);              \
708         _min1 < _min2 ? _min1 : _min2; })
709 
710 #define max(x, y) ({                            \
711         typeof(x) _max1 = (x);                  \
712         typeof(y) _max2 = (y);                  \
713         (void) (&_max1 == &_max2);              \
714         _max1 > _max2 ? _max1 : _max2; })
715 
716 #define min3(x, y, z) min((typeof(x))min(x, y), z)
717 #define max3(x, y, z) max((typeof(x))max(x, y), z)
718 
719 /**
720  * min_not_zero - return the minimum that is _not_ zero, unless both are zero
721  * @x: value1
722  * @y: value2
723  */
724 #define min_not_zero(x, y) ({                   \
725         typeof(x) __x = (x);                    \
726         typeof(y) __y = (y);                    \
727         __x == 0 ? __y : ((__y == 0) ? __x : min(__x, __y)); })
728 
729 /**
730  * clamp - return a value clamped to a given range with strict typechecking
731  * @val: current value
732  * @lo: lowest allowable value
733  * @hi: highest allowable value
734  *
735  * This macro does strict typechecking of lo/hi to make sure they are of the
736  * same type as val.  See the unnecessary pointer comparisons.
737  */
738 #define clamp(val, lo, hi) min((typeof(val))max(val, lo), hi)
739 
740 /*
741  * ..and if you can't take the strict
742  * types, you can specify one yourself.
743  *
744  * Or not use min/max/clamp at all, of course.
745  */
746 #define min_t(type, x, y) ({                    \
747         type __min1 = (x);                      \
748         type __min2 = (y);                      \
749         __min1 < __min2 ? __min1: __min2; })
750 
751 #define max_t(type, x, y) ({                    \
752         type __max1 = (x);                      \
753         type __max2 = (y);                      \
754         __max1 > __max2 ? __max1: __max2; })
755 
756 /**
757  * clamp_t - return a value clamped to a given range using a given type
758  * @type: the type of variable to use
759  * @val: current value
760  * @lo: minimum allowable value
761  * @hi: maximum allowable value
762  *
763  * This macro does no typechecking and uses temporary variables of type
764  * 'type' to make all the comparisons.
765  */
766 #define clamp_t(type, val, lo, hi) min_t(type, max_t(type, val, lo), hi)
767 
768 /**
769  * clamp_val - return a value clamped to a given range using val's type
770  * @val: current value
771  * @lo: minimum allowable value
772  * @hi: maximum allowable value
773  *
774  * This macro does no typechecking and uses temporary variables of whatever
775  * type the input argument 'val' is.  This is useful when val is an unsigned
776  * type and min and max are literals that will otherwise be assigned a signed
777  * integer type.
778  */
779 #define clamp_val(val, lo, hi) clamp_t(typeof(val), val, lo, hi)
780 
781 
782 /*
783  * swap - swap value of @a and @b
784  */
785 #define swap(a, b) \
786         do { typeof(a) __tmp = (a); (a) = (b); (b) = __tmp; } while (0)
787 
788 /**
789  * container_of - cast a member of a structure out to the containing structure
790  * @ptr:        the pointer to the member.
791  * @type:       the type of the container struct this is embedded in.
792  * @member:     the name of the member within the struct.
793  *
794  */
795 #define container_of(ptr, type, member) ({                      \
796         const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
797         (type *)( (char *)__mptr - offsetof(type,member) );})
798 
799 /* Trap pasters of __FUNCTION__ at compile-time */
800 #define __FUNCTION__ (__func__)
801 
802 /* Rebuild everything on CONFIG_FTRACE_MCOUNT_RECORD */
803 #ifdef CONFIG_FTRACE_MCOUNT_RECORD
804 # define REBUILD_DUE_TO_FTRACE_MCOUNT_RECORD
805 #endif
806 
807 /* Permissions on a sysfs file: you didn't miss the 0 prefix did you? */
808 #define VERIFY_OCTAL_PERMISSIONS(perms)                                 \
809         (BUILD_BUG_ON_ZERO((perms) < 0) +                               \
810          BUILD_BUG_ON_ZERO((perms) > 0777) +                            \
811          /* User perms >= group perms >= other perms */                 \
812          BUILD_BUG_ON_ZERO(((perms) >> 6) < (((perms) >> 3) & 7)) +     \
813          BUILD_BUG_ON_ZERO((((perms) >> 3) & 7) < ((perms) & 7)) +      \
814          /* Other writable?  Generally considered a bad idea. */        \
815          BUILD_BUG_ON_ZERO((perms) & 2) +                               \
816          (perms))
817 #endif
818 

This page was automatically generated by LXR 0.3.1 (source).  •  Linux is a registered trademark of Linus Torvalds  •  Contact us

    Home
    Development
    Services
    Training
    Docs
    Community
    Company
    Blog


