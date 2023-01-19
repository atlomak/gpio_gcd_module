#include <linux/kernel.h>       
#include <linux/module.h>      
#include <linux/proc_fs.h>      
#include <linux/uaccess.h>
#include <asm/uaccess.h>        
#include <asm/io.h>             


#define BUF_SIZE    100
#define SYKT_MEM_SIZE 16
#define SYKT_EXIT (0x3333)
#define SYKT_EXIT_CODE (0x7F)

#define SYKT_MEM_BASE_ADDR (0x00100000)
#define SYKT_GPIO_BASE_SPACE (0x0000)
#define SYKT_GPIO_CTRL_ADDR (0x108)

void __iomem *baseptr;
void __iomem *baseptr_1;          
      
              

static ssize_t gpio_read(struct file *file, char __user *ubuf, size_t count, loff_t *offset){
    char buf[BUF_SIZE];
    int len=0;

    if(*offset>0 || count<BUF_SIZE){
        return 0;                           
    }

    len=snprintf(buf, BUF_SIZE, "%x\n", readl(baseptr_1)); 
                                           
    if(copy_to_user(ubuf, buf, len)){       
        return -EFAULT;    
    }

    *offset = len;
    return len;
}


static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = gpio_read,
};

struct proc_dir_entry *parent;
static struct proc_dir_entry *ent;

static int procfs_init(void){
    printk(KERN_ALERT "Starting of the module...\n");

    parent=proc_mkdir("sykom", NULL);                   
    ent=proc_create("gpio_control", 0660, parent, &fops); 

    baseptr=ioremap(SYKT_MEM_BASE_ADDR+SYKT_GPIO_BASE_SPACE, SYKT_MEM_SIZE);
    baseptr_1=ioremap(SYKT_MEM_BASE_ADDR+SYKT_GPIO_CTRL_ADDR, SYKT_MEM_SIZE);

    return 0;
}

static void procfs_cleanup(void){
    printk(KERN_WARNING "Stoping of the module...\n");

    proc_remove(ent);
    remove_proc_entry("sykom", NULL);

    writel(SYKT_EXIT | ((127)<<16), baseptr);    
}

module_init(procfs_init);
module_exit(procfs_cleanup);

MODULE_INFO(intree, "Y");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Andrzej Tlomak");
MODULE_DESCRIPTION("Kernel module for sykom");