#include <linux/module.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/mm.h>
#include <linux/errno.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/miscdevice.h> 
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "rw_bios_nv.h"       
#include "bios_nv.h"
#define ALLOC_SIZE  500
#define KDA_MINOR 44



static char buf_dev[ALLOC_SIZE] = "kernel_data_2011_1_1";
unsigned char *kmallocmem;
unsigned long PALM=0;

static int RW_BIOS_NV_open(struct inode *inode, struct file *file)
{
    printk("%s, %d/n", __FUNCTION__, __LINE__);;
    return 0;
}

static ssize_t RW_BIOS_NV_write(struct file *file,const char *buf,size_t count,loff_t* f_pos){
        copy_from_user(buf_dev,buf,count);
        return 0;
}
static ssize_t RW_BIOS_NV_read(struct file *file,char *buf,size_t count,loff_t* f_pos){
        copy_to_user(buf,buf_dev,count);
        return count;
}


static long RW_BIOS_NV_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int retval = 0;
	int err = 0;
	unsigned short RetStatus=0;
	//GET_SMBIOS_INFO *pGetSmIf = NULL;
	BIOS_VAR_STRUCTURE *pGetVarStru=NULL;
	BIOS_VAR_STRUCTURE *pSetVarStru=NULL;

	int i=0;

    for(i=0;i<ALLOC_SIZE;++i){
	   *(kmallocmem+i) = 0;
	}
	
    if (_IOC_TYPE(cmd) != RW_BIOS_NV_IOC_MAGIC) 
        return -EINVAL;
    if (_IOC_NR(cmd) > RW_BIOS_NV_IOC_MAXNR) 
        return -EINVAL;

    if (_IOC_DIR(cmd) & _IOC_READ)
        err = !access_ok(VERIFY_WRITE, (void *)arg, _IOC_SIZE(cmd));
    else if (_IOC_DIR(cmd) & _IOC_WRITE)
        err = !access_ok(VERIFY_READ, (void *)arg, _IOC_SIZE(cmd));
    if (err) 
        return -EFAULT;
		
  
        switch (cmd)
       {
	      case RW_BIOS_NV_IOCPRINT:
		        printk(KERN_INFO "RW_BIOS_NV_IOCPRINT!\n");
		  break;
          case RW_BIOS_NV_IOCGETDATA:
            retval = copy_to_user((unsigned long *)arg,&PALM,sizeof(unsigned long));
			if(retval!=0){
              printk(KERN_INFO "get palm error:%x\n",retval);
			}
          break;
	      case RW_BIOS_NV_IOCGETBIOSNV:	
                  pGetVarStru = (BIOS_VAR_STRUCTURE *)kmallocmem;
		  retval= copy_from_user(pGetVarStru, (BIOS_VAR_STRUCTURE *)arg, sizeof(BIOS_VAR_STRUCTURE));
			if(retval!=0){
			  printk(KERN_INFO "get RW_BIOS_NV_IOCGETBIOSNV bios_nvram_name error:%x\n",retval);
			}
                   			
           
		  printk("get var stru var_name:%x,var_value:%x,RetStatus:%x\n",pGetVarStru->var_name
		                                        ,pGetVarStru->var_value
											    ,pGetVarStru->RetStatus
		  );
			
		    __asm__ ("movq %1, %%rbx; movw $178,%%dx;mov $243,%%al; out %%al,%%dx;"
             :"=a"(RetStatus)        /* output */
             :"r"(PALM)         /* input */
             : "%rbx", "%edx"         /* clobbered register */
             );
			printk("RetStatus GET BIOS NVRAM status:%x\n",RetStatus);		
                       printk("read bios nv value:%x\n", pGetVarStru->var_value);
			if(RetStatus==1){
				retval = copy_to_user((BIOS_VAR_STRUCTURE *)arg,pGetVarStru,sizeof(BIOS_VAR_STRUCTURE));
			}
		  break;


		  case RW_BIOS_NV_IOCSETBIOSNV:
		  pSetVarStru =  (BIOS_VAR_STRUCTURE *)kmallocmem;
		  if (copy_from_user(pSetVarStru, (BIOS_VAR_STRUCTURE *)arg, sizeof(BIOS_VAR_STRUCTURE)))
          {
            printk("Set BIOS NV RAM  copy from user error.\n");
            return -EFAULT;
          }
		  printk("var_name:%x,var_value:%x,RetStatus:%x\n",pSetVarStru->var_name
		                                        ,pSetVarStru->var_value
											    ,pSetVarStru->RetStatus
		  );
		  
	      //call smi set smbios struct
		    __asm__ ("movq %1, %%rbx; movw $178,%%dx;mov $244,%%al; out %%al,%%dx;"
             :"=a"(RetStatus)        /* output */
             :"r"(PALM)         /* input */
             : "%rbx", "%edx"         /* clobbered register */
             );
			printk("RetStatus SET BIOS NVRAM:%x\n",RetStatus);		  
			if(RetStatus==1){
				retval = copy_to_user((BIOS_VAR_STRUCTURE *)arg,pSetVarStru,sizeof(BIOS_VAR_STRUCTURE));
			}
		  
		  break;
          default:
          return - EINVAL;
       }
       return 0;
}
   
static struct file_operations kda_fops = {
    .owner = THIS_MODULE,
    .open  = RW_BIOS_NV_open,  
    .read  = RW_BIOS_NV_read,
    .write = RW_BIOS_NV_write,
    .unlocked_ioctl  = RW_BIOS_NV_ioctl,	
};



static struct miscdevice kda_dev = {
    KDA_MINOR,
    "RW_BIOS_NV",
    &kda_fops
};

static int __init RW_BIOS_NV_init(void)
{
    int err;
    unsigned int i=0;
    err = misc_register(&kda_dev);
    if (err) {
        printk("envctrl: Unable to get misc minor %d/n",
               kda_dev.minor);
    }else{
	    printk("RW_BIOS_NV install module success.\n");
	}
	
    kmallocmem = (unsigned char*)kmalloc(ALLOC_SIZE, __GFP_DMA);
	if(kmallocmem!=NULL){
	  PALM = virt_to_phys(kmallocmem);
	  //init memory
	  for(i=0;i<ALLOC_SIZE;++i){
	    *(kmallocmem+i) = 0;
	  }
      printk("<1>kmallocmem addr=%lx,PALM:%lx", (unsigned long)kmallocmem,PALM);
	}

    return 0;
}

static void __exit RW_BIOS_NV_exit(void)
{
    printk("RW_BIOS_NV remove module success.\n");
	kfree(kmallocmem);
    misc_deregister(&kda_dev);
}

module_init(RW_BIOS_NV_init);
module_exit(RW_BIOS_NV_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("dsg208");
