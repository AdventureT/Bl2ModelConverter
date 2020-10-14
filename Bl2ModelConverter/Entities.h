#pragma once
#include "trb2.h"

namespace Bl2ModelConverter {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Entities
	/// </summary>
	public ref class Entities : public System::Windows::Forms::Form
	{
	public:
		Entities(struct trb2::Props* prop)
		{
			InitializeComponent();
			props = prop;
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Entities()
		{
			if (components)
			{
				delete components;
			}
		}

		  struct trb2::Props* props;
	public: System::Windows::Forms::ListView^ listView1;
	private: System::Windows::Forms::ListView^ listView2;
	private: System::Windows::Forms::ColumnHeader^ Hname;
	private: System::Windows::Forms::ColumnHeader^ Htype;
	private: System::Windows::Forms::ColumnHeader^ Hvalue;


	public:
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->listView1 = (gcnew System::Windows::Forms::ListView());
			this->listView2 = (gcnew System::Windows::Forms::ListView());
			this->Hname = (gcnew System::Windows::Forms::ColumnHeader());
			this->Htype = (gcnew System::Windows::Forms::ColumnHeader());
			this->Hvalue = (gcnew System::Windows::Forms::ColumnHeader());
			this->SuspendLayout();
			// 
			// listView1
			// 
			this->listView1->HideSelection = false;
			this->listView1->Location = System::Drawing::Point(13, 13);
			this->listView1->Name = L"listView1";
			this->listView1->Size = System::Drawing::Size(194, 472);
			this->listView1->TabIndex = 2;
			this->listView1->UseCompatibleStateImageBehavior = false;
			this->listView1->View = System::Windows::Forms::View::SmallIcon;
			this->listView1->SelectedIndexChanged += gcnew System::EventHandler(this, &Entities::listView1_SelectedIndexChanged);
			this->listView1->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &Entities::listViewItem_MouseDoubleClick);
			// 
			// listView2
			// 
			this->listView2->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(3) {
				this->Hname, this->Htype,
					this->Hvalue
			});
			this->listView2->HideSelection = false;
			this->listView2->LabelEdit = true;
			this->listView2->Location = System::Drawing::Point(213, 13);
			this->listView2->Name = L"listView2";
			this->listView2->Size = System::Drawing::Size(871, 472);
			this->listView2->TabIndex = 2;
			this->listView2->UseCompatibleStateImageBehavior = false;
			this->listView2->View = System::Windows::Forms::View::Details;
			this->listView2->SelectedIndexChanged += gcnew System::EventHandler(this, &Entities::listView2_SelectedIndexChanged);
			// 
			// Hname
			// 
			this->Hname->Text = L"Name";
			this->Hname->Width = 248;
			// 
			// Htype
			// 
			this->Htype->Text = L"Type";
			this->Htype->Width = 105;
			// 
			// Hvalue
			// 
			this->Hvalue->Text = L"Value";
			this->Hvalue->Width = 514;
			// 
			// Entities
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1228, 604);
			this->Controls->Add(this->listView2);
			this->Controls->Add(this->listView1);
			this->Name = L"Entities";
			this->Text = L"Entities";
			this->Load += gcnew System::EventHandler(this, &Entities::Entities_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void Entities_Load(System::Object^ sender, System::EventArgs^ e) {

	}
	private: System::Void listBox1_MouseDoubleClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
	}





	private: System::Void listView1_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		listView2->Items->Clear();
		if (listView1->SelectedIndices->Count == 0) return;
		for (size_t i = 0; i < props->propNames[listView1->SelectedIndices[0]].size(); i++)
		{
			ListViewItem^ lvi = gcnew ListViewItem(gcnew System::String(props->propNames[listView1->SelectedIndices[0]][i].c_str()));
			lvi->SubItems->Add(gcnew System::String(props->types[listView1->SelectedIndices[0]][i].c_str()));
			lvi->SubItems->Add(gcnew System::String(props->values[listView1->SelectedIndices[0]][i].c_str()));
			listView2->Items->Add(lvi);
			//listView2->Items->Add(gcnew System::String(props->propNames[listView1->SelectedIndices[0]][i].c_str()) + "\n");
		}
		
	}
	private: System::Void listViewItem_MouseDoubleClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {

	}
	private: System::Void listView2_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		/*textBox1->Clear();
		if (listView2->SelectedIndices->Count == 0) return;
		textBox1->Text = gcnew System::String(props->values[listView1->SelectedIndices[0]][listView2->SelectedIndices[0]].c_str());*/
	}
};
}
