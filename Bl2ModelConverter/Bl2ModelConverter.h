#pragma once
#include "trb2.h"

namespace Bl2ModelConverter {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;
	/// <summary>
	/// Summary for Bl2ModelConverter
	/// </summary>
	public ref class Bl2ModelConverter : public System::Windows::Forms::Form
	{
	public:
		Bl2ModelConverter(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Bl2ModelConverter()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ button1;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog1;
	private: System::Windows::Forms::Button^ button2;

	private: System::Windows::Forms::ListView^ listView1;
	private: System::Windows::Forms::ColumnHeader^ Type;
	private: System::Windows::Forms::ColumnHeader^ columnHeader1;
	private: System::Windows::Forms::SaveFileDialog^ saveFileDialog1;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Button^ button3;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog2;


	public:
		   char* filename = new char[256];

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->listView1 = (gcnew System::Windows::Forms::ListView());
			this->Type = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->openFileDialog2 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(174, 392);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(138, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Load Trb File";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Bl2ModelConverter::button1_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(554, 170);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(85, 23);
			this->button2->TabIndex = 1;
			this->button2->Text = L"Save as";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Bl2ModelConverter::button2_Click);
			// 
			// listView1
			// 
			this->listView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(2) { this->Type, this->columnHeader1 });
			this->listView1->FullRowSelect = true;
			this->listView1->GridLines = true;
			this->listView1->HideSelection = false;
			this->listView1->Location = System::Drawing::Point(66, 58);
			this->listView1->Name = L"listView1";
			this->listView1->Size = System::Drawing::Size(419, 298);
			this->listView1->TabIndex = 3;
			this->listView1->UseCompatibleStateImageBehavior = false;
			this->listView1->View = System::Windows::Forms::View::Details;
			// 
			// Type
			// 
			this->Type->Text = L"Type";
			this->Type->Width = 68;
			// 
			// columnHeader1
			// 
			this->columnHeader1->Text = L"Filename";
			this->columnHeader1->Width = 293;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(511, 196);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(176, 13);
			this->label1->TabIndex = 4;
			this->label1->Text = L"Currently supports PMDL, PTEX and PCOL";
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(554, 301);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(75, 23);
			this->button3->TabIndex = 5;
			this->button3->Text = L"Import";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Bl2ModelConverter::button3_Click);
			// 
			// openFileDialog2
			// 
			this->openFileDialog2->FileName = L"openFileDialog2";
			// 
			// Bl2ModelConverter
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(891, 473);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->listView1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Name = L"Bl2ModelConverter";
			this->Text = L" ";
			this->Load += gcnew System::EventHandler(this, &Bl2ModelConverter::Bl2ModelConverter_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Bl2ModelConverter_Load(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		openFileDialog1->FileName = "Select a Trb Model file";
		openFileDialog1->Filter = "Trb Model file (*.trb)|*.trb";
		openFileDialog1->Title = "Open Trb Model File";
		openFileDialog1->Multiselect = false;
		if (openFileDialog1->ShowDialog() == Windows::Forms::DialogResult::OK)
			listView1->Items->Clear();
			filename = (char*)(void*)Marshal::StringToHGlobalAnsi(openFileDialog1->FileName);
			Trb2::trb2 trb = Trb2::trb2::trb2(filename,1);
			trb.readHeader();
			std::vector<std::string> dataInfo = trb.readListBoxInfos();
			for (int i = 0; i < dataInfo.size(); i+=3)
			{
				ListViewItem^ lvi = gcnew ListViewItem(gcnew System::String(dataInfo[i].c_str()));
				lvi->SubItems->Add(gcnew System::String(dataInfo[i+1].c_str()));
				lvi->SubItems->Add(gcnew System::String(dataInfo[i + 2].c_str()));
				listView1->Items->Add(lvi);
			}
			listView1->Sorting = SortOrder::Ascending;
	}
	private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
		auto selected = listView1->SelectedItems;
		System::String^ filter;
		for (int i = 0; i < selected->Count; i++)
		{
			if (selected[i]->Text == "PMDL")
			{
				filter = "Filmbox Model File (*.fbx)|*.fbx";
			}
			else if (selected[i]->Text == "PTEX")
			{
				filter = "DirectDraw Surface (*.dds)|*.dds";
			}
			else if (selected[i]->Text == "PCOL")
			{
				filter = "Havok Packed file (*.hkx)|*.hkx";
			}
			else
			{
				MessageBox::Show("Sorry, this type is not supported yet.");
				return;
			}
		}
		saveFileDialog1->FileName = selected[0]->SubItems[1]->Text;
		saveFileDialog1->Filter = filter;
		saveFileDialog1->Title = "Save";
		if (saveFileDialog1->ShowDialog() == Windows::Forms::DialogResult::OK)
		{
			System::IO::FileInfo^ fi = gcnew System::IO::FileInfo(saveFileDialog1->FileName);
			std::string dir = (char*)(void*)Marshal::StringToHGlobalAnsi(fi->DirectoryName);
			dir += "\\";
			std::vector<int> indices;
			std::vector<std::string> fn;
			for (int i = 0; i < selected->Count; i++)
			{
				fn.push_back(dir + (char*)(void*)Marshal::StringToHGlobalAnsi(selected[i]->SubItems[1]->Text));
				indices.push_back(int::Parse(selected[i]->SubItems[2]->Text));
			}
			Trb2::trb2 trb = Trb2::trb2::trb2(filename, 1);
			trb.readHeader();
			trb.readData(indices, fn);
		}
	}
private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e) {
	saveFileDialog1->Filter = "DirectDraw Surface (*.dds)|*.dds";
	if (openFileDialog2->ShowDialog() == Windows::Forms::DialogResult::OK)
	{

	}

}
};
}
