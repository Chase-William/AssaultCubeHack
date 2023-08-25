#include "Drawing.h"
#include "Injector.h"
#include <optional>

bool Drawing::show = true;
PROCESSENTRY32 Drawing::proc = {};

// ImGuiInputTextCallback

std::vector<std::string> names = Win32Helper::getProcs();

// ImGui::ComboAutoSelectData data = { names };

static const char* item_getter1(const std::vector<std::string>& items, int index) {
	if (index >= 0 && index < (int)items.size()) {
		return items[index].c_str();
	}
	return "N/A";
}

void Drawing::Draw() {
	if (!show)
		return;
	ImGui::SetNextWindowSize({ 350, 300 }, ImGuiCond_Once);
	if (ImGui::Begin("Main Window", 0, 0)) {
		// std::string input = {};


		ImGui::Text("Enter Process Name:");
		//ImGui::InputText("##", &input, ImGuiInputTextFlags_::ImGuiInputTextFlags_CallbackEdit, [](ImGuiInputTextCallbackData* data) -> int {
		//	// std::cout << data->Buf << "\n";
		//	names = Win32Helper::getProcs(data->Buf);						
		//	return ImGuiInputTextFlags_::ImGuiInputTextFlags_None;
		//});

		//if (ImGui::ComboAutoSelect("Processes", data)) {
		//	// std::cout << "Index: " << data.index << " Input: " << data.input << "\n";
		//	std::string selected = names.at(data.index);			
		//}

		// static std::vector<std::string> items1{ "instruction", "Chemistry", "Beating Around the Bush", "Instantaneous Combustion", "Level 999999", "nasal problems", "On cloud nine", "break the iceberg", "lacircificane" };
		static int selected_item1 = -1;
		if (ImGui::ComboAutoSelect(
			"std::vector combo", 
			selected_item1, 
			names, 
			item_getter1,
			[](ImGuiInputTextCallbackData* data) -> int {
				names = Win32Helper::getProcs(data->Buf);
				// Default return is 0
				return NULL;
			},
			ImGuiComboFlags_HeightRegular
			//ImGuiComboFlags_HeightSmall
		)) {
			/* Selection made */
			Drawing::proc = Win32Helper::getProcAddr(names.at(selected_item1));
		}

		ImGui::Text(std::format("Process Id: {0}", proc.th32ProcessID).c_str());
		ImGui::Text(std::format("Thread Count: {0}", proc.cntThreads).c_str());
		ImGui::Text(std::format("Parent Process: {0}", proc.th32ParentProcessID).c_str());

		static std::string dllPath{};
		ImGui::InputText("Dll Path", &dllPath);
		static std::optional<bool> injectionSuccess{};		
		if (ImGui::Button("Inject")) {
			int size = dllPath.length() + 1;
			char* pathBuf = new char[size];
			strcpy_s(pathBuf, size, dllPath.c_str());
			injectionSuccess = Injector::ClassicInject(Drawing::proc.th32ProcessID, pathBuf);
		}
		ImGui::SameLine();
		if (injectionSuccess.has_value())
			ImGui::Text(injectionSuccess ? "Injected" : "Failed");

		/*ImGui::Text("Processes Found:");
		if (ImGui::BeginListBox("##", {300, 200})) {
			for (std::string name : names) {				
				ImGui::Text(name.c_str());
			}

			ImGui::EndListBox();
		}*/

		ImGui::End();
	}
}

void Drawing::Show()
{
	Drawing::show = true;
}

void Drawing::Hide()
{
	Drawing::show = false;
}
