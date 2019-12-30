#include "Bl2ModelConverter.h"
using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main() {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew Bl2ModelConverter::Bl2ModelConverter);
	return 0;
}