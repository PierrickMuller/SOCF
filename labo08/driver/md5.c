/*
* Nom  : parrot.c
* Auteur : Pierrick Muller
* Date : 5.11.2019
* Version : 0.1
* Description : Module parrot permettant gérant l'écriture et la lecture sur un node,
* avec modification de la casse via une écriture ioctl servant de configuration. 
*
* Basé sur le chablon fournit en cours de DRV
*/


#include <linux/module.h>       /* Needed by all modules */
#include <linux/kernel.h>       /* Needed for KERN_INFO */
#include <linux/init.h>         /* Needed for the macros */
#include <linux/fs.h>           /* Needed for file_operations */
#include <linux/slab.h>         /* Needed for kmalloc */
#include <linux/uaccess.h>      /* copy_(to|from)_user */
#include <linux/cdev.h>		/* Needed for char device registration */

#include <linux/string.h>
#include <linux/io.h> 

typedef volatile unsigned int vuint;

#define MAJOR_NUM       97
#define DEVICE_NAME     "md5"

#define BASE_ADDR          0xFF200000
#define BASE_SIZE          4096



int ret;
int i;
void * addr_base;


static ssize_t
md5_read(struct file *filp, char __user *buf,
            size_t count, loff_t *ppos)
{

	//Si rien n'est écrit ou qu'on demande à lire moins que ce qui est présent dans le buffer, on retourne 0

	/*
	 * Si la position du pointeur dans le fichier est plus grand que la taille du buffer, c'est qu'on essaie
	 * de lire en dehors du buffer, on retourne 0.
	*/
	/*if (*ppos >= buffer_size) {
        	return 0;
    	}*/
	/*
	ret = copy_to_user(buf, global_buffer, buffer_size);
    	if (ret != 0) {
		printk("Parrot : Error during copy to user");	
		return -1;
	}
	//copy_to_user(buf, global_buffer, buffer_size);
	//On modifie le pointeur dans le fichier et on retourne la taille du buffer
    	*ppos = buffer_size;
    	*/
    	char footprint[16];
    	char buffer[34];
    	int i;
    	vuint* addr_data_md5 = addr_base + 0x8;
		size_t buffer_size = sizeof(buffer);
		
		if (buf == 0 || count < buffer_size) {
        	return 0;
    	}
    	
    	if (*ppos >= buffer_size) {
        	return 0;
    	}
    	
		for(i = 0; i < 128/32;i++)
		{
			*((unsigned int *)(footprint+(i*4))) = *addr_data_md5;
		}
		
		for(i=0; i < 16; ++i){ 
			sprintf(buffer + i*2, "%x", footprint[i]);
		}
		sprintf(buffer + buffer_size-2 , "\n");
    

		if (copy_to_user(buf, buffer, buffer_size) != 0){
			printk(KERN_ERR "Erreur de lecture\n");
		}

		*ppos = buffer_size;
    	return buffer_size;
}

static ssize_t
md5_write(struct file *filp, const char __user *buf,
             size_t count, loff_t *ppos)
{
	char * message_512;
	size_t count_bits = count*8;
	size_t size_message_512 = 512/8;
	unsigned long ret;
	int i,y,last_write;
	

	vuint* addr_data_md5 = addr_base + 0x8;
	vuint* addr_start_md5_work = addr_base + 0xc;
	vuint* addr_md5_work_done = addr_base + 0x10;
	vuint* addr_start_md5_block = addr_base + 0x14;
	
	//Si la taille de ce que le user veut écrire est 0, on ne fait rien et on retourne 0
	if (count == 0) {
        	return 0;
    	}
	
	//On place le pointeur au début du fichier
    	*ppos = 0;

    if(size_message_512 != 32768)
    {
		last_write = 1;
		
	}
	else 
	{
		last_write = 0;
	}
    
    message_512 = kmalloc(size_message_512, GFP_KERNEL);
	*addr_start_md5_work = 0x1;

	for(i = 0; i < count_bits/512; i++);
	{
		ret = copy_from_user(message_512,buf+(i*size_message_512),size_message_512);
		if(ret != 0){
			printk("Parrot : Error during copy from user");
			kfree(message_512);
			return -1;
		}

		for(y = 0; y < 512/32; y++);
		{
			*addr_data_md5 = *((unsigned int *)(message_512 + y*4));
		}

		*addr_start_md5_block = 0x1;
		while(*addr_md5_work_done == 0x0){
			}

		*addr_md5_work_done = 0x0;
		*addr_start_md5_block = 0x0;
	}
	if(last_write == 1)
	{
		*addr_start_md5_work = 0x0;
		kfree(message_512);
	}
		
    	return count;
}




/*Cette structure permet de préciser quelles fonctions contenues dans ce fichier correspondent a quelles fonctions propres à un fichier.
* On peut voir notamment le read et le write.
*/
const static struct
file_operations md5_fops = {
    	.owner         = THIS_MODULE,
    	.read          = md5_read,
    	.write         = md5_write,

};

static struct
cdev *md5_cdev;

static int
__init md5_init(void)
{
	//Structure permettant de représenter une device id 
	dev_t md5_dev = MKDEV(MAJOR_NUM,0);

	//On enregistre une range de device_number
	register_chrdev_region(md5_dev,1,DEVICE_NAME);
	
	//On crée la structure cdev pour notre module et on charge le module parrot
	md5_cdev = cdev_alloc();
	md5_cdev->ops = &md5_fops;
	md5_cdev->owner = THIS_MODULE;
	cdev_add(md5_cdev,md5_dev,1);

	addr_base = ioremap(BASE_ADDR, BASE_SIZE);
	
	//On ajoute au log l'information que le module est chargé et prêt
    	printk(KERN_INFO "md5 ready!\n");

    	return 0;
}

static void
__exit md5_exit(void)
{
	//Si le buffer n'est pas vide, on libère la mémoire qui lui était alloué


	// On désenregistre le driver parrot de la liste des modules chargés
	cdev_del(md5_cdev);
	//On ajoute au log l'information que le module Parrot à été supprimé des modules chargés
    	printk(KERN_INFO "md5 bye!\n");
}

//Information sur la licence et l'auteur
MODULE_AUTHOR("REDS");
MODULE_LICENSE("GPL");

//ajout des fonction init et exit en tant que fonction d'initialisation du module et de suppression du module.
module_init(md5_init);
module_exit(md5_exit);
