Linux Cross Reference
Free Electrons
Embedded Linux Experts

 • source navigation  • diff markup  • identifier search  • freetext search  • 

Version:  2.0.40 2.2.26 2.4.37 3.2 3.3 3.4 3.5 3.6 3.7 3.8 3.9 3.10 3.11 3.12 3.13 3.14 3.15 3.16 3.17 3.18
Linux/include/linux/types.h

  1 #ifndef _LINUX_TYPES_H
  2 #define _LINUX_TYPES_H
  3 
  4 #define __EXPORTED_HEADERS__
  5 #include <uapi/linux/types.h>
  6 
  7 #ifndef __ASSEMBLY__
  8 
  9 #define DECLARE_BITMAP(name,bits) \
 10         unsigned long name[BITS_TO_LONGS(bits)]
 11 
 12 typedef __u32 __kernel_dev_t;
 13 
 14 typedef __kernel_fd_set         fd_set;
 15 typedef __kernel_dev_t          dev_t;
 16 typedef __kernel_ino_t          ino_t;
 17 typedef __kernel_mode_t         mode_t;
 18 typedef unsigned short          umode_t;
 19 typedef __u32                   nlink_t;
 20 typedef __kernel_off_t          off_t;
 21 typedef __kernel_pid_t          pid_t;
 22 typedef __kernel_daddr_t        daddr_t;
 23 typedef __kernel_key_t          key_t;
 24 typedef __kernel_suseconds_t    suseconds_t;
 25 typedef __kernel_timer_t        timer_t;
 26 typedef __kernel_clockid_t      clockid_t;
 27 typedef __kernel_mqd_t          mqd_t;
 28 
 29 typedef _Bool                   bool;
 30 
 31 typedef __kernel_uid32_t        uid_t;
 32 typedef __kernel_gid32_t        gid_t;
 33 typedef __kernel_uid16_t        uid16_t;
 34 typedef __kernel_gid16_t        gid16_t;
 35 
 36 typedef unsigned long           uintptr_t;
 37 
 38 #ifdef CONFIG_UID16
 39 /* This is defined by include/asm-{arch}/posix_types.h */
 40 typedef __kernel_old_uid_t      old_uid_t;
 41 typedef __kernel_old_gid_t      old_gid_t;
 42 #endif /* CONFIG_UID16 */
 43 
 44 #if defined(__GNUC__)
 45 typedef __kernel_loff_t         loff_t;
 46 #endif
 47 
 48 /*
 49  * The following typedefs are also protected by individual ifdefs for
 50  * historical reasons:
 51  */
 52 #ifndef _SIZE_T
 53 #define _SIZE_T
 54 typedef __kernel_size_t         size_t;
 55 #endif
 56 
 57 #ifndef _SSIZE_T
 58 #define _SSIZE_T
 59 typedef __kernel_ssize_t        ssize_t;
 60 #endif
 61 
 62 #ifndef _PTRDIFF_T
 63 #define _PTRDIFF_T
 64 typedef __kernel_ptrdiff_t      ptrdiff_t;
 65 #endif
 66 
 67 #ifndef _TIME_T
 68 #define _TIME_T
 69 typedef __kernel_time_t         time_t;
 70 #endif
 71 
 72 #ifndef _CLOCK_T
 73 #define _CLOCK_T
 74 typedef __kernel_clock_t        clock_t;
 75 #endif
 76 
 77 #ifndef _CADDR_T
 78 #define _CADDR_T
 79 typedef __kernel_caddr_t        caddr_t;
 80 #endif
 81 
 82 /* bsd */
 83 typedef unsigned char           u_char;
 84 typedef unsigned short          u_short;
 85 typedef unsigned int            u_int;
 86 typedef unsigned long           u_long;
 87 
 88 /* sysv */
 89 typedef unsigned char           unchar;
 90 typedef unsigned short          ushort;
 91 typedef unsigned int            uint;
 92 typedef unsigned long           ulong;
 93 
 94 #ifndef __BIT_TYPES_DEFINED__
 95 #define __BIT_TYPES_DEFINED__
 96 
 97 typedef         __u8            u_int8_t;
 98 typedef         __s8            int8_t;
 99 typedef         __u16           u_int16_t;
100 typedef         __s16           int16_t;
101 typedef         __u32           u_int32_t;
102 typedef         __s32           int32_t;
103 
104 #endif /* !(__BIT_TYPES_DEFINED__) */
105 
106 typedef         __u8            uint8_t;
107 typedef         __u16           uint16_t;
108 typedef         __u32           uint32_t;
109 
110 #if defined(__GNUC__)
111 typedef         __u64           uint64_t;
112 typedef         __u64           u_int64_t;
113 typedef         __s64           int64_t;
114 #endif
115 
116 /* this is a special 64bit data type that is 8-byte aligned */
117 #define aligned_u64 __u64 __attribute__((aligned(8)))
118 #define aligned_be64 __be64 __attribute__((aligned(8)))
119 #define aligned_le64 __le64 __attribute__((aligned(8)))
120 
121 /**
122  * The type used for indexing onto a disc or disc partition.
123  *
124  * Linux always considers sectors to be 512 bytes long independently
125  * of the devices real block size.
126  *
127  * blkcnt_t is the type of the inode's block count.
128  */
129 #ifdef CONFIG_LBDAF
130 typedef u64 sector_t;
131 typedef u64 blkcnt_t;
132 #else
133 typedef unsigned long sector_t;
134 typedef unsigned long blkcnt_t;
135 #endif
136 
137 /*
138  * The type of an index into the pagecache.  Use a #define so asm/types.h
139  * can override it.
140  */
141 #ifndef pgoff_t
142 #define pgoff_t unsigned long
143 #endif
144 
145 /* A dma_addr_t can hold any valid DMA or bus address for the platform */
146 #ifdef CONFIG_ARCH_DMA_ADDR_T_64BIT
147 typedef u64 dma_addr_t;
148 #else
149 typedef u32 dma_addr_t;
150 #endif /* dma_addr_t */
151 
152 #ifdef __CHECKER__
153 #else
154 #endif
155 #ifdef __CHECK_ENDIAN__
156 #else
157 #endif
158 typedef unsigned __bitwise__ gfp_t;
159 typedef unsigned __bitwise__ fmode_t;
160 typedef unsigned __bitwise__ oom_flags_t;
161 
162 #ifdef CONFIG_PHYS_ADDR_T_64BIT
163 typedef u64 phys_addr_t;
164 #else
165 typedef u32 phys_addr_t;
166 #endif
167 
168 typedef phys_addr_t resource_size_t;
169 
170 /*
171  * This type is the placeholder for a hardware interrupt number. It has to be
172  * big enough to enclose whatever representation is used by a given platform.
173  */
174 typedef unsigned long irq_hw_number_t;
175 
176 typedef struct {
177         int counter;
178 } atomic_t;
179 
180 #ifdef CONFIG_64BIT
181 typedef struct {
182         long counter;
183 } atomic64_t;
184 #endif
185 
186 struct list_head {
187         struct list_head *next, *prev;
188 };
189 
190 struct hlist_head {
191         struct hlist_node *first;
192 };
193 
194 struct hlist_node {
195         struct hlist_node *next, **pprev;
196 };
197 
198 struct ustat {
199         __kernel_daddr_t        f_tfree;
200         __kernel_ino_t          f_tinode;
201         char                    f_fname[6];
202         char                    f_fpack[6];
203 };
204 
205 /**
206  * struct callback_head - callback structure for use with RCU and task_work
207  * @next: next update requests in a list
208  * @func: actual update function to call after the grace period.
209  */
210 struct callback_head {
211         struct callback_head *next;
212         void (*func)(struct callback_head *head);
213 };
214 #define rcu_head callback_head
215 
216 #endif /*  __ASSEMBLY__ */
217 #endif /* _LINUX_TYPES_H */
218 

This page was automatically generated by LXR 0.3.1 (source).  •  Linux is a registered trademark of Linus Torvalds  •  Contact us

    Home
    Development
    Services
    Training
    Docs
    Community
    Company
    Blog


