#define EFL_IF   0x00000200 //开启中断

#define CR4_PSE  0x00000010 //页面大小扩展
#define CR0_PG 0x80000000
#define CR0_WP 0x00010000
#define KSTACKSIZE 4096



//虚拟地址和物理地址的转换
#define V2P(a) (((uint) (a)) - KERNBASE)
#define P2V(a) ((void *)(((char *) (a)) + KERNBASE))

#define V2P_WO(x) ((x) - KERNBASE)    
#define P2V_WO(x) ((x) + KERNBASE)   


//页表
// 
// +--------10------+-------10-------+---------12----------+
// | Page Directory |   Page Table   | Offset within Page  |
// |      Index     |      Index     |                     |
// +----------------+----------------+---------------------+
//  \--- PDX(va) --/ \--- PTX(va) --/

// 页目录偏移
#define PDX(va)         (((uint)(va) >> PDXSHIFT) & 0x3FF)

// 页表索引
#define PTX(va)         (((uint)(va) >> PTXSHIFT) & 0x3FF)


// 虚拟地址（目录，页表，页内偏移）
#define PGADDR(d, t, o) ((uint)((d) << PDXSHIFT | (t) << PTXSHIFT | (o)))


#define NPDENTRIES      1024    //  页目录项数
#define NPTENTRIES      1024    //  每页的页表数
#define PGSIZE          4096    // 页内大小

#define PTXSHIFT        12      
#define PDXSHIFT        22      

//页大小对齐宏
#define PGROUNDUP(sz)  (((sz)+PGSIZE-1) & ~(PGSIZE-1))
#define PGROUNDDOWN(a) (((a)) & ~(PGSIZE-1))

// PTE/PDE flgs
#define PTE_P           0x001   // 占用
#define PTE_W           0x002   // 可写
#define PTE_U           0x004   // User
#define PTE_PS          0x080   // Page Size

#define PTE_ADDR(pte) ((uint)(pte) & ~0xFFF)

// PTE 的地址和偏移
#define PTE_ADDR(pte)   ((uint)(pte) & ~0xFFF)
#define PTE_FLAGS(pte)  ((uint)(pte) &  0xFFF)

//内存布局映射
#define KERNBASE 0x80000000
#define KERNLINK 0x80100000
#define EXTMEM  0x100000
#define PHYSTOP 0xE000000           // 物理内存最大值
#define DEVSPACE 0xFE000000    

#ifndef __ASSEMBLER__
#include "types.h"
//GDT
struct segdesc {
  uint lim_15_0 : 16;  // Low bits of segment limit
  uint base_15_0 : 16; // Low bits of segment base address
  uint base_23_16 : 8; // Middle bits of segment base address
  uint type : 4;       // Segment type (see STS_ constants)
  uint s : 1;          // 0 = system, 1 = application
  uint dpl : 2;        // Descriptor Privilege Level
  uint p : 1;          // Present
  uint lim_19_16 : 4;  // High bits of segment limit
  uint avl : 1;        // Unused (available for software use)
  uint rsv1 : 1;       // Reserved
  uint db : 1;         // 0 = 16-bit segment, 1 = 32-bit segment
  uint g : 1;          // Granularity: limit scaled by 4K when set
  uint base_31_24 : 8; // High bits of segment base address
};

//TSS

struct taskstate {
  uint link;         // Old ts selector
  uint esp0;         // Stack pointers and segment selectors
  ushort ss0;        //   after an increase in privilege level
  ushort padding1;
  uint *esp1;
  ushort ss1;
  ushort padding2;
  uint *esp2;
  ushort ss2;
  ushort padding3;
  void *cr3;         // Page directory base
  uint *eip;         // Saved state from last task switch
  uint eflags;
  uint eax;          // More saved state (registers)
  uint ecx;
  uint edx;
  uint ebx;
  uint *esp;
  uint *ebp;
  uint esi;
  uint edi;
  ushort es;         // Even more saved state (segment selectors)
  ushort padding4;
  ushort cs;
  ushort padding5;
  ushort ss;
  ushort padding6;
  ushort ds;
  ushort padding7;
  ushort fs;
  ushort padding8;
  ushort gs;
  ushort padding9;
  ushort ldt;
  ushort padding10;
  ushort t;          // Trap on task switch
  ushort iomb;       // I/O map base address
};
// 设置如上 GDT 结构的宏
#define SEG(type, base, lim, dpl) (struct segdesc)    \
{ ((lim) >> 12) & 0xffff, (uint)(base) & 0xffff,      \
  ((uint)(base) >> 16) & 0xff, type, 1, dpl, 1,       \
  (uint)(lim) >> 28, 0, 0, 1, 1, (uint)(base) >> 24 }
#define SEG16(type, base, lim, dpl) (struct segdesc)  \
{ (lim) & 0xffff, (uint)(base) & 0xffff,              \
  ((uint)(base) >> 16) & 0xff, type, 1, dpl, 1,       \
  (uint)(lim) >> 16, 0, 0, 1, 0, (uint)(base) >> 24 }

#endif

#define DPL_USER    0x3     // 用户DPL

// 段类型
#define STA_X       0x8     // 可执行
#define STA_W       0x2     // 可写
#define STA_R       0x2     // 可读


// 段选择器
#define SEG_KCODE 1 // 内核代码
#define SEG_KDATA 2 // 
#define SEG_UCODE 3 // 用户代码
#define SEG_UDATA 4 // 
#define SEG_TSS   5 //

#define STS_T32A    0x9     // Available 32-bit TSS
