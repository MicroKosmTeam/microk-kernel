#include <module/modulemanager.hpp>
#include <sys/printk.hpp>
#include <mm/memory.hpp>

struct ModuleNode {
	MKMI_Module Module;
	PROC::Process *Process;

	ModuleNode *Previous;
	ModuleNode *Next;
};

ModuleNode *First;
ModuleNode *Last;

namespace MODULE {
namespace Manager {
ModuleNode *FindModule(MKMI_ModuleID id) {
	ModuleNode *node = Last;

	while (node->Previous) {
		if (node->Module.ID.Codename == id.Codename &&
		    node->Module.ID.Writer == id.Writer) {
			break;
		}

		node = node->Previous;
	}

	return node;
}

uint64_t RegisterModule(MKMI_Module module, PROC::Process *mainProcess) {
	ModuleNode *newNode = new ModuleNode;

	newNode->Process = mainProcess;

	newNode->Module = module;

	newNode->Previous = Last;
	newNode->Next = NULL;

	Last->Next = newNode;
	Last = Last->Next;

	PRINTK::PrintK("Module 0x%x by 0x%x registered.\r\n",
			newNode->Module.ID.Codename,
			newNode->Module.ID.Writer);

	return 0;
}

uint64_t UnregisterModule(MKMI_ModuleID id) {
	ModuleNode *node = FindModule(id);
	if (node == NULL) return 1;

	uint64_t codename = node->Module.ID.Codename;
	uint64_t writer = node->Module.ID.Writer;

	node->Previous->Next = node->Next;
	if (node->Next != NULL) node->Next->Previous = node->Previous;

	delete node;

	PRINTK::PrintK("Module 0x%x by 0x%x unregistered.\r\n",
			codename,
			writer);

	return 0;
}

MKMI_Module *GetModuleData(MKMI_ModuleID id) {
	ModuleNode *node = FindModule(id);
	if (node == NULL) return NULL;

	return &node->Module;
}

PROC::Process *GetModuleProcess(MKMI_ModuleID id) {
	ModuleNode *node = FindModule(id);
	if (node == NULL || node->Process == NULL) return NULL;

	return node->Process;
}

void Init() {
	First = new ModuleNode;
	First->Next = NULL;
	First->Previous = NULL;
	Last = First;
}

}
}
