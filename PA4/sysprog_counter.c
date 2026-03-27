#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>

#define PROC_NAME "sysprog_counter"
#define BUFFER_SIZE 128

/* this variable stores the counter value */
static int counter = 0;

/* pointer for the /proc file entry */
static struct proc_dir_entry *proc_entry;

/* function to read the counter value from /proc */
static ssize_t proc_read(struct file *file, char __user *user_buffer,
                         size_t count, loff_t *pos)
{
    char kernel_buf[BUFFER_SIZE];
    int len;

    /* if file was already read once, return 0 to show EOF */
    if (*pos > 0)
        return 0;

    /* put the counter value into a string */
    len = snprintf(kernel_buf, sizeof(kernel_buf), "Counter value: %d\n", counter);

    /* make sure we do not copy more than user asked for */
    if (len > count)
        len = count;

    /* copy data from kernel space to user space */
    if (copy_to_user(user_buffer, kernel_buf, len))
        return -EFAULT;

    /* update file position */
    *pos += len;
    return len;
}

/* function to write a new counter value into /proc */
static ssize_t proc_write(struct file *file, const char __user *user_buffer,
                          size_t count, loff_t *pos)
{
    char kernel_buf[BUFFER_SIZE];
    int new_value;
    size_t copy_len;

    /* check if input is too big for our buffer */
    if (count >= BUFFER_SIZE) {
        printk(KERN_WARNING "sysprog_counter: input too large (%zu bytes)\n", count);
        return -EINVAL;
    }

    copy_len = count;

    /* copy input from user space into kernel buffer */
    if (copy_from_user(kernel_buf, user_buffer, copy_len))
        return -EFAULT;

    /* add null character so it becomes a proper string */
    kernel_buf[copy_len] = '\0';

    /* convert input string into integer */
    if (kstrtoint(strstrip(kernel_buf), 10, &new_value)) {
        printk(KERN_WARNING "sysprog_counter: invalid integer input\n");
        return -EINVAL;
    }

    /* update the counter value */
    counter = new_value;
    printk(KERN_INFO "sysprog_counter: counter updated to %d\n", counter);

    return count;
}

/* connects read and write functions to the /proc file */
static const struct proc_ops proc_file_ops = {
    .proc_read  = proc_read,
    .proc_write = proc_write,
};

/* this runs when the module is loaded */
static int __init sysprog_init(void)
{
    /* create the /proc file */
    proc_entry = proc_create(PROC_NAME, 0666, NULL, &proc_file_ops);
    if (!proc_entry) {
        printk(KERN_ERR "sysprog_counter: failed to create /proc/%s\n", PROC_NAME);
        return -ENOMEM;
    }

    printk(KERN_INFO "sysprog_counter: module loaded — /proc/%s created\n", PROC_NAME);
    return 0;
}

/* this runs when the module is removed */
static void __exit sysprog_exit(void)
{
    /* remove the /proc file */
    proc_remove(proc_entry);
    printk(KERN_INFO "sysprog_counter: module unloaded — /proc/%s removed\n", PROC_NAME);
}

/* tells kernel which functions to call on load and unload */
module_init(sysprog_init);
module_exit(sysprog_exit);
