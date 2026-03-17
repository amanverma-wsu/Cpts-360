#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x9036a98a, "proc_create" },
	{ 0xe8213e80, "_printk" },
	{ 0x8ae926c1, "proc_remove" },
	{ 0x0e9cab28, "memset" },
	{ 0x546c19d9, "validate_usercopy_range" },
	{ 0xa61fd7aa, "__check_object_size" },
	{ 0xaa47b76e, "__arch_copy_from_user" },
	{ 0x90a48d82, "__ubsan_handle_out_of_bounds" },
	{ 0x41495f0d, "strim" },
	{ 0xd09b06f5, "kstrtoint" },
	{ 0x9c4ed43a, "alt_cb_patch_nops" },
	{ 0xd272d446, "__stack_chk_fail" },
	{ 0xe54e0a6b, "__fortify_panic" },
	{ 0x40a621c5, "snprintf" },
	{ 0xaa47b76e, "__arch_copy_to_user" },
	{ 0xdc1bea0d, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0x9036a98a,
	0xe8213e80,
	0x8ae926c1,
	0x0e9cab28,
	0x546c19d9,
	0xa61fd7aa,
	0xaa47b76e,
	0x90a48d82,
	0x41495f0d,
	0xd09b06f5,
	0x9c4ed43a,
	0xd272d446,
	0xe54e0a6b,
	0x40a621c5,
	0xaa47b76e,
	0xdc1bea0d,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"proc_create\0"
	"_printk\0"
	"proc_remove\0"
	"memset\0"
	"validate_usercopy_range\0"
	"__check_object_size\0"
	"__arch_copy_from_user\0"
	"__ubsan_handle_out_of_bounds\0"
	"strim\0"
	"kstrtoint\0"
	"alt_cb_patch_nops\0"
	"__stack_chk_fail\0"
	"__fortify_panic\0"
	"snprintf\0"
	"__arch_copy_to_user\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "A3A6DD090CA7AC377EAEBB9");
