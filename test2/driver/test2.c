/*
* Auteur : Pierrick Muller
* Date : 5.11.2019
* Version : 0.1
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
#define DEVICE_NAME     "test2"

#define BASE_ADDR          0xFF200000
#define BASE_SIZE          4096



int ret;
int i;
void * addr_base;


static ssize_t
test2_write(struct file *filp, const char __user *buf,
             size_t count, loff_t *ppos)
{
	
	unsigned long ret;
	char * message[32];
	long * temp_result = 0;
	
	vuint* addr_enable = addr_base + 0x10;

	
	//Si la taille de ce que le user veut écrire est 0, on ne fait rien et on retourne 0
	if (count == 0) {
        	return 0;
    	}
	
	//On place le pointeur au début du fichier
    *ppos = 0;
	
	ret = copy_from_user(message,buf,32);
	
	if(strcmp(message,"0") == 0)
	{
		*addr_enable = 0x0;
	}
	else 
	{
		*addr_enable = 0x1; 
	}
    
    return count;
}




/*Cette structure permet de préciser quelles fonctions contenues dans ce fichier correspondent a quelles fonctions propres à un fichier.
* On peut voir notamment le read et le write.
*/
const static struct
file_operations test2_fops = {
    	.owner         = THIS_MODULE,
    	.write         = test2_write,

};

static struct
cdev *test2_cdev;

static int
__init test2_init(void)
{
	//Structure permettant de représenter une device id 
	dev_t test2_dev = MKDEV(MAJOR_NUM,0);

	//On enregistre une range de device_number
	register_chrdev_region(test2_dev,1,DEVICE_NAME);
	
	//On crée la structure cdev pour notre module et on charge le module parrot
	test2_cdev = cdev_alloc();
	test2_cdev->ops = &test2_fops;
	test2_cdev->owner = THIS_MODULE;
	cdev_add(test2_cdev,test2_dev,1);

	addr_base = ioremap(BASE_ADDR, BASE_SIZE);
	
	//On ajoute au log l'information que le module est chargé et prêt
    printk(KERN_INFO "test2 ready!\n");

    return 0;
}

static void
__exit test2_exit(void)
{
	//Si le buffer n'est pas vide, on libère la mémoire qui lui était alloué


	// On désenregistre le driver parrot de la liste des modules chargés
	cdev_del(test2_cdev);
	//On ajoute au log l'information que le module Parrot à été supprimé des modules chargés
    	printk(KERN_INFO "test2 bye!\n");
}

//Information sur la licence et l'auteur
MODULE_AUTHOR("REDS");
MODULE_LICENSE("GPL");

//ajout des fonction init et exit en tant que fonction d'initialisation du module et de suppression du module.
module_init(test2_init);
module_exit(test2_exit);
