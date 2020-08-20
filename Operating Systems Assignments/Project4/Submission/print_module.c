/*  
 *  print_module for Project 4
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h> 
#include <linux/cdev.h>
#include <linux/proc_fs.h>
#include <linux/pid.h>
#include <linux/pid_namespace.h>
#include <linux/init.h>
#include <asm/siginfo.h>
#include <linux/rcupdate.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/signal.h>
#include <linux/debugfs.h>
#include <linux/fdtable.h>
#include <linux/fs_struct.h>
#include <linux/dcache.h>
#include <linux/slab.h>
#include <linux/radix-tree.h>
#include <linux/mm_types.h>
#include <linux/page_ref.h>
#include <linux/device.h>
#include <linux/memremap.h>
#include <linux/pm_wakeup.h>
#include <linux/buffer_head.h>

// Number of pages to print
#define __PRINT_PAGE_LIMIT 100

static int processpid = 5;
struct pid *pid_struct;
struct task_struct *task;

module_param(processpid, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

static int __init hello_5_init(void)
{
	struct files_struct *current_files; 
	struct fs_struct *current_fs;
	struct fdtable *files_table;
	int i;
	unsigned long page_counter;

	// For part 2
	struct file *curr_fd;
	struct inode *curr_inode;
	struct address_space *curr_add_space;

	// For part 3
	struct dentry *pwd;
	char *cwd;
	char cwd_buff[1000];

	// To access and print page cache
	struct radix_tree_root radix_root;
	struct radix_tree_iter radix_iter;
	void **slot;
	struct page *page;
	struct buffer_head *buff_head;

	printk(KERN_INFO "Entering: print_module\n");

	// Find the PCB of the desired process.
	task = pid_task(find_get_pid(processpid), PIDTYPE_PID);
	if (task == NULL) {
		printk(KERN_INFO "1. No process with pid %d is found.\n", processpid);
		return 0;
	}
	printk(KERN_INFO "1. PCB of process with pid %d is found.\n", task->pid);	 

	// Get the necessary structs
	current_files = task->files;
	current_fs = task->fs;
	files_table = files_fdtable(current_files);

	// Check if there are open files
	i = 0;
	page_counter = 0;
	if (files_table->fd[i] == NULL) {
		printk(KERN_INFO "2. Process has no open files.\n");		
	}
	else {
		// Print information about open files
		printk(KERN_INFO "2. Process has open files:\n");
		while(files_table->fd[i] != NULL) {
			curr_fd = files_table->fd[i];
			curr_inode = curr_fd->f_inode;
			curr_add_space = curr_fd->f_mapping;

			printk(KERN_INFO "3.%d. Open file with fd: %d", i+1, i);
			printk(KERN_INFO "3.%d. Current file position pointer: %lld", i+1, curr_fd->f_pos);
			printk(KERN_INFO "3.%d. File name: %s", i+1, ((curr_fd->f_path).dentry->d_name).name);
			inode_lock(curr_inode);
			printk(KERN_INFO "3.%d. User's ID: %ld", i+1, curr_inode->i_uid);
			printk(KERN_INFO "3.%d. File mode: %d", i+1, curr_fd->f_mode);
			printk(KERN_INFO "3.%d. Inode number: %ld", i+1, curr_inode->i_ino);
			printk(KERN_INFO "3.%d. File size in bytes: %lld", i+1, curr_inode->i_size);
			printk(KERN_INFO "3.%d. Number of blocks allocated to the file: %ld", i+1, curr_inode->i_blocks);
			page_counter += curr_add_space->nrpages;
			inode_unlock(curr_inode);

			i++;
		}	
	}

	// Print current working directory and page cache size
	pwd = (current_fs->pwd).dentry;
	cwd = dentry_path_raw(pwd, cwd_buff, 999);
	printk(KERN_INFO "4.1. Current working directory: %s\n", cwd);

	// Reuse variables for the next step
	curr_inode = pwd->d_inode; 
	curr_add_space = curr_inode->i_mapping;
	printk(KERN_INFO "4.2. Number of pages in the buffer: %lu\n", page_counter);

	// Access page cache
	if (page_counter == 0) {
		printk(KERN_INFO "5. Process has no pages.\n");		
	}
	else {
		// Print information about open pages from open files
		i = 0;
		page_counter = 0;
		printk(KERN_INFO "5. Process has cached pages:\n");
		while(files_table->fd[i] != NULL) {
			curr_fd = files_table->fd[i];
			curr_inode = curr_fd->f_inode;
			curr_add_space = curr_fd->f_mapping;
			i++;

			radix_root = curr_add_space->i_pages;
			radix_tree_for_each_slot(slot, &radix_root, &radix_iter, 0) {
				page = radix_tree_deref_slot(slot);
				if (page != NULL) {
					buff_head = (struct buffer_head*)(page->private);

					if (buff_head != NULL) {
						printk(KERN_INFO "5.%lu. Page block in device with ID: %ld\n", page_counter+1, buff_head->b_bdev->bd_dev);
						printk(KERN_INFO "5.%lu. Page block number: %ld\n", page_counter+1, buff_head->b_blocknr);
						printk(KERN_INFO "5.%lu. Page use count: %ld\n", page_counter+1, page_ref_count(page));

						if (++page_counter >= __PRINT_PAGE_LIMIT) {
							break;
						}
					}					
				}				
			}

			if (page_counter >= __PRINT_PAGE_LIMIT) {
				break;
			}
		}
	}

	// Cleanup and return
	return 0;
}

static void __exit hello_5_exit(void)
{
	printk(KERN_INFO "Exiting: print_module\n");
}

MODULE_LICENSE("Dual BSD/GPL");
module_init(hello_5_init);
module_exit(hello_5_exit);
