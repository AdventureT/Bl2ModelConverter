#pragma once

namespace Bl2ModelConverter {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for LocaleStringsForm
	/// </summary>
	public ref class LocaleStringsForm : public System::Windows::Forms::Form
	{
	public:
		LocaleStringsForm(void)
		{
			InitializeComponent();
			listcollection->Clear();
			for each (System::String ^ str in listcollection)
			{
				listcollection->Add(str);
				textBox1->CharacterCasing = CharacterCasing::Lower;
			}
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~LocaleStringsForm()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::ListBox^ listBox1;
	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::Label^ label1;
	protected:
		System::Collections::Generic::List<System::String^>^ listcollection = gcnew System::Collections::Generic::List<System::String^>();
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
			this->listBox1 = (gcnew System::Windows::Forms::ListBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// listBox1
			// 
			this->listBox1->FormattingEnabled = true;
			this->listBox1->Location = System::Drawing::Point(100, 86);
			this->listBox1->Name = L"listBox1";
			this->listBox1->Size = System::Drawing::Size(565, 316);
			this->listBox1->TabIndex = 0;
			this->listBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &LocaleStringsForm::listBox1_SelectedIndexChanged);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(251, 51);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(202, 20);
			this->textBox1->TabIndex = 1;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(204, 54);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(41, 13);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Search";
			// 
			// LocaleStringsForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(785, 459);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->listBox1);
			this->Name = L"LocaleStringsForm";
			this->Text = L"LocaleStringsForm";
			this->Load += gcnew System::EventHandler(this, &LocaleStringsForm::LocaleStringsForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void listBox1_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		if (!System::String::IsNullOrEmpty(textBox1->Text))
		{
			listBox1->Items->Clear();
			for each (System::String^ str in listcollection)
			{
				if (str->StartsWith(textBox1->Text)) listBox1->Items->Add(str);
			}
		}
		else if (textBox1->Text == "")
		{
			listBox1->Items->Clear();
			for each (System::String^ str in listcollection)
			{
				if (str->StartsWith(textBox1->Text)) listBox1->Items->Add(str);
			}
		}
	}
	private: System::Void LocaleStringsForm_Load(System::Object^ sender, System::EventArgs^ e) {
	}
};
}
