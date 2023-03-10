#include <linux/kernel.h>       
#include <linux/module.h>
#include <linux/ioport.h>
#include <asm/errno.h>
#include <asm/io.h>
#include <linux/sysfs.h>          

MODULE_INFO(intree, "Y");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Andrzej Tlomak");
MODULE_DESCRIPTION("Kernel module for sykom 2022Z");

#define BUF_SIZE    1024
#define SYKT_MEM_SIZE 16
#define SYKT_EXIT (0x3333)
#define SYKT_EXIT_CODE (0x7F)

#define SYKT_MEM_BASE_ADDR  (0x00100000)

// Addresses
#define SYKT_GPIO_ADDR_SPACE (baseptr)
#define SYKT_GPIO_BASE_ADDR    (0x0000)
#define SYKT_GPIO_CTRL_ADDR (0x108)
#define SYKT_GPIO_A1_ADDR   (0xf8)
#define SYKT_GPIO_A2_ADDR   (0xfc)
#define SYKT_GPIO_W_ADDR    (0x100)
#define SYKT_GPIO_S_ADDR    (0x104)

void __iomem *baseptr;
static struct kobject *kobj_ref;
static uint32_t tae3arg1_value;
static uint32_t tae3arg2_value;
static uint32_t tae3result_value;
static uint32_t tae3status_value;
      
              
static ssize_t tae3arg1_store(struct kobject *kobj, 
                struct kobj_attribute *attr,const char *buf, size_t count)
{
        sscanf(buf,"%d",&tae3arg1_value);
		writel(tae3arg1_value, SYKT_GPIO_ADDR_SPACE+SYKT_GPIO_A1_ADDR);
        return count;
}

static ssize_t tae3arg2_store(struct kobject *kobj, 
                struct kobj_attribute *attr,const char *buf, size_t count)
{
        sscanf(buf,"%d",&tae3arg2_value);
		writel(tae3arg2_value, SYKT_GPIO_ADDR_SPACE+SYKT_GPIO_A2_ADDR);
        return count;
}
static ssize_t tae3result_show(struct kobject *kobj, 
                struct kobj_attribute *attr, char *buf)
{
	tae3result_value = readl(SYKT_GPIO_ADDR_SPACE+SYKT_GPIO_W_ADDR);
    return sprintf(buf, "%d", tae3result_value);
}

static ssize_t tae3status_show(struct kobject *kobj, 
                struct kobj_attribute *attr, char *buf)
{
	tae3status_value = readl(SYKT_GPIO_ADDR_SPACE+SYKT_GPIO_S_ADDR);
    return sprintf(buf, "%d", tae3status_value);
}




static struct kobj_attribute tae3arg1_attr = __ATTR_WO(tae3arg1);
static struct kobj_attribute tae3arg2_attr = __ATTR_WO(tae3arg2);
static struct kobj_attribute tae3result_attr = __ATTR_RO(tae3result);
static struct kobj_attribute tae3status_attr = __ATTR_RO(tae3status);

int my_sysfs_init(void){
    printk(KERN_ALERT "Starting of the module...\n");
    baseptr=ioremap(SYKT_MEM_BASE_ADDR, SYKT_MEM_SIZE);
    kobj_ref = kobject_create_and_add("sykom", NULL);                  
    sysfs_create_file(kobj_ref,&tae3arg1_attr.attr);
    sysfs_create_file(kobj_ref,&tae3arg2_attr.attr);
    sysfs_create_file(kobj_ref,&tae3result_attr.attr);
    sysfs_create_file(kobj_ref,&tae3status_attr.attr);

    return 0;
}

void my_sysfs_cleanup(void){
    printk(KERN_WARNING "Stoping of the module...\n");

    writel(SYKT_EXIT | ((SYKT_EXIT_CODE)<<16), baseptr);
	kobject_put(kobj_ref); 
	sysfs_remove_file(kernel_kobj, &tae3arg1_attr.attr);
	sysfs_remove_file(kernel_kobj, &tae3arg2_attr.attr);
	sysfs_remove_file(kernel_kobj, &tae3result_attr.attr);
	sysfs_remove_file(kernel_kobj, &tae3status_attr.attr);
	iounmap(baseptr);  
}

module_init(my_sysfs_init);
module_exit(my_sysfs_cleanup);
