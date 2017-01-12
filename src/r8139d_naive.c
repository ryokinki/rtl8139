#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/pci.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Martin Wetterwald");
MODULE_DESCRIPTION("A learning-purpose naive Realtek 8139D driver implementation");

static struct net_device * r8139dn;

static struct net_device_ops r8139dn_ops =
{
};

static struct pci_device_id r8139dn_pci_id_table[] =
{
    {PCI_DEVICE(PCI_VENDOR_ID_REALTEK, PCI_DEVICE_ID_REALTEK_8139)},
    {0, },
};

MODULE_DEVICE_TABLE(pci, r8139dn_pci_id_table);

static int r8139dn_pci_probe(struct pci_dev *pdev, const struct pci_device_id *id)
{
    pr_info("Hello!\n");

    if (pdev->revision != 0x10)
    {
        pr_err("This device (rtl8139 revision %02x) is not supported\n", pdev->revision);
        return -ENODEV;
    }

    r8139dn = alloc_etherdev(0);
    if (!r8139dn)
    {
        return -ENOMEM;
    }

    r8139dn->netdev_ops = &r8139dn_ops;
    register_netdev(r8139dn);

    return 0;
}

static void r8139dn_pci_remove(struct pci_dev *dev)
{
    pr_info("Bye!\n");
    unregister_netdev(r8139dn);
    free_netdev(r8139dn);
}

static struct pci_driver r8139dn_pci_driver =
{
    .name = KBUILD_MODNAME,
    .id_table = r8139dn_pci_id_table,
    .probe = r8139dn_pci_probe,
    .remove = r8139dn_pci_remove,
};

module_pci_driver(r8139dn_pci_driver);
