#include <module/modulemanager.hpp>
#include <sys/printk.hpp>
#include <mm/memory.hpp>

struct ModuleNode {
	MKMI_Module *module;

	ModuleNode *previous;
	ModuleNode *next;
};

ModuleNode *first;
ModuleNode *last;

namespace MODULE {
namespace Manager {
ModuleNode *FindModule(MKMI_ModuleID id) {
	ModuleNode *node = last;

	while (node->previous) {
		if (node->module->ID.Codename == id.Codename &&
		    node->module->ID.Writer == id.Writer) {
			break;
		}

		node = node->previous;
	}

	return node;
}

uint64_t RegisterModule(MKMI_Module module) {
	ModuleNode *newNode = last->next;
	newNode = new ModuleNode;
	newNode->module = new MKMI_Module;
	memcpy(newNode->module, &module, sizeof(MKMI_Module));
	newNode->previous = last;
	newNode->next = NULL;
	last = newNode;

	PRINTK::PrintK("Module 0x%x by 0x%x registered.\r\n",
			newNode->module->ID.Codename,
			newNode->module->ID.Writer);

	return 0;
}

uint64_t UnregisterModule(MKMI_ModuleID id) {
	ModuleNode *node = FindModule(id);
	if (node == NULL || node->module == NULL) return 1;

	uint64_t codename = node->module->ID.Codename;
	uint64_t writer = node->module->ID.Writer;

	delete node->module;
	node->previous->next = node->next;
	if (node->next != NULL) node->next->previous = node->previous;

	delete node;

	PRINTK::PrintK("Module 0x%x by 0x%x unregistered.\r\n",
			codename,
			writer);

	return 0;
}

MKMI_Module *GetModule(MKMI_ModuleID id) {
	ModuleNode *node = FindModule(id);
	if (node == NULL || node->module == NULL) return NULL;

	return node->module;
}

void Init() {
	first = new ModuleNode;
	first->module = NULL;
	first->next = NULL;
	first->previous = NULL;
	last = first;
}

}
}
