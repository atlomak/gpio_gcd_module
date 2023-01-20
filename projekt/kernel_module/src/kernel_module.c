#include <linux/kernel.h>       
#include <linux/module.h>
#include <linux/ioport.h>
#include <asm/errno.h>
#include <asm/io.h>
#include <linux/sysfs.h>          


#define BUF_SIZE    100
#define SYKT_MEM_SIZE 16
#define SYKT_EXIT (0x3333)
#define SYKT_EXIT_CODE (0x7F)

#define SYKT_MEM_BASE_ADDR  (0x00100000)

// Addresses
#define SYKT_GPIO_BASE_SPACE    (0x0000)
#define SYKT_GPIO_CTRL_ADDR (0x108)
#define SYKT_GPIO_A1_ADDR   (0xf8)
#define SYKT_GPIO_A2_ADDR   (0xfc)
#define SYKT_GPIO_W_ADDR    (0x100)
#define SYKT_GPIO_S_ADDR    (0x104)

void __iomem *baseptr; 
void __iomem *baseptr_ctrl;
void __iomem *baseptr_a1;  
void __iomem *baseptr_a2;
void __iomem *baseptr_w;  
void __iomem *baseptr_s;           
      
              

static ssize_t gpio_read_ctrl(struct file *file, char __user *ubuf, size_t count, loff_t *offset){
    char buf[BUF_SIZE];
    int len=0;
    if(*offset>0 || count<BUF_SIZE){
        return 0;                           
    }

    len=snprintf(buf, BUF_SIZE, "%x\n", readl(baseptr_ctrl)); 
                                           
    if(copy_to_user(ubuf, buf, len)){       
        return -EFAULT;    
    }
    *offset = len;
    return len;
}

static ssize_t gpio_write_a1(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count){
        unsigned int value = 0x0;
        sscanf(buffer, "%d", &value);
        writel(value, baseptr_A1);
}

static ssize_t gpio_write_a2(struct file *file, const char __user *ubuf, size_t count, loff_t *offset){
        static uint32_t a2_value;
        sscanf(ubuf,"%d", &a2_value);
		writel(a2_value, baseptr_a2);
        return count;
}

static ssize_t gpio_read_w(struct file *file, char __user *ubuf, size_t count, loff_t *offset){
    char buf[BUF_SIZE];
    int len=0;
    if(*offset>0 || count<BUF_SIZE){
        return 0;                           
    }
    len=snprintf(buf, BUF_SIZE, "%x\n", readl(baseptr_w)); 
                                           
    if(copy_to_user(ubuf, buf, len)){       
        return -EFAULT;    
    }
    *offset = len;
    return len;
}
static ssize_t gpio_read_s(struct file *file, char __user *ubuf, size_t count, loff_t *offset){
    char buf[BUF_SIZE];
    int len=0;
    if(*offset>0 || count<BUF_SIZE){
        return 0;                           
    }
    len=snprintf(buf, BUF_SIZE, "%x\n", readl(baseptr_s));                                
    if(copy_to_user(ubuf, buf, len)){       
        return -EFAULT;    
    }
    *offset = len;
    return len;
}





static struct kobject *parent;
static struct kobj_attribute A1_attr = __ATTR_WO();
static struct kobj_attribute A2_attr = __ATTR(bpfbarg2, 0660, sykt_sysfs_show, sykt_sysfs_store);
static struct kobj_attribute result_attr = __ATTR(bpfbresult, 0660, sykt_sysfs_show, sykt_sysfs_store);
static struct kobj_attribute status_attr = __ATTR(bpfbstatus, 0660, sykt_sysfs_show, sykt_sysfs_store);

static int procfs_init(void){
    printk(KERN_ALERT "Starting of the module...\n");

    parent=proc_mkdir("sykom", NULL);                   
    ent_ctr=proc_create("gpio_control", 0660, parent, &fops_gpio_control);
    ent_a1=proc_create("gpio_a1", 0660, parent, &fops_gpio_a1); 
    ent_a2=proc_create("gpio_a2", 0660, parent, &fops_gpio_a2); 
    ent_w=proc_create("gpio_w", 0660, parent, &fops_gpio_w); 
    ent_s=proc_create("gpio_s", 0660, parent, &fops_gpio_s);  

    baseptr=ioremap(SYKT_MEM_BASE_ADDR+SYKT_GPIO_BASE_SPACE, SYKT_MEM_SIZE);
    baseptr_ctrl=ioremap(SYKT_MEM_BASE_ADDR+SYKT_GPIO_CTRL_ADDR, SYKT_MEM_SIZE);
    baseptr_a1=ioremap(SYKT_MEM_BASE_ADDR+SYKT_GPIO_A1_ADDR, SYKT_MEM_SIZE);
    baseptr_a2=ioremap(SYKT_MEM_BASE_ADDR+SYKT_GPIO_A2_ADDR, SYKT_MEM_SIZE);
    baseptr_w=ioremap(SYKT_MEM_BASE_ADDR+SYKT_GPIO_W_ADDR, SYKT_MEM_SIZE);
    baseptr_s=ioremap(SYKT_MEM_BASE_ADDR+SYKT_GPIO_S_ADDR, SYKT_MEM_SIZE);

    return 0;
}

static void procfs_cleanup(void){
    printk(KERN_WARNING "Stoping of the module...\n");

    proc_remove(ent_ctr);
    proc_remove(ent_a1);
    proc_remove(ent_a2);
    proc_remove(ent_w);
    proc_remove(ent_s);
    remove_proc_entry("sykom", NULL);

    writel(SYKT_EXIT | ((127)<<16), baseptr);    
}

module_init(procfs_init);
module_exit(procfs_cleanup);

MODULE_INFO(intree, "Y");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Andrzej Tlomak");
MODULE_DESCRIPTION("Kernel module for sykom");