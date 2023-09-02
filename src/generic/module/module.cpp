#include <module/module.hpp>
#include <sys/printk.hpp>

namespace MODULE {
Module::Module(uint32_t vendorID, uint32_t productID, PROC::ProcessBase *proc) {
	VendorID = vendorID;
	ProductID = productID;
	Process = proc;
}
}
