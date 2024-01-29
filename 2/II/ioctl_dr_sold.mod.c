#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
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

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif


static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x8b375d72, "cdev_init" },
	{ 0xef692a70, "cdev_add" },
	{ 0xc0841d02, "__class_create" },
	{ 0x37b5f2b0, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x8d4274d7, "device_create" },
	{ 0xb4794a3e, "class_destroy" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x6a2ed50b, "find_get_pid" },
	{ 0xebb286b, "pid_task" },
	{ 0xd0da656b, "__stack_chk_fail" },
	{ 0x92997ed8, "_printk" },
	{ 0xe1537255, "__list_del_entry_valid" },
	{ 0x68f31cbd, "__list_add_valid" },
	{ 0xee492aed, "device_destroy" },
	{ 0x517f61ad, "module_layout" },
};

MODULE_INFO(depends, "");

